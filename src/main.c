#include <stdlib.h>
#include "tic80.h"
#include "resources.h"
#include "updater.h"
#include "menu.h"
#include "game.h"
#include "renderer.h"
#include "generator.h"
#include "leaderboard.h"

HANIMATION animations;
int anim_size;
HENTITY entities;
int enti_size, enti_gap;
int8_t level = 0;

HMAP worldMap;
HPLAYER player;

HWORLD_STATE worldState;
RENDER_STATE rendererState = RENDER_MAIN_MENU;
BOOL player_finishing;

HLEADERBOARD leaderBoard;

WASM_EXPORT("BOOT")
void BOOT() {
    animations = prepareAnimations();
    worldMap = createMap(WIDTH_TILES, HEIGHT_TILES);
    worldState = calloc(sizeof(WORLD_STATE), 1);
    music(GAME_MUSIC, -1, -1, 1, 0, -1, -1);
    player_finishing = 0;
    entities = NULL;
    enti_size = 0;

    leaderBoard = calloc(sizeof(LEADERBOARD), 1);
    leaderboard_init(leaderBoard);
    leaderboard_fill(leaderBoard);
    //leaderboard_add(leaderBoard, "Hi", 100);
}

WASM_EXPORT("TIC")
void TIC() {
    cls(-1);
    static int t;
    switch (rendererState) {
        case RENDER_GAME_WORLD:
          renderMap(worldMap);
          if (!player_finishing){
              for (int i = 0; i < enti_size; ++i) {
                  updateE(&entities[i], t);
              }
          }
          else {
              
              if (worldState->timeOnLevel >= 120) {
                  player_finishing = 0;
                  generateNewWorld(t);
                  return;
              }
              if (worldState->timeOnLevel >= 40) {
                  renderTile((POS) { player->collider.x, player->collider.y }, (POS) { 2, 2 }, HOLE_SPRITE_1, 1);
              }
              if (worldState->timeOnLevel >= 80) {
                  renderTile((POS) { player->collider.x, player->collider.y }, (POS) { 2, 2 }, HOLE_SPRITE_2, 1);
              }
              
              for (int i = 0; i < worldMap->width; i++) {
                  for (int j = 0; j < worldMap->height; j++) {
                      if ((player->collider.x / TILE_SIZE - i) * (player->collider.x / TILE_SIZE - i) +
                          (player->collider.y / TILE_SIZE - j) * (player->collider.y / TILE_SIZE - j)
                  > 120 - worldState->timeOnLevel) worldMap->tiles[j * worldMap->width + i] = WALL;
                  }
              }
          }
          for (int i = 0; i < enti_size; ++i) {
              renderE(&entities[i], t);
          }
          renderScore();
          if (worldState) {
              worldState->timeOnLevel++;
          }
          break;
      case RENDER_MAIN_MENU:
          renderMenu("SPACE\nGULAG!!!!",
              "Press Z on keyboard or A on gamepad\nto create new game",
              8, 3);
          updateMenu(t);
          break;
      case RENDER_DEAD_MENU:
	  renderScoreboard(worldState->score, 8, 3);
          updateMenu(t);
          break;
    }
    t += 1;
}

