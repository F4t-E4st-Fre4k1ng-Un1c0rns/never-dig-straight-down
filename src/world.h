#include "tic80.h"

#ifndef WORLD_INCLUDED
#define WORLD_INCLUDED

#define BOOL int
#define max(a, b) (a > b) ? a : b
#define min(a, b) (a < b) ? a : b

typedef struct {
	int x, y;
}POS,*HPOS;


typedef struct {
	int x, y;
	int width, height;
}RECT,*HRECT;

//Return if rectange contain point
BOOL doesContain(POS point, HRECT rect);

//Return if rectangles have common internal points
BOOL haveCommon(HRECT first, HRECT second);
#endif

