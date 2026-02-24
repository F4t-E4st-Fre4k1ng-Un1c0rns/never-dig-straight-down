#include "player.h"
#include "tic80.h"
#include "resources.h"
#include "game.h"
#include "map.h"
#include <math.h>
#include "game.h"
#include "leaderboard.h"

void _showelAttack(HPLAYER player, int time) {
    player->flag = (time << 4) | (player->flag & 0b1111);
    int x = (H_FLIP(player->controller.flag)) ? 1 : (-1);
    int y = (V_FLIP(player->controller.flag)) ? 1 : (-1);
    x *= TILE_SIZE / 2;
    y *= TILE_SIZE / 2;
    if (LAST(player->controller.flag)) {
        x = 0;
    }
    else {
        y = 0;
    }
    sfx(63, 2, 6, 8, 1, 15, 15, 1);
    HENTITY shovel = &entities[1];
    shovel->flag |= 0b1;
    shovel->collider = player->collider;
    shovel->collider.x += 3*x/2 - (y != 0) * TILE_SIZE/4;
    shovel->collider.y += 3*y/2 - (x != 0) * TILE_SIZE/4;
    shovel->collider.height = TILE_SIZE / 2 + (x>0?x:(-x));
    shovel->collider.width = TILE_SIZE / 2 + (y>0?y:(-y));
    shovel->controller.flag = player->controller.flag;

    shovel->controller.current_frame = 0;
    shovel->controller.last_update = time;
    shovel->controller.collider = shovel->collider;
    shovel->controller.collider.width = 1*TILE_SIZE;
    shovel->controller.collider.height = 2 * TILE_SIZE;
    shovel->flag = (time << 4) | (shovel->flag & 0b1111);    
}

void update(HPLAYER player, int time) {
    if (!ACTIVE(player->flag)) {
        // we're dead
	leaderboard_add(leaderBoard, "Player", worldState->score);
        changeState(RENDER_DEAD_MENU, time);
        return;
    }
    POS dir = { 0, 0 };
    if (btn(BUTTON_CODE_P1_UP)) dir.y--;
    if (btn(BUTTON_CODE_P1_DOWN)) dir.y++;
    if (btn(BUTTON_CODE_P1_LEFT)) dir.x--;
    if (btn(BUTTON_CODE_P1_RIGHT)) dir.x++;
    if (btn(BUTTON_CODE_P1_A) && time - LAST_ATTACK(player->flag) > 60) {
    _showelAttack(player, time);
    }
    
    if (dir.x || dir.y) {
        player->controller.animation->frame_len = 4;
    } else {
        player->controller.animation->frame_len = 1;
    }
    
    if (btn(BUTTON_CODE_P1_B) &&
        (time - (LAST_ATTACK(player->flag))) > 60) {
        sfx(SHOVEL_SFX, 2, 6, 8, 1, 15, 15, 1);
        worldState->timeOnLevel = 0;
        player_finishing = 1;
        player->controller.animation = &animations[6];

    }
    move(player, dir);
}

void attackByPlayer(HRECT rect) {
    // 0 is always player and 1 is always shovel
    for (int i = 2; i < enti_size; ++i) {
        if (haveCommon(rect, &entities[i].collider) && entities[i].flag & 0b1) {
            if(((entities[i].flag >> 1) & 0b111) == TSPIDER_1) {
                worldState->score += 1;
                if (worldState->score > 2) {
                    worldState->scoreSize = log(worldState->score + 2);
                }
            }
            damage(&(entities[i]));
        }
    }
}
