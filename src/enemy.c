#include "enemy.h"
#include "player.h"
#include "map.h"
#include "pathfinding.h"
#include "tic80.h"
#include <stdlib.h>
#include "game.h"
#include "world.h"
#include "animcontrol.h"
#include "renderer.h"

short getGoal(int flag) {
    switch ((flag >> 1) & 0b111) {
        case TSPIDER_1:
            return ((player->collider.x / TILE_SIZE) << 8) + (player->collider.y / TILE_SIZE);
        case TRAT:
            if (worldMap->chest_position == -1) return -1;
            return ((worldMap->chest_position % (WIDTH_TILES)) << 8) + (worldMap->chest_position / (WIDTH_TILES));;
        default:
            return -1;
    }
}

uint8_t attackByEnemy(HENEMY enemy) {
    switch (((enemy->flag) >> 1) & 0b111) {
        case TSPIDER_1:
        case TREAPER:
            if (haveCommon(&enemy->collider, &player->collider)) {
                damage(player);
                return 1;
            }
            break;
        case TRAT:
            uint16_t enemy_position_tiles_x = enemy->collider.x / (TILE_SIZE);
            uint16_t enemy_position_tiles_y = enemy->collider.y / (TILE_SIZE);
            if (enemy_position_tiles_y * (WIDTH_TILES) + enemy_position_tiles_x == worldMap->chest_position) {
                worldMap->tiles[worldMap->chest_position] = FLOOR;
                worldMap->chest_position = -1;
                return 1;
            }
            break;
        default:
            break;
    }
    return 0;
}

void updateReaper(HENEMY enemy, int t) {
    if (worldState->timeOnLevel > 300) {
        enemy->flag |= 1;
    }
    if (worldState->score >= 42) {
        enemy->controller.animation = &animations[5];
    }
    if (!ACTIVE(enemy->flag)) return;
    if (t % 3 !=  0) {
        return;
    }
    if (enemy->collider.x <= 0) {
        move(enemy, (POS){1, 0});
        return;
    }
    int16_t dx = enemy->collider.x - player->collider.x;
    int16_t dy = enemy->collider.y - player->collider.y;
        attackByEnemy(enemy);
    if (dx == 0 && dy == 0) {
    }
    if (dx < 0) {
        move(enemy, (POS){1, 0});
    }
    if (dx > 0) {
        move(enemy, (POS){-1, 0});
    }
    if (dy < 0) {
        move(enemy, (POS){0, 1});
    }
    if (dy > 0) {
        move(enemy, (POS){0, -1});
    }
}


void updateEnemy(HENEMY enemy, int t) { 
    if (worldState->timeOnLevel < 120) {
        return;
    }
    if (!ACTIVE(enemy->flag)) return;
    uint8_t enemy_position_x = enemy->collider.x;
    uint8_t enemy_position_y = enemy->collider.y;
    int next_pos = (int)((enemy->flag >> 4) & 0xffff);
    uint8_t next_pos_tiles_x = (next_pos % (WIDTH_TILES));
    uint8_t next_pos_tiles_y = (next_pos / (WIDTH_TILES));
    uint8_t next_pos_x = next_pos_tiles_x * (TILE_SIZE) + 2;
    uint8_t next_pos_y = next_pos_tiles_y * (TILE_SIZE) + 2;
    if (t % 2 != 0) {
        return;
    }
    if (attackByEnemy(enemy)) {
        return;
    }
    uint8_t stopped = 0;
    if (worldMap->tiles[next_pos_tiles_y * (WIDTH_TILES) + next_pos_tiles_x] != WALL) {
        int8_t dx = enemy_position_x - next_pos_x;
        int8_t dy = enemy_position_y - next_pos_y;
        if (dx < 0) {
            move(enemy, (POS){1, 0});
        } else if (dx > 0) {
            move(enemy, (POS){-1, 0});
        } else {
            stopped |= 1 << 1;
        }
        if (dy < 0) {
            move(enemy, (POS){0, 1});
        } else if (dy > 0) {
            move(enemy, (POS){0, -1});
        } else {
            stopped |= 1;
        }
        if (!(stopped & 1 && stopped & 0b10)) {
            return;
        }
    }
    int16_t goal = getGoal(enemy->flag);
    if (goal == -1) return;
    uint8_t goal_position_x = (goal >> 8) & 0xff;
    uint8_t goal_position_y = goal & 0xff;
    HPATH path = find_path(enemy_position_x / TILE_SIZE, enemy_position_y / TILE_SIZE,
        goal_position_x, goal_position_y);
    if (path->avaliable) {
        enemy->flag &= ~(0xffff << 4);
        enemy->flag |= (int)(path->next) << 4;
    }
    free(path);
}

void damageEnemy(HENEMY enemy) {}

