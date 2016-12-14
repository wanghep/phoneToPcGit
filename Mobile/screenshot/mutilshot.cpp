/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */ 

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <binder/ProcessState.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/ISurfaceComposer.h>
#include <ui/PixelFormat.h>
#include <SkImageEncoder.h>
#include <SkBitmap.h>
#include <SkData.h>
#include <SkStream.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>  
#include <sys/types.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "mutilshot.h"
#include "RawToJpg.h"
#include <stdlib.h>
#include <linux/delay.h>
#include "ServerSocket.h"
#include <signal.h>
#include <netdb.h>  
#include <stdarg.h>
#include <dirent.h>
#include <sys/un.h>
#include <errno.h>

//extern int errno;

using namespace android;
#define LOG_TAG "mutilshot"

static uint32_t DEFAULT_DISPLAY_ID = ISurfaceComposer::eDisplayIdMain;

int agent_to_apk_fd;
int agent_to_pc_socket = 0;  //clear and init socket fd
unsigned char pc_ipaddr[40];
bool hasIpAddress = false;
#define MAXLINE 4096
bool isPcClientRunning = false;   //pc client thread whether running
bool isMonkeyRunning = false;     //check monkey whehter running

/*
Function Description: get service pic by name
*/
int getPidByName(char* task_name)
{
    DIR *dir;
    struct dirent *ptr;
    FILE *fp;
    char cur_task_name[1024];
    char buf[BUF_SIZE];
    dir = opendir(PROC_DIRECTORY); 
    char chrarry_CommandLinePath[100];
    char chrarry_NameOfProcess[300];
    char* chrptr_StringToCompare = NULL;
	int target_pid = -1;
    if (NULL != dir)
    {   
        while ((ptr = readdir(dir)) != NULL) 
        {
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))            
				continue;
            if (DT_DIR != ptr->d_type)
        		continue;
			strcpy(chrarry_CommandLinePath, PROC_DIRECTORY) ;
			strcat(chrarry_CommandLinePath, ptr->d_name) ;
			strcat(chrarry_CommandLinePath, "/cmdline") ;
            FILE* fp = fopen (chrarry_CommandLinePath, "rt") ;
            if (NULL != fp)
            {
                if( fgets(buf, BUF_SIZE - 1, fp)== NULL ){
           	 		fclose(fp);
            		continue;
         		}
      			sscanf(buf, "%*s %s", cur_task_name);
                if(strcasestr(buf, task_name)){
					 //printf("PID:  %s\n", ptr->d_name);
					 target_pid = (int)atoi(ptr->d_name);
					 return target_pid;
				}
                fclose(fp);
            }
        }
        closedir(dir);
    }
	return -1;
}  

/*
Description: launch monkey thread
*/
void *monkey_thd(void *argv){
	ALOGE("start launch monkey!\n");
	while(isMonkeyRunning){
		if(getPidByName(MONKEY_PROCESS) < 0){
			int ret = system("monkey --port 36667");   //bind monkey in 36667
			ALOGE( " system monkey return %d" , ret );
			ALOGE( " ERROR NO %s" , strerror(errno) );
			
		}
	}
	return 0;
}

static void usage(const char* pname)
{
    fprintf(stderr,
            "usage: %s [-hp] [-d display-id] [FILENAME]\n"
            "   -h: this message\n"
            "   -p: save the file as a png.\n"
            "   -d: specify the display id to capture, default %d.\n"
            "If FILENAME ends with .png it will be saved as a png.\n"
            "If FILENAME is not given, the results will be printed to stdout.\n",
            pname, DEFAULT_DISPLAY_ID
    );
}

static SkBitmap::Config flinger2skia(PixelFormat f)
{
    switch (f) {
        case PIXEL_FORMAT_RGB_565:
            return SkBitmap::kRGB_565_Config;
        default:
            return SkBitmap::kARGB_8888_Config;
    }
}

static status_t vinfoToPixelFormat(const fb_var_screeninfo& vinfo, uint32_t* bytespp, uint32_t* f)
{

    switch (vinfo.bits_per_pixel) {
        case 16:
            *f = PIXEL_FORMAT_RGB_565;
            *bytespp = 2;
            break;
        case 24:
            *f = PIXEL_FORMAT_RGB_888;
            *bytespp = 3;
            break;
        case 32:
            // TODO: do better decoding of vinfo here
            *f = PIXEL_FORMAT_RGBX_8888;
            *bytespp = 4;
            break;
        default:
            return BAD_VALUE;
    }
    return NO_ERROR;
}


