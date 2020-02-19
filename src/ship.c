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

#include "ship.h"
#include "definitions.h"
#include "data.h"

BOOL shipdata_load(ShipData* ship, const char *data, uint32_t length) {
    if (length != sizeof(ShipData)) {
        return FALSE;
    }

    if (data[0] != 'S' || data[1] != 'H' || data[2] != 'P') {
        return FALSE;
    }

    memcpy(ship, data, length);

    return TRUE;
}

void ship_change_weapon(Ship* ship) {
    int i;

    for (i = ship->selectedWeaponIndex; i < ship->numWeapons; i++) {
        if (!ship->weapons[i].data->alwaysEquipped && ship->selectedWeapon != &ship->weapons[i]) {
            ship->selectedWeapon = &ship->weapons[i];
            ship->selectedWeaponIndex = i;
            break;
        }
    }
}

void ship_add_weapon(Ship* ship, WeaponData* weapon, Resources* resources)    {
    int i;

    // Check if ship already has the weapon
    for (i = 0; i < ship->numWeapons; i++) {
        if (ship->weapons[i].data == weapon) {
            return;
        }
    }

    ship->weapons[ship->numWeapons].data = weapon;
    ship->weapons[ship->numWeapons].sound = resources->sounds[weapon->soundId];
    ship->weapons[ship->numWeapons].lastShot = 0;
    ship->numWeapons++;
}

void ship_create_empty(Ship* ship)	{
	ship->damaged = ship->burstingUp = FALSE;
	ship->created = FALSE;
	ship->destroy = FALSE;
	ship->actor.xVel = ship->actor.xVel = 0.0f;
	ship->mapEnemy = NULL;
	ship->numWeapons = 0;
	ship->target = NULL;
	image_set_alpha(&ship->shadow, 128);
	ship->lastFrame = NULL;
}

BOOL ship_create_from_enemy(Ship* ship, const struct _map_enemy* enemy, Resources* resources) {

	int i;

	ship_create_empty(ship);
    ship_release(ship);

	const uint32_t enemyId = enemy->type;

	if (enemyId == SHIP_PILOT || enemyId >= SHIP_LAST) {
#ifdef __DEBUG__
		printf("ship_create_from_enemy(): invalid enemy id #%d\n", enemyId);
#endif
		return FALSE;
	}

	ship->data = G_DATA.ships[enemyId];

    sprite_create(&ship->actor.sprite);
    Animation *state = sprite_createState(&ship->actor.sprite);
    for (i = 0; i < ship->data->numFrames; i++) {
    	animation_addFrame(state, resources->images[ship->data->frames[i]]);
    }
    state->delay = 150;
	state->currentFrame = 0;
	state->infinite = TRUE;

	if (enemy->behavior != 0) {
		behaviour_reset(&ship->behaviour);
		ship->behaviour.behaviour = G_DATA.behaviour[enemy->behavior - 1];
	}

	ship->created = TRUE;
	ship->destroy = FALSE;
	ship->actor.w = state->frames[0]->surface->w;
	ship->actor.h = state->frames[0]->surface->h;


	ship->energy = ship->data->energy;

	ship->mapEnemy = enemy;

	return TRUE;
}

void ship_render(Ship* ship, Video* video)  {
    if (ship->created && !ship->destroy) {

    	// Place shadow position
    	if (ship->data->ground == 0) {
			const float xoffset = (ship->actor.x - (VIDEO_SCREEN_WIDTH / 3.0f)) * 0.25f;
			const float yoffset = (ship->actor.y - (VIDEO_SCREEN_HEIGHT / 2.0f)) * 0.25f;
			image_render_sized(&ship->shadow, ship->actor.x - xoffset, ship->actor.y - yoffset, ship->actor.w *0.75f, ship->actor.h *0.75f, video);
		}

        actor_render(&ship->actor, video);

        /*
		for (i = 0; i < SHIP_MAX_EXPLOSIONS; i++) {
			explosion_render(&ship->explosions[i], video);
		}
		*/
    }
}

