
#include "CV/cv.h"
#include "CV/highgui.h"
#include "CV/cxcore.h"

#include "RawToJpg.h"
#include <png.h>
#include "jpeglib.h"
#include <libpng/png.h>

using namespace android;

/*
*@target               raw data switch to jpg, creat a jpg file
*@param  rawData:      the pointer of the raw data
*@param  rawDataWidth: the width of the image raw data
*@param  rawDataHigh:  the high of the image raw data
*@param  mode :        the narrow number of the image
*/

int (*fb_unpack) (unsigned char *pixel, unsigned char *r, unsigned char *g, unsigned char *b);

RawToJpg::RawToJpg(){
}

RawToJpg::~RawToJpg(){
}

unsigned char RawToJpg::rawDaraToJpeg( unsigned char * rawData , int rawDataWidth , int rawDataHigh ,int mode  )
{
	int narrowData = 1;
	if (mode == 2)
	{
		narrowData = 2;
	}
	if (mode == 3)
	{
		narrowData = 4;
	}
	
	int w = rawDataWidth;
	int h = rawDataHigh;

	int wp = w / narrowData; 
	int hp = h / narrowData;

	int FramworkImage = 1;    // framework data

	unsigned char* srcImageRaw ;
	unsigned char* srcImage ;

	srcImage = (unsigned char*)malloc(w*h*4*sizeof(unsigned char));
	//srcImageRaw = rawData +3;
	srcImageRaw = rawData ;
	memcpy(srcImage,srcImageRaw,sizeof(unsigned char)*w*h*4);  //  copy the raw image data
	
	IplImage* imgRaw = cvCreateImage(cvSize(wp,hp),8,3);       //  creat the image 
	unsigned char * pImg = (unsigned char *)imgRaw->imageData;
	int ws = imgRaw->widthStep;

	FramworkImage = *(rawData +2);

	if (FramworkImage == 1 || FramworkImage == 2)
	{
		for (int i  = 0; i < hp; ++i)
		{
			for (int j  = 0; j < wp; ++j)
			{
				pImg[j*3]     = srcImage[i*narrowData*w*4 + j*narrowData*4];
				pImg[j*3 + 1] = srcImage[i*narrowData*w*4 + j*narrowData*4 + 1];
				pImg[j*3 + 2] = srcImage[i*narrowData*w*4 + j*narrowData*4 + 2];

			}
			pImg += ws;
		}
	}
	else
	{
		return;
	}
	//cvSaveImage("screen.jpg",imgRaw);   
	
	//cvReleaseImage(&imgRaw);
    //free(srcImage);
    return 0;

}

static int fb_unpack_rgb565(unsigned char *pixel, unsigned char *r, unsigned char *g, unsigned char *b)
{
	unsigned short color = *(unsigned short *)pixel;

	*r = ((color >> 11) & 0xff) << 3;
	*g = ((color >> 5) & 0xff) << 2;
	*b = (color & 0xff) << 3;

	return 0;
}

static int fb_unpack_rgb24(unsigned char *pixel, unsigned char *r, unsigned char *g, unsigned char *b)
{
	unsigned short color = *(unsigned short *)pixel;

	*r = ((color >> 11) & 0xff) << 3;
	*g = ((color >> 5) & 0xff) >> 3;
	*b = (color & 0xff) << 3;
	return 0;
}

static int fb_unpack_argb32(unsigned char *pixel, unsigned char *r, unsigned char *g, unsigned char *b)
{
	unsigned short color = *(unsigned short *)pixel;

	*r = ((color >> 11) & 0xff) << 3;
	*g = ((color >> 5) & 0xff) >> 3;
	*b = (color & 0xff) << 3;
	return 0;
}

static int fb_unpack_none(unsigned char *pixel, unsigned char *r, unsigned char *g, unsigned char *b)
{
	*r = *g = *b = 0;

	return 0;
}

static void set_pixel_unpacker(int bpp)
{
	if (bpp == 2) {
		fb_unpack = fb_unpack_rgb565;
	} else if (bpp == 3) {
		fb_unpack= fb_unpack_rgb24;
	} else if (bpp == 4) {
		fb_unpack = fb_unpack_argb32;
	} else {
		fb_unpack = fb_unpack_none;
		printf("%s: not supported format.\n");
	}
	return;
}

static int RawToJpg::snap2jpg(char *outdata, int quality, int resize, int width, int height, int *nSize, 
	                  char * base, int bpp)
{
	int row_stride = 0;
	JSAMPROW row_pointer[1] = { 0 };
	struct jpeg_error_mgr jerr;
	struct jpeg_compress_struct cinfo;
	memset(&jerr, 0x00, sizeof(jerr));
	memset(&cinfo, 0x00, sizeof(cinfo));

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outdata, nSize);
	
	printf("fb_width(fb) / resize is: %d\n", width / resize);
	printf("fb_height(fb) / resize is: %d\n", height / resize);
	cinfo.image_width = width / resize;
	cinfo.image_height = height / resize;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);
	set_pixel_unpacker(bpp);
	cinfo.dct_method = JDCT_IFAST;
	cinfo.optimize_coding = FALSE;
	cinfo.do_fancy_downsampling = FALSE;
	jpeg_start_compress(&cinfo, TRUE);
	row_stride = width * 2 / resize;
	JSAMPLE *image_buffer = malloc(3 * width / resize);
	printf("start encoder jpg\n");
	while (cinfo.next_scanline < cinfo.image_height) {
		int i = 0;
		int offset = 0;
		unsigned char *line = base + cinfo.next_scanline *width * bpp * resize;

		for (i = 0; i < width / resize; i++, offset += 3, line += bpp * resize) {
			fb_unpack(line, image_buffer + offset, image_buffer + offset + 1, image_buffer + offset + 2);
		}
		
		row_pointer[0] = base[cinfo.next_scanline * row_stride];
		(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	jpeg_finish_compress(&cinfo);
	
	jpeg_destroy_compress(&cinfo);
	
	return 0;
}

