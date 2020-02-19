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

#ifndef __bullet_h
#define __bullet_h

#include "resources.h"
#include "actor.h"
#include "ship.h"

typedef struct _bullet {
    Actor actor;

    Ship    *owner;
    Ship    *target;

    uint8_t width, height;
    //int32_t targetX, targetY;
    BOOL    destroy;
    BOOL    created;
    //uint8_t     damage;

    Behaviourable behaviour;
    const struct _weapon_data *weapon;
} Bullet;

void bullet_create(Bullet* bullet, const struct _weapon_data* weapon, Resources* resources);
void bullet_render(Bullet* bullet, Video* video);
void bullet_update(Bullet* bullet, float dt);
void bullet_release(Bullet* bullet);

#endif  // __bullet_h