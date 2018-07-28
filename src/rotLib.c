#include <stdio.h>
#include <math.h>
#include "rotImgLib.h"
#include "lookupTable.h"

float modfi(float value, int base){
	float valabs = (value < 0) ? (float)fabs(value) : value;
	float valres = valabs - (int)valabs;
	float val = (float)((int)valabs % base);
	return (value < 0) ? -(val + valres) : (val + valres);
}

void calcZoneInfo(int* zoneId, float* zoneAng, float srcAngDeg){
	float ang = modfi(srcAngDeg, 360);
	float angIn360 = ang > 0 ? ang : (ang == 0) ? 0 : 360 + ang;
	*zoneId = (int)(angIn360 / 90);
	*zoneAng = angIn360 - (float)(*zoneId) * 90;
	printf(" ang %f zoneId %d zoneAng %f\n", ang, *zoneId, *zoneAng);
}

ImgInfo* rotImgSpecial(ImgInfo *srcImg, ROTSPECIAL rotType){
	int w = srcImg->width;
	int h = srcImg->height;
	int numCH = srcImg->nChannels;
	int ws = srcImg->widthStep;
	int wd = 0;
	ImgInfo* dstImg;
	if (rotType == ROT0 || rotType == ROT180){
		dstImg = createImg(w, h, numCH);
	}else{
		dstImg = createImg(h, w, numCH);
	}
	wd = dstImg->widthStep;
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			int x = 0, y = 0;
			if (rotType == ROT0){
				x = i;
				y = j;
			}else if (rotType == ROT270){
				x = j;
				y = w - 1 - i;
			}else if (rotType == ROT180){
				x = w - 1 - i;
				y = h - 1 - j;
			}else if (rotType == ROT90){
				x = h - 1 - j;
				y = i;
			}
			for (int ch = 0; ch < numCH; ch++){
				dstImg->imageData[wd*y + numCH*x + ch] = srcImg->imageData[ws*j + numCH*i + ch];
			}
		}
	}
	return dstImg;
}

void ang2Skew(float* skewX, float* skewY, float angDeg){
	float tanVal = tanTbl(angDeg / 2);
	if (tanVal >= 0){
		*skewX = -tanVal;
	}
	else{
		*skewX = 0;
	}

	float sinVal = sinTbl(angDeg);
	if (sinVal >= 0){
		*skewY = sinVal;
	}
	else{
		*skewY = 0;
		printf("look up tabel fail\n");
	}
}

ImgInfo * shearImgX(float skew, ImgInfo *srcImg){
	int wsSrc = srcImg->widthStep;
	int w = srcImg->width;
	int h = srcImg->height;
	int numCH = srcImg->nChannels;
	int n = (int)((float)h*fabs(skew) + 0.5f);
	ImgInfo* dstImg = createImg(w + n, h, numCH);
	int wsDst = dstImg->widthStep;
	for (int i = 0; i < w; i++){
		float skewCount = 0;
		for (int j = 0; j < h; j++){
			int shift = (int)skewCount;
			int x = (skew > 0) ? i + shift : i + n - shift;
			for (int ch = 0; ch < numCH; ch++){
				dstImg->imageData[wsDst*j + numCH* x + ch] = srcImg->imageData[wsSrc*j + numCH* i + ch];
			}
			skewCount += (float)fabs(skew);
		}
	}
	freeImgInfo(srcImg);
	return dstImg;
}

ImgInfo* shearImgY(float skew, ImgInfo *srcImg){
	int wsSrc = srcImg->widthStep;
	int w = srcImg->width;
	int h = srcImg->height;
	int numCH = srcImg->nChannels;
	int n = (int)((float)w*fabs(skew) + 0.5f);
	ImgInfo* dstImg = createImg(w, h + n, numCH);
	int wsDst = dstImg->widthStep;
	for (int j = 0; j < h; j++)
	{
		float skewCount = 0;
		for (int i = 0; i < w; i++)
		{
			int shift = (int)skewCount;
			int y = (skew > 0) ? j + shift : j + n - shift;
			for (int ch = 0; ch < numCH; ch++)
			{
				dstImg->imageData[wsDst*y + numCH * i + ch] = srcImg->imageData[wsSrc*j + numCH * i + ch];
			}
			skewCount += (float)fabs(skew);
		}
	}
	freeImgInfo(srcImg);
	return dstImg;
}

ImgROI calcROIAfterRotation(float rotAng, ImgInfo*src, ImgInfo*dst){
	float arc = 3.1415926f*rotAng / 180.0f;
	float w = (float)src->width;
	float h = (float)src->height;
	float c = fabs(cosf(arc)), s = fabs(sinf(arc));
	int wNew = (int)(w*c + s*h);
	int hNew = (int)(w*s + c*h);
	ImgROI roi;
	roi.startX = dst->width/2 - wNew/2;
	roi.startY = dst->height/2 - hNew/2;
	roi.width = wNew;
	roi.height = hNew;
	return roi;
}

ImgInfo* rotImg(float rotAng, ImgInfo *srcImg){
	int zoneId = 0;
	float zoneAng = 0;
	calcZoneInfo(&zoneId, &zoneAng, rotAng);
	ImgInfo* dstImg = rotImgSpecial(srcImg, zoneId);
	float skewX = 0, skewY = 0;
	ang2Skew(&skewX, &skewY, zoneAng);
	if (0 != zoneAng){
		// shear to replace rotation
		dstImg = shearImgX(skewX, dstImg);
		dstImg = shearImgY(skewY, dstImg);
		dstImg = shearImgX(skewX, dstImg);
	}
	// set roi & crop image
	ImgROI roi = calcROIAfterRotation(rotAng, srcImg, dstImg);
	return cropImageROI(dstImg, roi);
}