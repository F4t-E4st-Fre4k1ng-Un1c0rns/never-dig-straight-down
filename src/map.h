#include "world.h"
#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED
typedef enum
{
	WALL = 0,
	FLOOR = 1,
	CHEST = 2,
	SPIKES,
	LAVA
}GTILES;

typedef struct {
	GTILES* tiles;
	unsigned short width, height;
  short chest_position;
}GMAP, * HMAP;

extern HMAP worldMap;

//Map generating
HMAP createMap(unsigned short width, unsigned short height);

//Map rendering
void renderMap(HMAP map);

void regenerateFreeMap(HMAP map);

void regenerateMap(HMAP map, int seed);

void regenerateSmartMap(HMAP map,int seed, float f);

void renderScore();

BOOL doesPointObstructed(POS position);

BOOL doesMapPointObstructed(POS position);

void mineBlock(POS pos);

#endif
