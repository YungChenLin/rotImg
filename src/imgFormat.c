#include "imgFormat.h"


ImgInfo* createImg( int w,  int h, int chs){
	ImgInfo* obj = (ImgInfo*)calloc(1,sizeof(ImgInfo));
	obj->nChannels = chs;
	obj->width = w;
	obj->widthStep = ((w*chs) % 4 == 0) ? w*chs : (1 + (w*chs) / 4) * 4;
	obj->height = h;
	obj->imgSize = obj->widthStep*obj->height;
	obj->imageData = (unsigned char*)calloc(obj->imgSize, sizeof(unsigned char));
	return obj;
}

ImgInfo* toImgInfo(unsigned char* srcData,int w, int h, int chs){
	ImgInfo* obj = createImg(w, h, chs);
	memcpy(obj->imageData, srcData, obj->imgSize*sizeof(unsigned char));
	return obj;
}

unsigned char* toUchar(ImgInfo* img){
	unsigned char* dst= (unsigned char*)calloc(img->imgSize, sizeof(unsigned char));
	memcpy(dst, img->imageData, img->imgSize*sizeof(unsigned char));
	return dst;
}

ImgInfo* cropImageROI(ImgInfo* srcImg, ImgROI roi){
	int chs = srcImg->nChannels;
	ImgInfo* cropImg = createImg(roi.width, roi.height, chs);
	int wsSrc = srcImg->widthStep;
	int wsDst = cropImg->widthStep;
	for ( int j = 0; j < cropImg->height; j++){
		int x = roi.startX;
		int y = roi.startY + j;
		memcpy(&cropImg->imageData[wsDst*j], &srcImg->imageData[wsSrc*y + chs*x ], sizeof(unsigned char)*wsDst);
	}
	return cropImg;
}

void freeImgInfo(ImgInfo* img){
	free(img->imageData);
	free(img);
}

