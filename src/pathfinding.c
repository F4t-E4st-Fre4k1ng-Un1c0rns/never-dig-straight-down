#include <stdlib.h>
#include "map.h"
#include "tic80.h"
#include "pathfinding.h"

int bfs_directions_x[4] = {0, 0, 1, -1};
int bfs_directions_y[4] = {1, -1, 0, 0};


HPATH find_path(int from_x, int from_y, int to_x, int to_y) {
    int width = worldMap->width;
    int height = worldMap->height;
    GTILES* map = worldMap->tiles;
    int* directions = malloc(sizeof(int) * width * height);
    if (!directions) {
        trace("Broken memory", 2);
        return 0;
    }
    for (int i = 0; i < (width * height); ++i) {
        directions[i] = -1;
    }
    int* queue = malloc(sizeof(int) * width * height);
    if (!queue) {
        trace("Broken memory", 2);
        return 0;
    }
    int end_pos = 1, start_pos = 0;
    int start = width * from_y + from_x;
    directions[start] = -2;
    queue[0] = start;
    int limit = width * height;
    int goal = width * to_y + to_x;

    while (start_pos < end_pos && queue[start_pos] != goal) {
        int p = queue[start_pos];
        for (int i = 0; i < 4; ++i) {
            if (p % width == 0 && bfs_directions_y[i] == 1) {
              continue;
            }
            if ((p + 1) % width == 0 && bfs_directions_y[i] == -1) {
              continue;
            }

            int changed_pos = p - (bfs_directions_x[i] * width) - bfs_directions_y[i];
            if (0 <= changed_pos &&
                changed_pos < limit && 
                directions[changed_pos] == -1 &&
                map[changed_pos] != WALL) {
                queue[end_pos] = changed_pos;
                directions[changed_pos] = p;
                end_pos++;
            }
        }
        start_pos++;
    }

    HPATH path = malloc(sizeof(PATH));
    if (directions[goal] < 0) {
      path->avaliable = 0;
      free(directions);
      free(queue);
      return path;
    }
    path->avaliable = 1;
    int pos = goal;
    int prev_pos = pos;
    while (pos != start) {
        prev_pos = pos;
        pos = directions[pos];
    }
    path->next = prev_pos;
    free(directions);
    free(queue);

    return path;
} 

