#include <stdio.h>
#include <stdlib.h>
#include "bmpIO.h"

int readBmpHeader(char* fileName, BmpHeader* hbmp){
	FILE* file;
	unsigned char* ptr;
	// read with binary mode
	file = fopen(fileName, "rb");
	if (file == NULL){
		return 0;
	}
	ptr = (unsigned char*)(void*)hbmp;
	// read the header 
	fread(ptr, sizeof(unsigned char), sizeof(BmpHeader), file);
	printf("%d\n", fclose(file));
	return 1;
}

int readBmpRaw(char* fileName, BmpHeader* hbmp, unsigned char* buffer){
	FILE* file;
	// read with binary mode
	file = fopen(fileName, "rb");
	if (file == NULL){
		printf("readbmp: file open error\n");
		return -1;
	}
	// read raw data to buffer
	int w = hbmp->width;
	int h = hbmp->height;
	int chs = hbmp->bits_perpixel >> 3;
	int widthStep = ((w*chs) % 4 == 0) ? w*chs : (1 + (w*chs) / 4) * 4;
	unsigned int imgSize = widthStep*h;
	// move file ptr to raw data start
	rewind(file);
	//fseek(file, sizeof(BmpHeader), SEEK_SET);
	fseek(file, hbmp->bitmap_dataoffset, SEEK_SET);
	fread(buffer, sizeof(unsigned char), imgSize, file);
	fclose(file);
	return 1;
}

int writeBmp(char* filename, BmpHeader* hbmp, unsigned char* buffer)
{
	FILE* ofp;
	unsigned char* ptr;
	ofp = fopen(filename, "wb");
	if (ofp == NULL){
		printf("writebmp: file open error\n");
		return -1;
	}
	ptr = (unsigned char*)(void*)hbmp;
	// write header
	fwrite(ptr, sizeof(unsigned char), sizeof(BmpHeader), ofp);
	// reset file pointer
	rewind(ofp);
	fseek(ofp, hbmp->bitmap_dataoffset, SEEK_SET);
	// write raw data
	int w = hbmp->width;
	int h = hbmp->height;
	int chs = hbmp->bits_perpixel >> 3;
	int widthStep = ((w*chs) % 4 == 0) ? w*chs : (1 + (w*chs) / 4) * 4;
	unsigned int imgSize = widthStep*h;
	fwrite(buffer, sizeof(unsigned char), imgSize, ofp);
	fclose(ofp);
	return 1;
}
