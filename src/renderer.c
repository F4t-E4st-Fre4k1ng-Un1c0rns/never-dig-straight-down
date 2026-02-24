#include "renderer.h"
#include "tic80.h"
#include "entity.h"
static int background = 0;
static int transparent = -1;

#define TRANSPARENT(flag) (flag&0b1)
#define ROTATE(flag) ((flag>>1)&0b11)

void renderDefaultTile(POS pos, int sprite, BOOL transparent) {
	renderTile(pos, (POS) { 2, 2 }, sprite, TRANSPARENT(transparent));
}

void renderTile(POS pos, POS size, int sprite, BOOL flag) {
	spr(sprite, pos.x, pos.y, TRANSPARENT(flag)? & transparent : &background, 1, 1, 0, ROTATE(flag), size.x, size.y);
}

void renderSprite(HRECT collider, int sprite, BOOL flip)
{
	spr(sprite, collider->x, collider->y, &background, 1, 1, SHOULD_ROTATE(flip)?(LAST(flip)? (!V_FLIP(flip)):H_FLIP(flip)):H_FLIP(flip),(SHOULD_ROTATE(flip))*(LAST(flip)) * (45 + 90 * V_FLIP(flip)), collider->width / TILE_SIZE, collider->height / TILE_SIZE);
}

void renderCollider(HRECT collider)
{
	rectb(collider->x, collider->y, collider->width, collider->height, 14);
}