#include <stdint.h>

#ifndef LEADERBOARD_INCLUDED
#define LEADERBOARD_INCLUDED

#define MAX_PLAYERS 8

typedef struct {
    char name[50];
    int score;
} Player;

typedef struct {
    Player players[MAX_PLAYERS];
    int count;
} LEADERBOARD, *HLEADERBOARD;

extern HLEADERBOARD leaderBoard;


void leaderboard_init(LEADERBOARD *lb);
void leaderboard_add(LEADERBOARD *lb, const char *name, int score);
void leaderboard_fill(LEADERBOARD *lb);


#endif
