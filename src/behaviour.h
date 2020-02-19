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

#ifndef __behaviour_h
#define __behaviour_h

#include <stdint.h>
#include "defs.h"
//#include "scene.h"

enum {
    BEHAVIOURCOMMAND_MOVEX,
    BEHAVIOURCOMMAND_MOVEY,
    BEHAVIOURCOMMAND_SHOT,
    BEHAVIOURCOMMAND_DELAY,
    BEHAVIOURCOMMAND_MOVETARGET,
    BEHAVIOURCOMMAND_GOTO,
    BEHAVIOURCOMMAND_SET_ANGLE,
    BEHAVIOURCOMMAND_SET_VELOCITY
};

struct _behaviour_step {
    uint8_t     command;

    // Movement stuff
    float 		xvel, yvel;
    float       maxxvel, maxyvel;

    // Shooting stuff
    uint8_t 	weapon, x, y;
    BOOL        target;

    // Pause
    uint32_t 	delay;

    float angle;

    // General purpose data
    uint32_t data1, data2;
};

typedef struct _behaviour {
    uint32_t 	id;
    uint8_t 	numSteps;
    uint8_t 	infinite;
    struct _behaviour_step *steps;
} Behaviour;

typedef struct _behaviourable {
    Behaviour   *behaviour;
    uint32_t    lastBehaviourTick;
    uint8_t     currentBehaviourStep;
    struct _behaviour_step *lastBehaviourStep;
} Behaviourable;

BOOL behaviour_load(Behaviour* behaviour, const char *data, uint32_t length);

void behaviour_release(Behaviour* behaviour);

void behaviour_reset(Behaviourable* behaviourable);

//void behaviour_run_step(Behaviourable *behaviourable, Scene *scene);

#endif  // __behaviour_h