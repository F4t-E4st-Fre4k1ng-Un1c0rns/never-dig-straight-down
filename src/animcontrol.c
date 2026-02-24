#include "animcontrol.h"
#include "renderer.h"
#include <stdlib.h>
#include "resources.h"
#include <string.h>


BOOL updateA(HAC ac, int time) 
{
	if (time - ac->last_update < ac->animation->frame_duration) return 0;
	ac->last_update = time;
	ac->current_frame = (ac->current_frame + 1) % ac->animation->frame_len;
	return 1;
}

void renderA(HAC ac)
{
	renderSprite(&ac->collider, ac->animation->frame_id[ac->current_frame], ac->flag);
}

HANIMATION prepareAnimations() {
	HANIMATION animations = malloc(7*sizeof(ANIMATION));

	animations[0] = (ANIMATION){ (unsigned int*)malloc(4*sizeof(int)), 1, 13 };
	animations[0].frame_id[0] = PLAYER_IDLE_SPRITE;
	animations[0].frame_id[1] = PLAYER_MOVE_SPRITE_1;
	animations[0].frame_id[2] = PLAYER_MOVE_SPRITE_2;
	animations[0].frame_id[3] = PLAYER_MOVE_SPRITE_3;

	animations[1] = (ANIMATION){ (unsigned int*)malloc(sizeof(int) * 2), 2, 13 };
	animations[1].frame_id[0] = RAT_SPRITE_1;
	animations[1].frame_id[1] = RAT_SPRITE_2;

	animations[2] = (ANIMATION){ (unsigned int*)malloc(sizeof(int) * 3), 3, 5 };
	animations[2].frame_id[0] = SHOVEL_SLICE_SPRITE_1;
	animations[2].frame_id[1] = SHOVEL_SLICE_SPRITE_2;
	animations[2].frame_id[2] = SHOVEL_SLICE_SPRITE_3;

	animations[3] = (ANIMATION){ (unsigned int*)malloc(sizeof(int) * 4), 4, 10 };
	animations[3].frame_id[0] = SPRIDER_BIG_SPRITE_1;
	animations[3].frame_id[1] = SPRIDER_BIG_SPRITE_2;
	animations[3].frame_id[2] = SPRIDER_BIG_SPRITE_3;
	animations[3].frame_id[3] = SPRIDER_BIG_SPRITE_4;

	animations[4] = (ANIMATION){ (unsigned int*)malloc(sizeof(int) * 2), 2, 20 };
	animations[4].frame_id[0] = REAPER_SPRITE_1;
	animations[4].frame_id[1] = REAPER_SPRITE_2;

	animations[5] = (ANIMATION){ (unsigned int*)malloc(sizeof(int) * 2), 2, 20 };
	animations[5].frame_id[0] = REAPER_CAT_SPRITE_1;
	animations[5].frame_id[1] = REAPER_CAT_SPRITE_2;

	animations[6] = (ANIMATION){ (unsigned int*)malloc(sizeof(int) * 2), 2, 30 };
	animations[6].frame_id[0] = PLAYER_IDLE_SPRITE;
	animations[6].frame_id[1] = PLAYER_DIG_SPRITE;
	return animations;
}

