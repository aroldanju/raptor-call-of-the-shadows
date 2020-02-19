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

#include "behaviour.h"

#include <stdlib.h>
#include <memory.h>

BOOL behaviour_load(Behaviour* behaviour, const char *data, uint32_t length)    {

    behaviour->id = *(uint32_t*)(data);
    behaviour->numSteps = *(uint8_t*)(data + 4);
    behaviour->infinite = *(uint8_t*)(data + 5);

    behaviour->steps = (struct _behaviour_step*)malloc(sizeof(struct _behaviour_step) * behaviour->numSteps);
    memcpy(behaviour->steps, data + 6, length - 6);

    return TRUE;
}

void behaviour_release(Behaviour* behaviour) {
    free(behaviour->steps);
    behaviour->steps = NULL;
    behaviour->numSteps = 0;
}

void behaviour_reset(Behaviourable* behaviourable) {
    behaviourable->lastBehaviourTick = 0;
    behaviourable->currentBehaviourStep = 0;
    behaviourable->lastBehaviourStep = NULL;
    behaviourable->behaviour = NULL;
}

/*
void behaviour_run_step(Behaviourable *behaviourable, Scene *scene) {
    if (!behaviourable) {
#ifdef __DEBUG__
        printf("behaviour_run_step(): invalid behaviour\n");
#endif
        return;
    }

    const Behaviour* behaviour = behaviourable->behaviour;

    // Reached end of behaviour
    if (behaviour->infinite == 0 && behaviourable->currentBehaviourStep >= behaviour->numSteps) {
        return;
    }

    // Restart behaviour steps if the last step has been reached
    if (behaviour->infinite == 1 && behaviourable->currentBehaviourStep >= behaviour->numSteps) {
        behaviourable->currentBehaviourStep = 0;
    }

    const struct _behaviour_step *step = &behaviour->steps[behaviourable->currentBehaviourStep];

    switch(step->command) {
        case BEHAVIOURCOMMAND_SHOT:
            printf("behaviour_run_step(): shot %d, %d, %d, %d\n", step->weapon, step->x, step->y, step->target);
            ship->on_shot(ship->game, ship, step->weapon);
            ship->behaviour.lastBehaviourTick = SDL_GetTicks();
            break;

        case BEHAVIOURCOMMAND_MOVEX:
            //printf("run_behaviour_step(): %u movex %.2f, %.2f\n", behaviour, step->xvel, step->maxxvel);
            if (step->maxxvel != 0.0f) {
                ship->actor.xVel = (step->xvel * 100.0f + (rand() % (int)(step->maxxvel * 100.0f - step->xvel * 100.0f))) / 100.0f;
            }
            else {
                //ship->actor.xVel = GAME_SCENE_SPEED * step->xvel;
                ship->actor.xVel = step->xvel;
            }

            ship->behaviour.lastBehaviourTick = SDL_GetTicks();

            break;

        case BEHAVIOURCOMMAND_MOVEY:
            //printf("run_behaviour_step(): %u movey %.2f, %.2f\n", behaviour, step->yvel, step->maxyvel);
            if (step->maxyvel != 0.0f) {
                ship->actor.yVel = (step->yvel * 100.0f + (rand() % (int)(step->maxyvel * 100.0f - step->yvel * 100.0f))) / 100.0f;
            }
            else {
                //ship->actor.yVel = GAME_SCENE_SPEED * step->yvel;
                ship->actor.yVel = step->yvel;
            }

            ship->behaviour.lastBehaviourTick = SDL_GetTicks();

            break;

        case BEHAVIOURCOMMAND_DELAY:
            //printf("run_behaviour_step(): %u delay %d\n", behaviour, step->delay);
            if (SDL_GetTicks() - behaviourable->lastBehaviourTick < step->delay) {
                return;
            }
            break;
    }

    ship->behaviour.lastBehaviourStep = step;
    ship->behaviour.currentBehaviourStep++;

    return run_behvaiour_step(ship);
}
*/
