#include <stdint.h>
#include "world.h"
#ifndef GAME_STATE_INCLUDED
#define GAME_STATE_INCLUDED

typedef enum {
    RENDER_GAME_WORLD,
    RENDER_MAIN_MENU,
    RENDER_DEAD_MENU
} RENDER_STATE;

typedef struct {
    int8_t level;
    int8_t score;
    int8_t scoreSize;
    uint16_t timeOnLevel;
} WORLD_STATE, *HWORLD_STATE;

extern HWORLD_STATE worldState;
extern RENDER_STATE rendererState;
extern BOOL player_finishing;
void changeState(RENDER_STATE to, int32_t t);
void generateNewWorld(int32_t t);

#endif
