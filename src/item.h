#ifndef __item_h
#define __item_h

#include <stdint.h>
#include "defs.h"
#include "actor.h"

typedef struct _item_data {
    uint8_t magic[3];
    uint8_t title[32];
    uint8_t subtitle[16];
    uint8_t description[256];
    uint32_t cost, resale, pack;
    uint32_t maxAmount, minAmount;
    uint8_t icon;
    uint8_t duplicate;
    uint8_t thumbnail;
    uint8_t frames;
    uint32_t score;
} ItemData;

#define ITEM_MAX_FRAMES	4

static const float ITEM_INTERNAL_ANIMATION_STEPS[4] = { 45.0f, 135.0f, 225.0f, 315.0f };

typedef struct _item {
	uint32_t 	id;
	BOOL        destroy;
	BOOL        created;
	ItemData 	*data;
	Actor 		actor;
	Image 		frames[ITEM_MAX_FRAMES];
	uint8_t 	step;
	uint32_t 	stepTick;
	Animation	glow;
} Item;

BOOL item_load(ItemData* item, const char *data, uint32_t length);

void item_render(Item* item, Video* video);

void item_update(Item* item, float dt);

void item_release(Item* item);

void item_create(Item* item, const ItemData* data, uint32_t id);

#endif  /* __item_h */