/*  Write msg to a socket  */
ssize_t sendmsgcomplete(int sockd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *buffer;

	buffer = vptr;
	nleft = n;
	while (nleft > 0) {
		if ((nwritten = write(sockd, buffer, nleft)) <= 0) {
			if (errno == EINTR)
				nwritten = 0;
			else{
				ALOGE("enter sendmsgcomplete error!!!!!!!!!!!!!!!!\n");
				return -1;
			}
		}

		nleft -= nwritten;
		buffer += nwritten;
	}
	return n;
}

ssize_t senddata(int sockd, const char *buffer, size_t buf_size)
{
	int status = send(sockd, buffer, buf_size, MSG_NOSIGNAL);
	return status;
}
  
/*
Description: check whether need to reconnect capture screen thread and listening system command
*/
int check_capture_thread_status(){
	int ret = 0;
	ALOGE("[check_capture_thread_status] enter.\n");
	//check pc client whether is running 
	if(isPcClientRunning){
		isPcClientRunning = false;
	} 
	//sleep(10);   //delay for 200 milliseconds for pc and monkey thread exit
	//reconnect agent with pc
	ALOGE("start a new client with pc.\n");
	connect_pc_client_socket();  //reconnect pc client
	//screen_shot_to_pc();
	return ret;
}
   
/*
Description: Analyse command which come from apk
                If command is 0x06, that is IP addres command
*/
int apk_cmd_ana(char * buf, int buf_size){
	int ret = 0;
	char cmd_len[4];
	char cmd_type[4];
	int data_size = buf_size - 8;
	ALOGE("buf_size is: %d, data_size is: %d\n", buf_size, data_size);
	char *cmd_data = NULL;
	memset(cmd_len, 0, 4);
	memset(cmd_type, 0, 4);  
	if((cmd_data = (char *)malloc((data_size * sizeof(char)))) == NULL){
		printf("cmd_data malloc error!\n");
		exit(0);
	} 
	//copy command header length
	memset(cmd_data, 0, data_size);
	memset(pc_ipaddr, 0 ,40);
	memcpy(cmd_len, buf, 4);
	//copy command type
	memcpy(cmd_type, buf + 4, 4);
	//ALOGE("cmd_type is :%s\n", cmd_type);
	//copy command data
	memcpy(cmd_data, buf + 8, data_size);
	switch(cmd_type[0]){
		case APK_CMD_IP:
			//handle ip address
			memcpy(pc_ipaddr, cmd_data, data_size);
			ALOGE("[apk_cmd_ana]pc ip address is:%s\n", cmd_data);
			ALOGE("[apk_cmd_ana]pc_ipaddr is:%s\n", pc_ipaddr);
			ALOGE("[apk_cmd_ana]cmd_len is: %s, data_size is: %d\n", cmd_len, data_size);
			if(!hasIpAddress){
				hasIpAddress = true;
				break;
			}
			check_capture_thread_status();
			break;
		default:
			break;
	}
	free(cmd_data);
	return ret;
}

