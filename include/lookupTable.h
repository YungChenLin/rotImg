#ifndef _LOOKUPTABLE_H_
#define _LOOKUPTABLE_H_

#include <stdlib.h>
#include <math.h>

int checkInTblAngRng(float start, float end, float value);
float tanTbl(float angDeg);
float sinTbl(float angDeg);
float cosTbl(float angDeg);

#endif