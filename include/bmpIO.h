#ifndef _BMPIO_H_
#define _BMPIO_H_
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push)
#pragma pack(1)
typedef struct BMPHEADER_{
	// file header
	unsigned short identifier;      // 0x0000
	unsigned int filesize;          // 0x0002
	unsigned int reserved;          // 0x0006
	unsigned int bitmap_dataoffset; // 0x000A
	// info header
	unsigned int bitmap_headersize; // 0x000E
	unsigned int width;             // 0x0012
	unsigned int height;            // 0x0016
	unsigned short planes;          // 0x001A
	unsigned short bits_perpixel;   // 0x001C
	unsigned int compression;       // 0x001E
	unsigned int bitmap_datasize;   // 0x0022
	unsigned int hresolution;       // 0x0026
	unsigned int vresolution;       // 0x002A
	unsigned int usedcolors;        // 0x002E
	unsigned int importantcolors;   // 0x0032
	unsigned int palette;           // 0x0036
}BmpHeader;
#pragma pack(pop)

int readBmpHeader(char* fileName, BmpHeader* hbmp);
int readBmpRaw(char* fileName, BmpHeader* hbmp, unsigned char* buffer);
int writeBmp(char* filename, BmpHeader* hbmp, unsigned char* buffer);

#endif