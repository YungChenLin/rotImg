#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "bmpIO.h"
#include "rotImgLib.h"
#include "lookupTable.h"

int main(){
	char inFileName[128] = "test.bmp";
	// read bmp header
	BmpHeader hbmp;
	if ( readBmpHeader(inFileName, &hbmp)){
		printf("img height %d \n", hbmp.height);
		printf("img width  %d \n", hbmp.width);
		printf("planes  %d \n", hbmp.planes);
		printf("bmp header size %d\n", hbmp.bitmap_headersize);
		printf("pixel in bits %d \n", hbmp.bits_perpixel);
		printf("compression %d \n", hbmp.compression);
		printf("fileSize %d\n", hbmp.filesize);
		printf("data_offset %d\n", hbmp.bitmap_dataoffset);
		printf("size of bmp header %d\n", sizeof(BmpHeader));
		printf("hresolution %d\n", hbmp.hresolution);
		printf("vresolution %d\n", hbmp.vresolution);
		printf("w*h*d %d\n", hbmp.width*hbmp.height * 3);
	}else{
		printf("readbmp: file is not found or is opened with error\n");
		printf("press any key to exit\n");
		getchar();
		return 0;
	}
	// read bmp raw data
	int w = hbmp.width;
	int h = hbmp.height;
	int chs = hbmp.bits_perpixel >> 3;
	int widthStep = ((w*chs) % 4 == 0) ? w*chs : (1 + (w*chs)/4)*4;
	unsigned int imgSize = widthStep*h;
	unsigned char* buffer = (unsigned char*)calloc(imgSize, sizeof(unsigned char));
	readBmpRaw(inFileName, &hbmp, buffer);

	char outFileTitle[80] = "result";
	char format[10] = ".bmp";
	for (int angId = 0; angId < 360; angId+=90){
		float rotAng = (float)angId;
		// your code here
		ImgInfo* srcCopy = toImgInfo(buffer, w, h, chs);
		ImgInfo* dstImg = rotImg(rotAng, srcCopy);
		// transform back to bmp
		hbmp.height = dstImg->height;
		hbmp.width = dstImg->width;
		hbmp.filesize = hbmp.bitmap_dataoffset + dstImg->imgSize;
		// create output file name
		char outFileName[128];
		strcpy(outFileName, outFileTitle);
		char id[30];
		sprintf(id, "%d", angId);
		strcat(outFileName, id);
		strcat(outFileName, format);
		printf("fileName %s\n", outFileName);
		// write image
		writeBmp(outFileName, &hbmp, dstImg->imageData);
		freeImgInfo(srcCopy);
		freeImgInfo(dstImg);
	}
	free(buffer);
	printf("press any key to exit\n");
	getchar();
	return 0;
}