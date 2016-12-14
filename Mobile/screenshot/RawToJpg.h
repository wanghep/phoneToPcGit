#ifndef RAWTOJPG_H_class
#define RAWTOJPG_H_class


namespace android {

class RawToJpg {
	
public: 
	RawToJpg();
	virtual ~RawToJpg();
	unsigned char rawDaraToJpeg(unsigned char * rawData , int rawDataWidth , int rawDataHigh ,int mode);
	static int snap2jpg(char *outdata, int quality, int resize, int width, int height, int *nSize, char * base, int bpp);
};
};

#endif
