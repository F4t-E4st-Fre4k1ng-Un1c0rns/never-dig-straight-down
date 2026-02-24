#include "shovel.h"
#include "map.h"
#include "player.h"

void slice(HSHOVEL shovel, int time) {
	if (!ACTIVE(shovel->flag)) return;
	if (time - CREATE_TIME(shovel->flag) >= shovel->controller.animation->frame_duration* shovel->controller.animation->frame_len) {
		damage(shovel);
		return;
	}
	if (time - CREATE_TIME(shovel->flag) > shovel->controller.animation->frame_duration * 3 / 2) {
		return;
	}
	if (time - CREATE_TIME(shovel->flag) > shovel->controller.animation->frame_duration) {
		for (int i = shovel->collider.x / TILE_SIZE; i <= (shovel->collider.x + shovel->collider.width) / TILE_SIZE; i++) {
			for (int j = shovel->collider.y / TILE_SIZE; j <= (shovel->collider.y + shovel->collider.height) / TILE_SIZE; j++) {
				mineBlock((POS){ i, j });
			}
		}
		attackByPlayer(&shovel->collider);
	}
}