/*
Description: thread communication which agent to apk 
*/
void *agent_to_apk_thd(void *argv){
	char buff[4096];
	char data_length[4];
	char data_type[4];   //for image is 0x06
	char data_actual[2];
	int connfd = 0;
	int receCount = 0;
	int ret =0;
	printf("enter agent to apk thd\n!");
/*	struct sockaddr_in apk_client_addr;
	bzero(&apk_client_addr, sizeof(apk_client_addr));
	apk_client_addr.sin_family = AF_INET;	
	apk_client_addr.sin_addr.s_addr = htons(INADDR_ANY);	
	apk_client_addr.sin_port = htons(0);	*/
	agent_to_apk_fd= ::socket(AF_INET, SOCK_STREAM, 0);
	if (agent_to_apk_fd < 0) {
		perror("server");
		ALOGE("opend socket failed!\n");
		exit(0);  
	}
	/*ret = ::bind(agent_to_apk_fd, (struct sockaddr*)&apk_client_addr, sizeof(struct sockaddr));
	if(ret < 0){
		ALOGE("bind to apk error");
		printf("bind to apk error\n");
		exit(0);
	}   */
	printf("statrt to agent111111111111\n");
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(AGENT_APK_ADDRESS);	
	server_addr.sin_port = htons(APK_TO_AGENT);	
	socklen_t server_addr_length = sizeof(server_addr);
	printf("ready to connect with apk!\n");
	ALOGE("ready to connect with apk!");
	while(true){
		if((::connect(agent_to_apk_fd, (struct sockaddr *)&server_addr, server_addr_length)) < 0){
			continue;
		}
		printf("connect with apk success!\n");
		ALOGE("connect with apk success!");
		break;
	}
	memset(buff, 0, MAXLINE);
	memset(data_type, 0, 4);
	memset(data_length, 0, 4);
	memset(data_actual, 0, 2);
	data_type[0] = 0x11;     //ip command
	data_actual[0] = 0x12;   // ip buffer data to request ip address from apk
	data_length[0] = 0x05;
	memcpy(buff, data_length, 4);       //data length
	memcpy(buff + 4, data_type, 4);     //data type
	memcpy(buff + 8, data_actual, 2);   //actual data
	int status = ::send(agent_to_apk_fd, buff, 10, MSG_NOSIGNAL);
	if(status < 0){
		printf("agent to apk send error!\n");
		ALOGE("agent to apk send error!");
		exit(0);
	}
	ALOGE("start to receive socket from server! ");
	printf("start to receive socket from server!\n");
	while(true){
		memset(buff, 0, MAXLINE);
		//printf("[agent_to_apk_thd] start to accept connect.\n");
		receCount = ::recv(connfd, buff, MAXLINE, 0);
		if(receCount < 0){
			continue;
		}
		apk_cmd_ana(buff, receCount);
	}
	/*ret = ::bind(agent_to_apk_fd, (struct sockaddr*)&apk_client_addr, sizeof(struct sockaddr));
	if(ret < 0){
		ALOGE("bind to apk error");
		printf("bind to apk error\n");
		exit(0);
	} 
	if(::listen(agent_to_apk_fd, 50)< 0){
		ALOGE("listen apk error!\n");  
	} 
	ssize_t len = sizeof(struct sockaddr_in);
	ALOGE("start listening for client conecting.\n");
//	printf("start listening for client conecting.\n");
	while(true){
		memset(buff, 0, MAXLINE);
	//	printf("[agent_to_apk_thd] start to accept connect.\n");
		ALOGE("[agent_to_apk_thd] start to accept connect.\n");
		if((connfd = ::accept(agent_to_apk_fd, (struct sockaddr *)(&apk_client_addr), &len)) < 0){
			ALOGE("accept from apk is error!\n");
			continue;
		}
		ALOGE("[agent_to_apk_thd] start to recv buffer.\n");
		receCount = ::recv(connfd, buff, MAXLINE, 0);
		apk_cmd_ana(buff, receCount);
		close(connfd);
	}*/
	
	close(agent_to_apk_fd);
	return 0;
}
     
int connect_pc_client_socket(){
	int ret = 0;
	struct sockaddr_in pc_client_addr;
	agent_to_pc_socket = 0;
	pc_client_addr.sin_family = AF_INET;	
	pc_client_addr.sin_addr.s_addr = inet_addr(pc_ipaddr);	
//	pc_client_addr.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);	
	pc_client_addr.sin_port = htons(AGENT_TO_PC_PORT);	
	ALOGE("enter pc client connect second! \n");
	agent_to_pc_socket= socket(AF_INET, SOCK_STREAM, 0);
	if (agent_to_pc_socket < 0) {
		perror("server");
		ALOGE("opend socket failed!\n");
		exit(0);
	}
	ret=connect(agent_to_pc_socket,(struct sockaddr *)(&pc_client_addr),sizeof(pc_client_addr));
    if (ret<0)
    {
        ALOGE("connect to pc fail!\n ");
        exit(0);
    }
	return ret;
}

int connect_apk_socket(){
	int ret = 0;
	pthread_t apk_thd;
	ret = pthread_create(&apk_thd, NULL, agent_to_apk_thd, NULL);
	if(ret){
		printf("capture screen thread error!\n");
		return -1;
	}
	return ret;
}

