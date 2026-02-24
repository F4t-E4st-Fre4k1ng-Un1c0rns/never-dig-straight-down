#include "entity.h"

#ifndef SHOVEL_INCLUDED
#define SHOVEL_INCLUDED
#define CREATE_TIME(flag) ((flag>>4)&((1<<28)-1))
typedef ENTITY SHOVEL;
typedef HENTITY HSHOVEL;

void slice(HSHOVEL shovel, int time);

#endif // !SHOVEL_INCLUDED
