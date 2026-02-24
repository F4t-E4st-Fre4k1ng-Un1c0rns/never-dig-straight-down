#include <math.h>
#include "entity.h"
#include "enemy.h"
#include "generator.h"
#include "map.h"
#include "player.h"
#include <stdlib.h>
#include "game.h"
#include "colors.h"
#include "shuffle.h"
#include "shovel.h"
short generateRandomCoordinates(int16_t player) {
    short coords = rand() % SCREEN_TILES_SQUARE;
    while ((worldMap->tiles[coords] != FLOOR) || (coords == player)) {
        coords = (coords + 1) % SCREEN_TILES_SQUARE;
    }
    return coords;
}

void preloadPlayer() {
    entities = malloc(2 * sizeof(ENTITY));
    enti_size = 2;
    enti_gap = 2;

    player = &entities[0];
    *player = (PLAYER){
        .collider = (RECT) {
            .x = WIDTH/2 - TILE_SIZE / 2,
            .y = HEIGHT/2-TILE_SIZE/2,
            .width = 8,
            .height = 8
        },
        .controller = (AC) {
             .flag = 0b1000 ,
             (RECT) {
                .x = 0,
                .y = 0,
                .width = 2 * TILE_SIZE,
                .height = 2 * TILE_SIZE
            },
            & animations[0] , 0, 0
        },
        .flag = 0b1 | (TPLAYER << 1)
    };

    HSHOVEL shovel = &entities[1];
    shovel->controller = (AC){
        .flag = 0b1000,
        .animation = &animations[2]
    };
    shovel->flag = 0b0 | (TSHOVEL << 1);
}

void generateTutorial(int t) {
    setPalette(0);
    regenerateFreeMap(worldMap);
}

void generateDefault(int t) {
    if (worldState->level <= 7) {
        setPalette(worldState->level - 1);
    }
    else {
        setPalette(rand() % 7);
    }
    regenerateMap(worldMap, rand());

    for (int i = player->collider.x / TILE_SIZE; i <= (player->collider.x + player->collider.width) / TILE_SIZE; i++) {
        for (int j = player->collider.y / TILE_SIZE; j <= (player->collider.y + player->collider.height) / TILE_SIZE; j++) {
            if(doesMapPointObstructed((POS){i,j})) worldMap->tiles[j*worldMap->width+i] = FLOOR;
        }
    }

    int* floors = malloc(worldMap->width * worldMap->height * sizeof(int));
    short floor_size = 0;
    for (int i = 0; i < worldMap->width * worldMap->height; i++) {
        if(worldMap->tiles[i] == FLOOR)
            floors[floor_size++] = i;
    }
    shuffle(floors, floor_size, rand());
    int8_t spiders_num = min(worldState->level - 1, 3+rand() % 4);
    int8_t rats_num = (worldState->level > 3) * (rand() % 3);
    enti_size = 3 + spiders_num + rats_num;
    if (enti_size > enti_gap) {
        enti_gap = enti_size;
        entities = realloc(entities, enti_gap * sizeof(ENTITY));
        player = &entities[0];
    }
    int8_t enti_pointer = 2;
    for (int i = 0; i < spiders_num; i++) {
        int coord = floors[--floor_size];
        int start_coords_x = (coord % worldMap->width) * TILE_SIZE;
        int start_coords_y = (coord / worldMap->width) * TILE_SIZE;

        HENTITY spider = &entities[enti_pointer++];
        *spider = (ENEMY){
            .controller = (AC){
                .flag = 0b1000,
                .collider = (RECT) {
                    .x = start_coords_x,
                    .y = start_coords_y,
                    .width = TILE_SIZE * 2,
                    .height = TILE_SIZE * 2
                },
                .animation = &animations[3]
            },
            .collider = (RECT){
                    .x = start_coords_x + 4,
                    .y = start_coords_y + 4,
                    .width = TILE_SIZE / 2,
                    .height = TILE_SIZE / 2
            },
            .flag = 0b1 | (TSPIDER_1 << 1) | (start_coords_y * WIDTH_TILES + start_coords_x) << 4
        };
    }
    for (int i = 0; i < rats_num; i++) {
        HENTITY rat = &entities[enti_pointer++];
        int coord = floors[--floor_size];
        int start_coords_x = (coord % worldMap->width) * TILE_SIZE;
        int start_coords_y = (coord / worldMap->width) * TILE_SIZE;
        *rat = (ENEMY){
            .controller = (AC){
                .flag = 0b1000,
                .collider = (RECT) {
                    .x = start_coords_x,
                    .y = start_coords_y,
                    .width = TILE_SIZE * 2 + 2,
                    .height = TILE_SIZE + 6
                },
                .animation = &animations[1]
            },
            .collider = (RECT){
                    .x = start_coords_x + 2,
                    .y = start_coords_y + 6,
                    .width = TILE_SIZE / 2,
                    .height = TILE_SIZE / 2
            },
            .flag = 0b1 | (TRAT << 1) | (start_coords_y * WIDTH_TILES + start_coords_x) << 4
        };
    }
    free(floors);
    HENEMY reaper = &entities[enti_pointer++];
    reaper->controller = (AC){
            .flag = 0,
            .collider = (RECT) {
                .x = -(TILE_SIZE * 2),
                .y = 48,
                .width = TILE_SIZE * 4,
                .height = TILE_SIZE * 4
            },
            .animation = &animations[4]
    };
    reaper->collider = (RECT){
            .x = -(TILE_SIZE * 2),
            .y = 48,
            .width = TILE_SIZE,
            .height = TILE_SIZE
    };
    reaper->flag = (TREAPER << 1);
}

void generate(int t) {
    if (!entities) preloadPlayer();
    player->controller.animation = &animations[0];
    player->flag = (player->flag & 0b1111) | (t << 4);
    srand(t);
    worldState->level++;
    if (worldState->level == 1) {
        generateTutorial(t);
    }
    else {
        generateDefault(t);
    }
    player_finishing = 0;
}