void int_to_char(char *dest, int src){
	dest[0] = (char)((src + 4) & 0xff);
	dest[1] = (char)((src + 4) >> 8 & 0xff);
	dest[2] = (char)((src + 4) >> 16 & 0xff);
	dest[3] = (char)((src + 4) >> 24 & 0xff);
	return;
}

/*
Description: capture screen thread
*/
void *screen_to_pc_thd(void *argv){
	int image_size = 1024 * 1024 * 2;
    void const* mapbase = MAP_FAILED;
    ssize_t mapsize = -1;
    void const* base = 0;
    uint32_t w, s, h, f;
    size_t size = 0;
	FILE *out_file = NULL;
	char *read_file = NULL;
	char data_length[4];
	char * all_data_buf = NULL;
	struct timeval start, end;
	char data_type[4];   //for image is 0x06
	int32_t displayId = DEFAULT_DISPLAY_ID;
	bool png = true;
	char outfile[PATH_MAX] = "/data/test/1.jpg";
	data_type[0] = 0x06;  //for image 
	int nSize = 0;
	float timeuse = 0.0f;
	int ret = 0;
	struct sockaddr_in pc_client_addr;  
	pc_client_addr.sin_family = AF_INET;	
	pc_client_addr.sin_addr.s_addr = inet_addr(pc_ipaddr);	 
//	pc_client_addr.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);	
	pc_client_addr.sin_port = htons(AGENT_TO_PC_PORT);	  
	ALOGE("enter pc client connect socket: %d\n", agent_to_pc_socket);     
	agent_to_pc_socket= socket(AF_INET, SOCK_STREAM, 0);
	if (agent_to_pc_socket < 0) {
		//perror("server");
		ALOGE("opend socket failed!\n");
		exit(0);
	}  
	ret=connect(agent_to_pc_socket,(struct sockaddr *)(&pc_client_addr),sizeof(pc_client_addr));
    if (ret<0)
    {
        ALOGE("connect to pc fail!\n ");
        exit(0);
    } 
	//connect_pc_client_socket();
	all_data_buf = (char *)malloc((image_size) * sizeof(char));
	if(!all_data_buf){
		ALOGE("all data buf malloc error\n");
		return 0;
	}
	ALOGE("create pc socket success!\n");
	SkData* streamData = NULL;
    ScreenshotClient screenshot;
	int isConfig = 0;
	while(true){
		if(isPcClientRunning){
			ALOGE("start to capture screentshot.");
		    sp<IBinder> display = SurfaceComposerClient::getBuiltInDisplay(displayId);
		    if ((display != NULL && screenshot.update(display) == NO_ERROR)) {
		        base = screenshot.getPixels();
		        w = screenshot.getWidth();
		        h = screenshot.getHeight();
		        s = screenshot.getStride();
		        f = screenshot.getFormat();
		        size = screenshot.getSize();
		    } else {
		        const char* fbpath = "/dev/graphics/fb0";
		        int fb = open(fbpath, O_RDONLY);
		        if (fb >= 0) {
		            struct fb_var_screeninfo vinfo;
		            if (ioctl(fb, FBIOGET_VSCREENINFO, &vinfo) == 0) {
		                uint32_t bytespp;
		                if (vinfoToPixelFormat(vinfo, &bytespp, &f) == NO_ERROR) {
		                    size_t offset = (vinfo.xoffset + vinfo.yoffset*vinfo.xres) * bytespp;
		                    w = vinfo.xres;
		                    h = vinfo.yres;
		                    s = vinfo.xres;
		                    size = w*h*bytespp;
		                    mapsize = offset + size;
		                    mapbase = mmap(0, mapsize, PROT_READ, MAP_PRIVATE, fb, 0);
		                    if (mapbase != MAP_FAILED) {
		                        base = (void const *)((char const *)mapbase + offset);
		                    }
		                }
		            }
		            close(fb);
		        }
		    }  
		    if (base) {    
		        if (png) {  
					ALOGE("save as bitmap=========\n");
					SkBitmap b;
					gettimeofday(&start, NULL );  
					b.setConfig(flinger2skia(f), w, h, s*bytesPerPixel(f));
		            b.setPixels((void*)base);
		            SkDynamicMemoryWStream stream;
		            SkImageEncoder::EncodeStream(&stream, b, SkImageEncoder::kJPEG_Type, 15);   //20 is ok
		            streamData = stream.copyToData();	
					int_to_char(data_length,  streamData->size());

					//data option is data_length(4) + data_type(4) + data_content(actual len)
					memset(all_data_buf, 0, image_size);
					memcpy(all_data_buf, data_length, 4);
					memcpy(all_data_buf + 4, data_type, 4);
					memcpy(all_data_buf + 8, streamData->data(), streamData->size());
					//printf("image data is: %d\n", streamData->size());
				//	gettimeofday(&start, NULL );  
					int status = ::send(agent_to_pc_socket, all_data_buf, streamData->size() + 8, MSG_NOSIGNAL);
					if(status <= 0){
						ALOGE("[screen_to_pc_thd] socket disconnect!\n");
						close(agent_to_pc_socket);
						sleep(20);
						isPcClientRunning = false;  //network has disconnected, thread should exit right now.
					}  
				//	gettimeofday( &end, NULL );
					//timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec; 
					//timeuse /= 1000000;
					//printf("-----transfer data time use: %f -----\n", timeuse);
					//free(all_data_buf);
					//printf("save as bitmap end\n"); 
				}
				streamData->unref();
	        } else {
	            size_t Bpp = bytesPerPixel(f);
				fwrite(&w, 4, 1, out_file);
				fwrite(&h, 4, 1, out_file);
				fwrite(&f, 4, 1, out_file);
				size_t y = 0;
				for (y=0 ; y<h ; y++) {
				    fwrite(base, w*Bpp, 1, out_file);
				    base = (void *)((char *)base + s*Bpp);
				}
	       }
	   }
	}
    if (mapbase != MAP_FAILED) {
        munmap((void *)mapbase, mapsize);
    }
	//free(all_data_buf);
	ALOGE("[screen_to_pc_thd] ready to exit.\n");
	return 0;
}

