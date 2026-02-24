#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leaderboard.h"

void leaderboard_init(LEADERBOARD *lb) {
    lb->count = 0;
}

void leaderboard_add(LEADERBOARD *lb, const char *name, int score) {
    int i, j;
    Player new_player;
    strcpy(new_player.name, name);
    new_player.score = score;

    if (lb->count < 8) {
        lb->players[lb->count++] = new_player;
    } else {
        if (new_player.score > lb->players[7].score) {
            lb->players[7] = new_player;
        } else {
            return;
        }
    }

    for (int i = 7; i > 0; i--) {
	int temp_score;
	char temp_name[50];
        if (lb->players[i].score > lb->players[i-1].score) {
            temp_score = lb->players[i-1].score;
	    strcpy(temp_name, lb->players[i-1].name);

            lb->players[i-1].score = lb->players[i].score;
	    strcpy(lb->players[i-1].name, lb->players[i].name);

            lb->players[i].score = temp_score;
	    strcpy(lb->players[i].name, temp_name);
        }
        else {
            break;
        }
    }
}

void leaderboard_fill(LEADERBOARD *lb) {
    leaderboard_add(lb, "Tipla", 99);
    leaderboard_add(lb, "Gostik", 63);
    leaderboard_add(lb, "Zhora", 50);
    leaderboard_add(lb, "Zloty", 40);
    leaderboard_add(lb, "Tkyugi", 30);
    leaderboard_add(lb, "Kendaj", 20);
    leaderboard_add(lb, "Mint", 10);
    leaderboard_add(lb, "Alhtrs", 5);
}
