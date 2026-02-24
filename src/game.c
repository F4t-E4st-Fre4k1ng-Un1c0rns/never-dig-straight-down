#include "game.h"
#include "updater.h"
#include "generator.h"
#include "colors.h"
#include <stdlib.h>
#include "entity.h"
#include "leaderboard.h"
void changeState(RENDER_STATE to, int32_t t) {
    if (to == RENDER_GAME_WORLD) {
        worldState->level = 0;
        worldState->score = 0;
        worldState->scoreSize = 1;
        worldState->timeOnLevel = 0;
        free(entities);
        entities = NULL;
        enti_gap = 0;
        generateNewWorld(t);
    }
    rendererState = to;
}

void generateNewWorld(int32_t t) {
    worldState->timeOnLevel = 0;
    generate(t);
}

