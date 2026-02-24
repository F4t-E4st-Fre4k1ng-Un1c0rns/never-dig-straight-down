#include "map.h"
#include <stdlib.h>
#include "perlins.h"
#include <math.h>
#include "renderer.h"
#include "resources.h"
#include "world.h"
#include "game.h"

void regenerateFreeMap(HMAP map) {
    for (int i = 0; i < map->width* map->height; i++) {
        map->tiles[i] = FLOOR;
    }
}

void regenerateSmartMap(HMAP map, int seed, float f) {
    HPERLIN  perlin = generatePerlin(seed);
    for (unsigned short i = 0; i < map->height; ++i) {
        for (unsigned short j = 0; j < map->width; ++j) {
            double y = (double)j / ((double)map->height);
            double x = (double)i / ((double)map->width);

            double n = 7 * noise(perlin, (map->width + 1) * x, (map->height + 1) * y, 0);
            map->tiles[i * map->width + j] = (n - floor(n)) < f ? WALL : FLOOR;
        }
    }
    free(perlin->weigths);
    free(perlin);
}

void regenerateMap(HMAP map, int seed) {
    regenerateSmartMap(map, seed, 0.5f);
    while (1) {
        int i = rand() % (map->width * map->height);
        if (map->tiles[i] == FLOOR) {
            map->tiles[i] = CHEST;
            map->chest_position = i;
            break;
        }
    }
}

HMAP createMap(unsigned short width, unsigned short height)
{
	HMAP map = (HMAP)malloc(sizeof(GMAP));
    *map = (GMAP){ malloc(width * height * sizeof(GTILES)), width, height };
    return map;
}

void renderMap(HMAP map) {
    for (int i = 0; i < map->width; ++i) {
        for (int j = 0; j < map->height; ++j) {
            POS pos = (POS){ TILE_SIZE * i, TILE_SIZE * j };
            switch (map->tiles[j * map->width + i]) {
            case WALL:
                if (i > 0 && j > 0 && map->tiles[(j - 1) * map->width + i - 1] != WALL) {
                    renderTile(pos, (POS) { 1, 1 }, WALL_CORNER_SPRITE, 0b0);
                }
                if (i < map->width-1 && j > 0 && map->tiles[(j - 1) * map->width + i + 1] != WALL) {
                    renderTile((POS) { pos.x+TILE_SIZE/2,pos.y }, (POS) { 1, 1 }, WALL_CORNER_SPRITE, 0b10);
                }
                if (i < map->width - 1 && j < map->height - 1 && map->tiles[(j + 1) * map->width + i + 1] != WALL) {
                    renderTile((POS) { pos.x + TILE_SIZE/2,pos.y + TILE_SIZE / 2 }, (POS) { 1, 1 }, WALL_CORNER_SPRITE, 0b100);
                }
                if (i > 0 && j < map->height - 1 && map->tiles[(j + 1) * map->width + i - 1] != WALL) {
                    renderTile((POS) { pos.x,pos.y + TILE_SIZE / 2 }, (POS) { 1, 1 }, WALL_CORNER_SPRITE, 0b110);
                }
                if (i > 0 && map->tiles[j * map->width + i - 1] != WALL) {
                    renderTile(pos, (POS) {2,1}, WALL_SPRITE, 0b010);
                }
                if (i < map->width - 1 && map->tiles[j * map->width + i + 1] != WALL) {
                    renderTile((POS){pos.x+TILE_SIZE/2, pos.y}, (POS) { 2, 1 }, WALL_SPRITE, 0b110);
                }
                if (j > 0 && map->tiles[(j-1) * map->width + i] != WALL) {
                    renderTile(pos, (POS) { 2, 1 }, WALL_SPRITE, 0b100);
                }
                if (j < map->height - 1 && map->tiles[(j+1) * map->width + i] != WALL) {
                    renderTile((POS) { pos.x , pos.y + TILE_SIZE / 2}, (POS) { 2, 1 }, WALL_SPRITE, 0b0);
                }

                break;
            case FLOOR:
                renderDefaultTile(pos, FLOOR_SPRITE, 1);
                break;
            case CHEST:
                renderDefaultTile(pos, FLOOR_SPRITE, 1);
                renderDefaultTile(pos, CHEST_SPRITE, 0);
                break;
            default:
                break;
            }
        }
    }
}

void renderScore() {
    char scoreBuf[4];
    snprintf(scoreBuf, 4, "%d", worldState->score);
    print(scoreBuf, 10, 10, 14, 0, worldState->scoreSize, 0);
}

BOOL doesMapPointObstructed(POS pos) {
    return worldMap->tiles[pos.y * worldMap->width + pos.x] == WALL;
}

BOOL doesPointObstructed(POS pos) {
    return doesMapPointObstructed((POS) { pos.x / TILE_SIZE, pos.y / TILE_SIZE });
}

void mineBlock(POS pos) {
    if (pos.x < 0 || pos.y < 0 || pos.x >= worldMap->width || pos.y >= worldMap->height) return;
    if (worldMap->tiles[pos.y * worldMap->width + pos.x] == CHEST) {
        worldState->score += 5;
        worldState->scoreSize = log(worldState->score+2);
        if (worldState->score >= 64) {
            worldState->score = -64;
        }
        worldMap->tiles[(pos.y) * worldMap->width + pos.x] = FLOOR;
        return;
    }

    if (!doesMapPointObstructed(pos)) return;
    worldMap->tiles[(pos.y) * worldMap->width + pos.x] = FLOOR;
}

