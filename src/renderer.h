#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED
#include "world.h"

void renderTile(POS pos, POS size, int sprite, BOOL transp);

void renderDefaultTile(POS pos, int sprite, BOOL transparent);

void renderSprite(HRECT collider, int sprite, BOOL flag);

void renderCollider(HRECT collider);

#endif
