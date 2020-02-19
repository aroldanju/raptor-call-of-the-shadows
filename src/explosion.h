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

#ifndef __explosion_h
#define __explosion_h

#include "actor.h"
#include "resources.h"

enum {
    EXPLOSION_FLARE,
    EXPLOSION_BUILDING,
    EXPLOSION_SPARKLE,
    EXPLOSION_BSPARK,
    EXPLOSION_OSPARK,
    EXPLOSION_SHIP,
    EXPLOSION_HIT,
    EXPLOSION_LARGE,
    EXPLOSION_BONUS,
    EXPLOSION_EXTRALARGE
};

typedef struct _explosion {
    Actor   actor;
    BOOL    destroy, created;
} Explosion;

void explosion_create(Explosion* explosion, uint8_t type, Resources* resources);

void explosion_release(Explosion* explosion);

void explosion_render(Explosion* explosion, Video* video);

void explosion_update(Explosion* explosion, float dt);

uint32_t explosion_get_size(uint8_t type);

#endif  // __explosion_h