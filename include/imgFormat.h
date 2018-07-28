#ifndef _IMGFORMAT_H_
#define _IMGFORMAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct IMGINFO_{
	int width;
	int widthStep;
	int height;
	int nChannels;
	int imgSize;
	unsigned char* imageData;
}ImgInfo;

typedef struct IMGROI_{
	int startX;
	int startY;
	int width;
	unsigned int height;
}ImgROI;

ImgInfo* createImg(int width, int height, int chs);
ImgInfo* toImgInfo(unsigned char* srcData, int w, int h, int chs);
unsigned char* toUchar(ImgInfo* img);
ImgInfo* cropImageROI(ImgInfo* srcImg, ImgROI roi);
void freeImgInfo(ImgInfo* img);
#endif