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

#include "bullet.h"
#include "definitions.h"

#include "data.h"

Data G_DATA;

void bullet_create(Bullet* bullet, const struct _weapon_data* weapon, Resources* resources) {

    int i;

    // Release last bullet
    bullet_release(bullet);

    // Create sprite
    sprite_create(&bullet->actor.sprite);
    Animation* state = sprite_createState(&bullet->actor.sprite);
    state->delay = 80;
    state->currentFrame = rand() % weapon->bulletFramesNumber;
    for (i = 0; i < weapon->bulletFramesNumber; i++) {
        if(i == 0) {
            // Set size by first frame
            bullet->actor.w = resources->images[weapon->bulletFrames[i]]->surface->w;
            bullet->actor.h = resources->images[weapon->bulletFrames[i]]->surface->h;
        }
        animation_addFrame(state, resources->images[weapon->bulletFrames[i]]);
    }

    // Sets behaviour and weapon
    behaviour_reset(&bullet->behaviour);
    bullet->behaviour.behaviour = G_DATA.behaviour[weapon->behaviour - 1];
    bullet->weapon = weapon;
    bullet->actor.targeted = FALSE;
    bullet->actor.angled = FALSE;
    bullet->actor.angle = 0.0f;
    bullet->target = NULL;
    bullet->actor.xVel = bullet->actor.yVel = 0.0f;

    // Sets bullet as created
    bullet->destroy = FALSE;
    bullet->created = TRUE;
}

void bullet_render(Bullet* bullet, Video* video)    {
    if (bullet->created && !bullet->destroy) {
        actor_render(&bullet->actor, video);
    }
}

void bullet_run_behvaiour_step(Bullet* bullet) {

    Behaviour *behaviour = bullet->behaviour.behaviour;

    // Reached end of behaviour
    if (behaviour->infinite == 0 && bullet->behaviour.currentBehaviourStep >= bullet->behaviour.behaviour->numSteps) {
        return;
    }

    // Restart behaviour steps if the last step has been reached
    if (bullet->behaviour.behaviour->infinite == 1 && bullet->behaviour.currentBehaviourStep >= bullet->behaviour.behaviour->numSteps) {
        bullet->behaviour.currentBehaviourStep = 0;
    }

    struct _behaviour_step *step = &bullet->behaviour.behaviour->steps[bullet->behaviour.currentBehaviourStep];

    switch(step->command) {
        case BEHAVIOURCOMMAND_MOVEX:
            //printf("run_behaviour_step(): movex %.2f, %.2f\n", step->xvel, step->maxxvel);
            if (step->maxxvel != 0.0f) {
                bullet->actor.xVel = (step->xvel * 100.0f + (rand() % (int)(step->maxxvel * 100.0f - step->xvel * 100.0f))) / 100.0f;
            }
            else {
                bullet->actor.xVel = step->xvel;
            }

            bullet->behaviour.lastBehaviourTick = SDL_GetTicks();
            bullet->actor.targeted = FALSE;

            break;

        case BEHAVIOURCOMMAND_MOVEY:
            //printf("run_behaviour_step(): movey %.2f, %.2f\n", step->yvel, step->maxyvel);
            if (step->maxyvel != 0.0f) {
                bullet->actor.yVel = (step->yvel * 100.0f + (rand() % (int)(step->maxyvel * 100.0f - step->yvel * 100.0f))) / 100.0f;
            }
            else {
                bullet->actor.yVel = step->yvel;
            }

            bullet->behaviour.lastBehaviourTick = SDL_GetTicks();
            bullet->actor.targeted = FALSE;

            break;

        case BEHAVIOURCOMMAND_DELAY:
            //printf("run_behaviour_step(): delay %d\n", step->delay);
            //ship->lastBehaviourTick = SDL_GetTicks() + step->delay;
            if (SDL_GetTicks() - bullet->behaviour.lastBehaviourTick < step->delay) {
                return;
            }
            break;

    	case BEHAVIOURCOMMAND_MOVETARGET:
    	    // Vel x = y
    	    bullet->actor.xVel = step->xvel;
    	    bullet->actor.yVel = step->xvel;
    	    bullet->target = NULL;
    	    bullet->actor.targeted = TRUE;
    		break;

        case BEHAVIOURCOMMAND_GOTO:
            bullet->behaviour.currentBehaviourStep = step->data1;
            break;

        case BEHAVIOURCOMMAND_SET_VELOCITY:
            bullet->actor.xVel = step->xvel;
            bullet->actor.yVel = step->yvel;
            bullet->behaviour.lastBehaviourTick = SDL_GetTicks();
            break;

        case BEHAVIOURCOMMAND_SET_ANGLE:
            bullet->actor.angle = step->angle * M_PI / 180.0f;
            bullet->actor.angled = TRUE;
            bullet->behaviour.lastBehaviourTick = SDL_GetTicks();
            break;
    }

    bullet->behaviour.lastBehaviourStep = step;
    if (step->command != BEHAVIOURCOMMAND_GOTO) {
        bullet->behaviour.currentBehaviourStep++;
    }

    return bullet_run_behvaiour_step(bullet);
}

void bullet_update(Bullet* bullet, float dt)    {
    if (bullet->created && !bullet->destroy) {
        actor_update(&bullet->actor, dt);

        if (bullet->behaviour.behaviour) {
            bullet_run_behvaiour_step(bullet);
        }

        // Bullet out of screen
        if (bullet->actor.x < -32 || bullet->actor.y < -32 || bullet->actor.x > VIDEO_SCREEN_WIDTH ||
            bullet->actor.y > VIDEO_SCREEN_HEIGHT) {
            bullet->destroy = TRUE;
        }
    }
}

void bullet_release(Bullet* bullet) {
    if (bullet->created) {
        sprite_release(&bullet->actor.sprite);
    }

    bullet->weapon = NULL;

    bullet->actor.w = bullet->actor.h = 0;
}