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

#include "actor.h"

void actor_render(Actor* actor, Video *video)   {
    sprite_render(&actor->sprite, video, actor->x, actor->y);
}

void actor_update(Actor* actor, float dt) {
    if (actor->targeted || actor->angled) {
        actor->x += ((cos(actor->angle) * actor->xVel) * dt);
        actor->y += ((sin(actor->angle) * actor->yVel) * dt);
    }
    else {
        actor->x += (actor->xVel * dt);
        actor->y += (actor->yVel * dt);
    }

    sprite_update(&actor->sprite);
}

