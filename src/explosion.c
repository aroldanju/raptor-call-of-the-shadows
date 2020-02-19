/*
 *  Raptor: Call of the Shadows clone
 *  Copyright (C) 2020 A. Roldán
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * 	arldn.dev -at- gmail.com
 */

#include "explosion.h"
#include "definitions.h"

uint32_t explosion_get_size(uint8_t type)	 	{

	switch (type) {
		case EXPLOSION_FLARE:
			return G_RESOURCES.images[IMAGE_EXPLOSION_FLARE_START]->surface->w;

		case EXPLOSION_BUILDING:
			return G_RESOURCES.images[IMAGE_EXPLOSION_BUILDING_START]->surface->w;

		case EXPLOSION_SPARKLE:
			return G_RESOURCES.images[IMAGE_SPARKLE_START]->surface->w;

		case EXPLOSION_BSPARK:
			return G_RESOURCES.images[IMAGE_BSPARK_START]->surface->w;

		case EXPLOSION_OSPARK:
			return G_RESOURCES.images[IMAGE_OSPARK_START]->surface->w;

		case EXPLOSION_SHIP:
			return G_RESOURCES.images[IMAGE_EXPLOSION_SHIP_START]->surface->w;

		case EXPLOSION_LARGE:
			return G_RESOURCES.images[IMAGE_EXPLOSION_START]->surface->w;

		case EXPLOSION_EXTRALARGE:
			return G_RESOURCES.images[IMAGE_EXPLOSION_EXTRALARGE_START]->surface->w;

		case EXPLOSION_BONUS:
			return G_RESOURCES.images[IMAGE_BONUS_EXPLOSION_START]->surface->w;

		case EXPLOSION_HIT:
			return G_RESOURCES.images[IMAGE_EXPLOSION_HIT_START]->surface->w;

		default:
			return 0;
	}
}

void explosion_create(Explosion* explosion, uint8_t type, Resources* resources)   {

    Animation* state = NULL;
    int i;

    explosion_release(explosion);
    sprite_create(&explosion->actor.sprite);

    switch (type) {
        case EXPLOSION_FLARE:
            state = sprite_createState(&explosion->actor.sprite);

            state->infinite = FALSE;
            state->delay = 40;
            sprite_set_current_state(&explosion->actor.sprite, 0);

            for (i = 0; i < 26; i++) {
                animation_addFrame(state, resources->images[IMAGE_EXPLOSION_FLARE_START + i]);
            }

            break;

        case EXPLOSION_BUILDING:
            state = sprite_createState(&explosion->actor.sprite);

            state->infinite = FALSE;
            state->delay = 40;
            sprite_set_current_state(&explosion->actor.sprite, 0);

            for (i = 0; i < 35; i++) {
                animation_addFrame(state, resources->images[IMAGE_EXPLOSION_BUILDING_START + i]);
            }

            break;

        case EXPLOSION_SPARKLE:
            state = sprite_createState(&explosion->actor.sprite);

            state->infinite = FALSE;
            state->delay = 40;
            sprite_set_current_state(&explosion->actor.sprite, 0);

            for (i = 0; i < 17; i++) {
                animation_addFrame(state, resources->images[IMAGE_SPARKLE_START + i]);
            }

            break;

        case EXPLOSION_BSPARK:
            state = sprite_createState(&explosion->actor.sprite);

            state->infinite = FALSE;
            state->delay = 40;
            sprite_set_current_state(&explosion->actor.sprite, 0);

            for (i = 0; i < 9; i++) {
                animation_addFrame(state, resources->images[IMAGE_BSPARK_START + i]);
            }

            break;

        case EXPLOSION_OSPARK:
            state = sprite_createState(&explosion->actor.sprite);

            state->infinite = FALSE;
            state->delay = 40;
            sprite_set_current_state(&explosion->actor.sprite, 0);

            for (i = 0; i < 9; i++) {
                animation_addFrame(state, resources->images[IMAGE_OSPARK_START + i]);
            }

            break;

        case EXPLOSION_SHIP:
            state = sprite_createState(&explosion->actor.sprite);

            state->infinite = FALSE;
            state->delay = 30;
            sprite_set_current_state(&explosion->actor.sprite, 0);

            for (i = 0; i < IMAGE_EXPLOSION_SHIP_SIZE; i++) {
                animation_addFrame(state, resources->images[IMAGE_EXPLOSION_SHIP_START + i]);
            }

            break;

        case EXPLOSION_LARGE:
            state = sprite_createState(&explosion->actor.sprite);

            state->infinite = FALSE;
            state->delay = 30;
            sprite_set_current_state(&explosion->actor.sprite, 0);

            for (i = 0; i < IMAGE_EXPLOSION_SIZE; i++) {
                animation_addFrame(state, resources->images[IMAGE_EXPLOSION_START + i]);
            }

            break;

        case EXPLOSION_EXTRALARGE:
            state = sprite_createState(&explosion->actor.sprite);

            state->infinite = FALSE;
            state->delay = 30;
            sprite_set_current_state(&explosion->actor.sprite, 0);

            for (i = 0; i < IMAGE_EXPLOSION_EXTRALARGE_SIZE; i++) {
                animation_addFrame(state, resources->images[IMAGE_EXPLOSION_EXTRALARGE_START + i]);
            }

            break;

        case EXPLOSION_BONUS:
            state = sprite_createState(&explosion->actor.sprite);

            state->infinite = FALSE;
            state->delay = 30;
            sprite_set_current_state(&explosion->actor.sprite, 0);

            for (i = 0; i < IMAGE_BONUS_EXPLOSION_SIZE; i++) {
                animation_addFrame(state, resources->images[IMAGE_BONUS_EXPLOSION_START + i]);
            }

            break;

        case EXPLOSION_HIT:
            state = sprite_createState(&explosion->actor.sprite);

            state->infinite = FALSE;
            state->delay = 20;
            sprite_set_current_state(&explosion->actor.sprite, 0);

            for (i = 0; i < IMAGE_EXPLOSION_HIT_SIZE; i++) {
                animation_addFrame(state, resources->images[IMAGE_EXPLOSION_HIT_START + i]);
            }

            break;

#ifdef __DEBUG__
        default:
            printf("explosion_create(): invalid explosion type %d\n", type);
            return;
#endif
    }

    explosion->created = TRUE;
    explosion->destroy = FALSE;
    explosion->actor.xVel = explosion->actor.yVel = 0.0f;
}

void explosion_release(Explosion* explosion)  {
    if (explosion->created) {
        sprite_release(&explosion->actor.sprite);
    }

    explosion->created = FALSE;
}

void explosion_render(Explosion* explosion, Video* video)  {
    if (explosion->created && !explosion->destroy) {
        actor_render(&explosion->actor, video);
    }
}

void explosion_update(Explosion* explosion, float dt)  {
    if (explosion->created && !explosion->destroy) {
        actor_update(&explosion->actor, dt);
        if (animation_ended(sprite_getCurrentState(&explosion->actor.sprite))) {
            explosion->destroy = TRUE;
        }
    }
}

