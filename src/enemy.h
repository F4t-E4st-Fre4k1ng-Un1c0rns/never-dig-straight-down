#include "entity.h"
#include "player.h"
#include "map.h"
#ifndef ENEMY_INCLUDED
#define ENEMY_INCLUDED

typedef ENTITY ENEMY;
typedef HENTITY HENEMY;

void updateEnemy(HENEMY enemy, int t);
void updateReaper(HENEMY enemy, int t);
void damageEnemy(HENEMY player);
#endif
