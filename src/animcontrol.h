#include "world.h"
#ifndef ANIMCONTROL_INCLUDED
#define ANIMCONTROL_INCLUDED
typedef struct {
	unsigned int* frame_id;
	unsigned int frame_len, frame_duration;
}ANIMATION,*HANIMATION;

extern HANIMATION animations;

#define H_FLIP(flag) (flag&0b1)
#define V_FLIP(flag) ((flag >> 1) & 0b1)
#define LAST(flag) ((flag>>2)& 0b1)
#define SHOULD_ROTATE(flag) ((flag>>3)& 0b1)

typedef struct {
	BOOL flag;
	RECT collider;
	HANIMATION animation;
	unsigned int last_update, current_frame;
}AC,*HAC;

//Animation update; should be called when entity is active
BOOL updateA(HAC ac, int time);

//Animation render; should be called when entity sprite changed
void renderA(HAC ac);

HANIMATION prepareAnimations();
#endif