int process_is_run(){
	int ret = 0;
	ret = ServerSocket::ServerStatus(PROCESS_LISTEN);
	return ret;
}

/*
Fucntion Description: communicate with apk
*/
int comm_with_apk(){
	int ret = 0;
	connect_apk_socket();
	return ret;
}
  
/*
Fucntion Descrtion: launch monkey service
*/
int launch_monkey(){
	int ret = 0;
	pthread_t monkey_tid;
	isMonkeyRunning = true;   //start launch monkey
	ret = pthread_create(&monkey_tid, NULL, monkey_thd, NULL);
	if(ret){
		ALOGE("monkey thread fail!\n");
		return -1;
	}
	return ret;
}

/*
Function Description: capture screen to pc
*/
int screen_shot_to_pc(){
	int ret = 0;
	pthread_t pc_client_thd;
	isPcClientRunning = true; //capture screen is running 
	ret = pthread_create(&pc_client_thd, NULL, screen_to_pc_thd, NULL);
	if(ret){
		ALOGE("capture screen thread error!\n");
		return -1;
	}
	return ret;
}
 
/*
Function Desciption: mutilIO manager which manage monkey, screen shot, communicate with apk part. 
*/   
int mutilIO_manager(){
	int ret = 0;
	bool showTipsFlag = false;
	launch_monkey();   
	comm_with_apk();
	while(true){
		if(!showTipsFlag){
			ALOGE("Agent wait for apk ip info!\n");
			showTipsFlag = true;
		}
		if(hasIpAddress){
			break;
		}
	}
	//Agent has ip address info and continue next operations
	screen_shot_to_pc();
	return ret;
}   

int main(int argc, char** argv)
{
    const char* pname = argv[0];
	ALOGE("[mutilshotagent] enter main.11111111111111...");
	printf("[mutilshotagent] enter main.11111111111...\n");
    bool png = false;
	hasIpAddress = false;
	isPcClientRunning = false;
	isMonkeyRunning = false;
	if(process_is_run() == SOCKET_BIND_ERR){
		printf("Alpha Agent has running, do not run again!\n");
		ALOGE("Alpha Agent has running, do not run again!\n");
		exit(0);
	}
	mutilIO_manager();
	while(1){
		
	}
    return 0;
}

