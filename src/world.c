#include "world.h"


BOOL doesContain(POS point, HRECT rect) {
	return point.x >= rect->x &&
		point.y >= rect->y &&
		point.x <= rect->x + rect->width &&
		point.y <= rect->y + rect->height;
}

BOOL haveCommon(HRECT first, HRECT second)
{
	return doesContain((POS) { first->x, first->y }, second) ||
		doesContain((POS) { first->x + first->width, first->y }, second) ||
		doesContain((POS) { first->x, first->y + first->height }, second) ||
		doesContain((POS) { first->x + first->width, first->y + first->height }, second) ||
		doesContain((POS) { first->x + first->width/2, first->y + first->height/2 }, second) ||
		doesContain((POS) { second->x, second->y }, first) ||
		doesContain((POS) { second->x + second->width, second->y }, first) ||
		doesContain((POS) { second->x, second->y + second->height }, first) ||
		doesContain((POS) { second->x + second->width, second->y + second->height }, first) ||
		doesContain((POS) { second->x + second->width/2, second->y + second->height/2 }, first);
}
