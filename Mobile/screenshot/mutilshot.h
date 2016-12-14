#ifndef MUTILSHOT_H
#define MUTILSHOT_H

enum{
	AGENT_TO_PC_PORT = 35832,
	APK_TO_AGENT = 36669, 
	PROCESS_LISTEN
};  
int screen_shot_to_pc();
int connect_pc_client_socket();

#define LOCAL_IP_ADDRESS   "10.106.9.171"  // ji cheng huan jing 
#define AGENT_APK_ADDRESS  "127.0.0.1"

#define PROC_DIRECTORY "/proc/"
#define BUF_SIZE 1024
#define MONKEY_PROCESS "com.android.commands.monkey"
#define ALPHAGENT_PROCESS "./alphagent"

#define APK_CMD_IP  0x11

#endif

