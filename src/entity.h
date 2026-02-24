#include "world.h"
#include "animcontrol.h"
#ifndef ENTITY_INCLUDED
#define ENTITY_INCLUDED

#define TYPE(flag) ((flag>>1)&0b111)
#define ACTIVE(flag) (flag&0b1)



typedef enum {
	TPLAYER,
	TSHOVEL,
	TRAT,
	TSPIDER_1,
	TSPIDER_2,
	TREAPER,
	TSIEVE,
}ENEMY_TYPE;

typedef struct {
	RECT collider;
	AC controller;
	BOOL flag;
} ENTITY, *HENTITY;

extern HENTITY entities;
extern int enti_size, enti_gap;

//entity render
void renderE(HENTITY entity, int time);

//entity movement
void move(HENTITY entity, POS dir);

//damage entity
void damage(HENTITY entity);
#endif
