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

#include "weapon.h"

#include <SDL2/SDL_timer.h>

BOOL weapon_can_shot(Weapon* weapon) {
    return (SDL_GetTicks() - weapon->lastShot >= weapon->data->delay);
}


BOOL weapondata_load(WeaponData* weapon, const char *data, uint32_t length) {
    if (length != sizeof(WeaponData)) {
        return FALSE;
    }

    if (data[0] != 'W' || data[1] != 'P' || data[2] != 'N') {
        return FALSE;
    }

    memcpy(weapon, data, length);

    return TRUE;
}
