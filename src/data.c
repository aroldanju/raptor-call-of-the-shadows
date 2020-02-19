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

#include <memory.h>
#include "data.h"

void data_create(Data* data)    {
    int i;

    data->numBehaviour = 0;
    for (i = 0; i < DATA_MAX_BEHAVIOUR; i++) {
        data->behaviour[i] = NULL;
    }

    data->numWeapons = 0;
    for (i = 0; i < DATA_MAX_WEAPONS; i++) {
        data->weapons[i] = NULL;
    }

    data->numShips = 0;
    for (i = 0; i < DATA_MAX_SHIPS; i++) {
        data->ships[i] = NULL;
    }

    data->numItems = 0;
    for (i = 0; i < DATA_MAX_ITEMS; i++) {
        data->items[i] = NULL;
    }

    data->numTiles = 0;
}

void data_add_behaviour(Data* data, Behaviour* behaviour)   {
    data->behaviour[data->numBehaviour] = behaviour;
    data->numBehaviour++;
}

void data_add_weapon(Data* data, WeaponData* weapon)   {
    data->weapons[data->numWeapons] = weapon;
    data->numWeapons++;
}


void data_add_ship(Data* data, ShipData* ship)   {
    data->ships[data->numShips] = ship;
    data->numShips++;
}


void data_add_tile(Data* data, TileData* tile)  {
    memcpy(&data->tiles[data->numTiles], tile, sizeof(TileData));
    data->numTiles++;
}

void data_add_item(Data* data, ItemData* item)  {
    data->items[data->numItems] = item;
    data->numItems++;
}

void data_release(Data* data)   {

    int i;

    for (i = 0; i < DATA_MAX_BEHAVIOUR; i++) {
        if (data->behaviour[i] != NULL) {
            behaviour_release(data->behaviour[i]);
            free(data->behaviour[i]);
        }
    }
    data->numBehaviour = 0;

    for (i = 0; i < DATA_MAX_WEAPONS; i++) {
        if (data->weapons[i] != NULL) {
            //weapon_release(data->behaviour[i]);
            free(data->weapons[i]);
        }
    }
    data->numWeapons = 0;

    for (i = 0; i < DATA_MAX_SHIPS; i++) {
        if (data->ships[i] != NULL) {
            free(data->ships[i]);
        }
    }
    data->numShips = 0;

    for (i = 0; i < DATA_MAX_ITEMS; i++) {
        if (data->items[i] != NULL) {
            free(data->items[i]);
        }
    }
    data->numItems = 0;

    data->numTiles = 0;
}