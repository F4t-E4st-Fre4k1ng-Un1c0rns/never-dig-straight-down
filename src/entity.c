#include "entity.h"
#include "renderer.h"
#include "map.h"
#include "world.h"
#include "game.h"

void manageCollision(HRECT collider, HPOS dir) {
    if (!worldMap) return;
    if (collider->width <= 0 || collider->height <= 0) {
        // it's reaper
        return;
    }
    POS flag = *dir;
    if (collider->x + dir->x < 0 ||
        (collider->width + collider->x + dir->x) >= worldMap->width*TILE_SIZE) {
        flag.x = 0;
    }
    if (collider->y + dir->y < 0 ||
        (collider->height + collider->y + dir->y) >= worldMap->height * TILE_SIZE) {
        flag.y = 0;
    }
    if (dir->y > 0) {
        if (doesPointObstructed((POS) { collider->x, collider->y + collider->height + dir->y }) || doesPointObstructed((POS) { collider->x + collider->width, collider->y + collider->height + dir->y }))
            flag.y = 0;
    }
    else if (dir->y < 0) {
        if (doesPointObstructed((POS) { collider->x, collider->y + dir->y }) || doesPointObstructed((POS) { collider->x + collider->width, collider->y + dir->y }))
            flag.y = 0;
    }
    if (dir->x > 0) {
        if (doesPointObstructed((POS){ collider->x + collider->width + dir->x, collider->y}) || doesPointObstructed((POS) { collider->x + collider->width + dir->x, collider->y + collider->height}))
            flag.x = 0;
    }
    else if (dir->x < 0) {
        if (doesPointObstructed((POS) { collider->x + dir->x, collider->y}) || doesPointObstructed((POS) { collider->x + dir->x, collider->y + collider->height}))
            flag.x = 0;
    }
    dir->x &= flag.x;
    dir->y &= flag.y;
}

void renderE(HENTITY e, int time) {
    if (!ACTIVE(e->flag)) return;
    if (TYPE(e->flag) != TPLAYER && worldState->timeOnLevel < 120) {
        if (time/15 % 3 == 0) return;
    }
    updateA(&e->controller, time);
    renderA(&e->controller);
    //renderCollider(&e->collider);
}

void move(HENTITY e, POS dir) {
    BOOL last_flag = e->controller.flag;
    e->controller.flag = ((dir.y != 0 || LAST(last_flag)) && (dir.x == 0)) << 2;
    e->controller.flag |= (dir.x ? (dir.x > 0) : H_FLIP(last_flag)) | ((dir.y ? (dir.y > 0) : V_FLIP(last_flag)) << 1);
    e->controller.flag |= last_flag & (~0b111);
    if (((e->flag >> 1) & 0b111) != TREAPER) manageCollision(&e->collider, &dir);
    e->collider.y += dir.y;
    e->collider.x += dir.x;
    e->controller.collider.x = e->collider.x - 5;
    e->controller.collider.y = e->collider.y - 5;
}


void damage(HENTITY e)
{
    e->flag &= ~0b1;
}