uint32_t ship_get_width(Ship *ship)	{
	Animation* animation = sprite_getCurrentState(&ship->actor.sprite);
	if (animation) {
		const Image *frame = animation_getCurrentFrame(animation);
		if (frame && frame->surface) {
			return frame->surface->w;
		}
	}

	return 0;
}

uint32_t ship_get_height(Ship* ship)	{
	Animation* animation = sprite_getCurrentState(&ship->actor.sprite);
	if (animation) {
		const Image *frame = animation_getCurrentFrame(animation);
		if (frame && frame->surface) {
			return frame->surface->h;
		}
	}

	return 0;
}

void ship_release(Ship* ship)   {
    if (ship->created) {
        sprite_release(&ship->actor.sprite);
    }

    ship->behaviour.behaviour = NULL;
    ship->behaviour.lastBehaviourStep = NULL;

    ship->burstingUp = FALSE;
    ship->damaged = FALSE;
    ship->numWeapons = 0;

    image_destroy(&ship->shadow);
    ship->lastFrame = NULL;

    // Reset velocity
    ship->actor.yVel = 0.0f;
    ship->actor.xVel = 0.0f;
    ship->actor.angle = 0.0f;
    ship->actor.targeted = FALSE;
    ship->actor.angled = FALSE;
}


void ship_damage(Ship* ship)	{
	ship->damaged = TRUE;
}

void ship_burstup(Ship* ship)	{
	ship->burstingUp = TRUE;
	ship->burstingUpTick = SDL_GetTicks();
}

void run_behvaiour_step(Ship* ship) {

    const Behaviour *behaviour = ship->behaviour.behaviour;

    if (!behaviour) {
#ifdef __DEBUG__
        printf("run_behaviour_step(): invalid behaviour\n");
#endif
        return;
    }

    // Reached end of behaviour
    if (behaviour->infinite == 0 && ship->behaviour.currentBehaviourStep >= ship->behaviour.behaviour->numSteps) {
        return;
    }

    // Restart behaviour steps if the last step has been reached
    if (ship->behaviour.behaviour->infinite == 1 && ship->behaviour.currentBehaviourStep >= ship->behaviour.behaviour->numSteps) {
        ship->behaviour.currentBehaviourStep = 0;
    }

    const struct _behaviour_step *step = &ship->behaviour.behaviour->steps[ship->behaviour.currentBehaviourStep];

    switch(step->command) {
        case BEHAVIOURCOMMAND_SHOT:
            // Shot
            //printf("run_behaviour_step(): shot %d, %d, %d, %d\n", step->weapon, step->x, step->y, step->target);
            ship->on_shot(ship->game, ship, step->weapon);
            ship->behaviour.lastBehaviourTick = SDL_GetTicks();
            break;

        case BEHAVIOURCOMMAND_MOVEX:
            //printf("run_behaviour_step(): %u movex %.2f, %.2f\n", behaviour, step->xvel, step->maxxvel);
            if (step->maxxvel != 0.0f) {
                ship->actor.xVel = (step->xvel * 100.0f + (rand() % (int)(step->maxxvel * 100.0f - step->xvel * 100.0f))) / 100.0f;
            }
            else {
                ship->actor.xVel = /*GAME_SCENE_SPEED * */step->xvel;
            }

            ship->behaviour.lastBehaviourTick = SDL_GetTicks();
            ship->actor.targeted = FALSE;

            break;

        case BEHAVIOURCOMMAND_MOVEY:
            //printf("run_behaviour_step(): %u movey %.2f, %.2f\n", behaviour, step->yvel, step->maxyvel);
            if (step->maxyvel != 0.0f) {
                ship->actor.yVel = (step->yvel * 100.0f + (rand() % (int)(step->maxyvel * 100.0f - step->yvel * 100.0f))) / 100.0f;
            }
            else {
                ship->actor.yVel = /*GAME_SCENE_SPEED * */step->yvel;
            }

            ship->behaviour.lastBehaviourTick = SDL_GetTicks();
            ship->actor.targeted = FALSE;

            break;

        case BEHAVIOURCOMMAND_DELAY:
            //printf("run_behaviour_step(): %u delay %d\n", behaviour, step->delay);
            if (SDL_GetTicks() - ship->behaviour.lastBehaviourTick < step->delay) {
                return;
            }
            break;

        case BEHAVIOURCOMMAND_GOTO:
            //printf("run_behaviour_step(): goto %d\n", step->data1);
            ship->behaviour.currentBehaviourStep = step->data1;
            break;

        case BEHAVIOURCOMMAND_SET_VELOCITY:
            //printf("run_behaviour_step(): set velocity %.2f\n", step->xvel);
            ship->actor.xVel = step->xvel;
            ship->actor.yVel = step->yvel;
            ship->behaviour.lastBehaviourTick = SDL_GetTicks();
            break;

        case BEHAVIOURCOMMAND_SET_ANGLE:
            //printf("run_behaviour_step(): set angle %.2f\n", step->angle);
            ship->actor.angle = step->angle * M_PI / 180.0f;
            ship->actor.angled = TRUE;
            ship->actor.targeted = FALSE;
            ship->behaviour.lastBehaviourTick = SDL_GetTicks();
            break;

        case BEHAVIOURCOMMAND_MOVETARGET:
            // Vel x = y
            ship->actor.xVel = (step->xvel * 100.0f + (rand() % (int)(step->maxxvel * 100.0f - step->xvel * 100.0f))) / 100.0f;
            ship->actor.yVel = (step->xvel * 100.0f + (rand() % (int)(step->maxxvel * 100.0f - step->xvel * 100.0f))) / 100.0f;
            ship->target = NULL;
            ship->actor.angle = FALSE;
            ship->actor.targeted = TRUE;
            break;

    }

    ship->behaviour.lastBehaviourStep = step;
    if (step->command != BEHAVIOURCOMMAND_GOTO) {
        ship->behaviour.currentBehaviourStep++;
    }

    return run_behvaiour_step(ship);
}

