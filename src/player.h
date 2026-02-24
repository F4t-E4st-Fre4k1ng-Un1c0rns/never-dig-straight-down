#include "entity.h"

#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED
#define LAST_ATTACK(flag) ((flag>>4)&((1<<28)-1))

typedef ENTITY PLAYER;
typedef HENTITY HPLAYER;

extern HPLAYER player;

void update(HPLAYER player, int time);
void attackByPlayer(HRECT pos);
#endif
