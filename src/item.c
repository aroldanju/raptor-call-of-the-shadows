#include "item.h"
#include "definitions.h"
#include <memory.h>
#include <resources.h>

BOOL item_load(ItemData* item, const char *data, uint32_t length) {
    if (length != sizeof(ItemData)) {
        return FALSE;
    }

    if (data[0] != 'I' || data[1] != 'T' || data[2] != 'M') {
        return FALSE;
    }

    memcpy(item, data, length);

    return TRUE;
}

void item_render(Item* item, Video* video)	{
	if (item->created && !item->destroy) {
		actor_render(&item->actor, video);
		animation_render(&item->glow, video, item->actor.x - 8, item->actor.y - 8);
	}
}

void item_update(Item* item, float dt)	{
	if (item->created && !item->destroy) {
		actor_update(&item->actor, dt);

		animation_update(&item->glow);

		// Fake circle movement
		item->actor.x += cos(ITEM_INTERNAL_ANIMATION_STEPS[item->step] * M_PI / 180.0f) * 0.1f;
		item->actor.y += sin(ITEM_INTERNAL_ANIMATION_STEPS[item->step] * M_PI / 180.0f) * 0.1f;
		if (SDL_GetTicks() - item->stepTick >= 500) {
			item->stepTick = SDL_GetTicks();
			item->step++;
			if (item->step >= 4) {
				item->step = 0;
			}
		}
	}
}

void item_release(Item* item)	{
	int i;

	if (item->created && !item->destroy) {
		for (i = 0; i < item->data->frames; i++) {
			image_destroy(&item->frames[i]);
		}
	}
}

void item_create(Item* item, const ItemData* data, uint32_t id)	{
	int i;
	Image *bonusSheet = G_RESOURCES.images[IMAGE_BONUS];

	item->id = id;
	item->step = 0;
	item->stepTick = SDL_GetTicks();
	item->data = data;
	item->actor.x = item->actor.y = 0;
	item->actor.xVel = 0.0f;
	item->actor.yVel = GAME_SCENE_SPEED;
	item->actor.w = item->actor.h = 16;
	sprite_create(&item->actor.sprite);
	Animation* animation = sprite_createState(&item->actor.sprite);
	animation->delay = 100;
	for (i = 0; i < data->frames; i++) {
		image_extract(&item->frames[i], bonusSheet, 16 * i, 16 * data->thumbnail, 16 ,16);
		animation_addFrame(animation, &item->frames[i]);
	}

	animation_create(&item->glow, 50);
	for (i = 0; i < IMAGE_GLOW_ITEM_SIZE; i++) {
		Image *frame = G_RESOURCES.images[IMAGE_GLOW_ITEM_START + i];
		image_set_alpha(frame, 64);
		animation_addFrame(&item->glow, frame);
	}

	item->created = TRUE;
	item->destroy = FALSE;
}
