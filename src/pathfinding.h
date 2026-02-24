#include "map.h"
#ifndef PATHFINDING_INCLUDED
#define PATHFINDING_INCLUDED
typedef struct {
    char avaliable;
    char next;
} PATH, *HPATH;

HPATH find_path(int from_x, int from_y, int to_x, int to_y);
#endif
