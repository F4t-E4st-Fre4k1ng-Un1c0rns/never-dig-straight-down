#include <stdint.h>
#include "leaderboard.h"
#ifndef MENU_INCLUDED
#define MENU_INCLUDED

void renderMenu(char* title, char* subtitle, int8_t bg, int8_t fg);
void renderScoreboard(int8_t score, int8_t bg, int8_t fg);
void updateMenu(int32_t);

#endif
