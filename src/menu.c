#include "menu.h"
#include "game.h"
#include "player.h"
#include "tic80.h"
#include <string.h>
#include "leaderboard.h"




void renderMenu(char* title, char* subtitle, int8_t bg, int8_t fg) {
    cls(bg);
    print(title, 10, 10, fg, 0, 4, 0);
    print(subtitle, 10, 80, fg, 0, 1, 0);
}

void renderScoreboard(int8_t score, int8_t bg, int8_t fg) {
    cls(bg);
    print("You died", 10, 10, fg, 0, 3, 0);
    char Buf[100];

    strcpy(Buf, "Your score is ");
    char countBuf[4];
    snprintf(countBuf, 4, "%d", score);
    strcat(Buf, countBuf);
    print(Buf, 10, 30, fg, 0, 1, 0);

    print("LEADERBOARDS", 10, 40, fg, 0, 2, 0);
    
    for (int i = 0; i < 8; i++) {
        snprintf(Buf, 100, "%4d | %-6s - %3d", i+1, leaderBoard->players[i].name, leaderBoard->players[i].score);
        print(Buf, 10, 55+i*10, fg, 0, 1, 0);
    }	

}

void updateMenu(int32_t t) {
    if (btn(BUTTON_CODE_P1_A)) {
        changeState(RENDER_GAME_WORLD, t);
    };
}

