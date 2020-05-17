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

#ifndef __actor_h
#define __actor_h

#include "sprite.h"

typedef struct _actor {
    float x, y;
    float xVel;
    float yVel;
    Sprite sprite;
    int w, h;
    float angle;
    BOOL targeted;
    BOOL angled;
    BOOL targetFound; // -1: looking for | 0: found | 1: not found
} Actor;

void actor_render(Actor* actor, Video *video);
void actor_update(Actor* actor, float dt);

#endif  /* __actor_h */