void ship_update(Ship* ship, float dt) {
    if (ship->created && !ship->destroy) {
        actor_update(&ship->actor, dt);

        // If sprite has changed, update the shadow
        Image *currentFrame = animation_getCurrentFrame(sprite_getCurrentState(&ship->actor.sprite));
        if (currentFrame && currentFrame != ship->lastFrame) {
			ship->lastFrame = currentFrame;
			image_copy(&ship->shadow, animation_getCurrentFrame(sprite_getCurrentState(&ship->actor.sprite)));
			image_set_alpha(&ship->shadow, 64);
			image_set_color_mod(&ship->shadow, 0, 0, 0);
        }

        if (ship->behaviour.behaviour) {
            run_behvaiour_step(ship);
        }

        if (ship->actor.y > VIDEO_SCREEN_HEIGHT + 64) {
            ship->destroy = TRUE;
        }

        /*
        for (i = 0; i < SHIP_MAX_EXPLOSIONS; i++) {
        	explosion_update(&ship->explosions[i], dt);
        }
        */
    }
}

/*
void ship_add_explosion(Ship* ship, uint8_t type)	{
	int i;

	for (i = 0; i < SHIP_MAX_EXPLOSIONS; i++) {
		Explosion* explosion = &ship->explosions[i];

		if (explosion->destroy) {
			explosion_create(explosion, type, &G_RESOURCES);
			explosion->actor.x = (ship->actor.x - 24 / 2) + (rand() % 40);
			explosion->actor.y = (ship->actor.y - 24 / 2) + (rand() % 40);

			return;
		}
	}

	printf("ship_add_explosion(): any explosion slot was found\n");
}
*/

void ship_remove_energy(Ship* ship, uint32_t damage) {
    if (ship->energy > damage) {
        ship->energy -= damage;

		// Damaged ship when energy becomes < 20%
		if (ship->energy < ((float)ship->data->energy * 0.2f) && !ship->damaged) {
			ship_damage(ship);
		}
    }
    else {
        ship->energy = 0;

		// Shattered ship
		if (!ship->burstingUp) {
			ship_burstup(ship);
		}
    }
}