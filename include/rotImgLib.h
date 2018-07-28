#ifndef _ROTIMGLIB_H_
#define _ROTIMGLIB_H_
#include <math.h>
#include "lookupTable.h"
#include "imgFormat.h"

typedef enum ROTSPECIAL_{
	ROT0 = 0,
	ROT90 = 1,
	ROT180 = 2,
	ROT270 = 3,
}ROTSPECIAL;

float modfi(float value, int base);
void calcZoneInfo(int* zoneId, float* zoneAng, float srcAngDeg);
void ang2Skew(float* skewX, float* skewY, float angDeg);
ImgROI calcROIAfterRotation(float rotAng, int srcImgW, int srcImgH);
ImgInfo* rotImgSpecial(ImgInfo *srcImg, ROTSPECIAL rotType);
ImgInfo* shearImgX(float skew, ImgInfo *srcImg);
ImgInfo* shearImgY(float skew, ImgInfo *srcImg);
ImgInfo* rotImg(float rotAng, ImgInfo *srcImg);

#endif