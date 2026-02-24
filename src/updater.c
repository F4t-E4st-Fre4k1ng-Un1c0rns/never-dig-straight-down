#include "player.h"
#include "shovel.h"
#include "map.h"
#include "enemy.h"

void updateE(HENTITY e, int t) {
    switch ((e->flag >> 1) & 0b111) {
        case TPLAYER:
            // TODO
            update(e, t);
            break;
        case TSHOVEL:
            slice(e, t);
            break;
        case TRAT:
        case TSPIDER_1:
        case TSPIDER_2:
            updateEnemy(e, t);
            break;
        case TREAPER:
            updateReaper(e, t);
            break;
        default:
            trace("Update of unknown entity", 2);
    }
}

