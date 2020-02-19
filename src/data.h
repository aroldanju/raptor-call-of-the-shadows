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

#ifndef __data_h
#define __data_h

#include "behaviour.h"
#include "weapon.h"
#include "map.h"
#include "ship.h"
#include "item.h"

#define DATA_MAX_BEHAVIOUR      128
#define DATA_MAX_WEAPONS        16
#define DATA_MAX_SHIPS          16
#define DATA_MAX_TILES          256
#define DATA_MAX_ITEMS          32

// TODO add id

typedef struct _data {
    uint32_t        numBehaviour;
    Behaviour       *behaviour[DATA_MAX_BEHAVIOUR];

    uint32_t        numWeapons;
    WeaponData      *weapons[DATA_MAX_WEAPONS];

    uint32_t        numShips;
    ShipData        *ships[DATA_MAX_SHIPS];

    uint32_t        numTiles;
    TileData        tiles[DATA_MAX_TILES];

    uint32_t        numItems;
    ItemData        *items[DATA_MAX_ITEMS];

} Data;

void data_create(Data* data);

void data_add_behaviour(Data* data, Behaviour* behaviour);

void data_add_weapon(Data* data, WeaponData* weapon);

void data_add_ship(Data* data, ShipData* ship);

void data_add_tile(Data* data, TileData* tile);

void data_add_item(Data* data, ItemData* item);

void data_release(Data* data);

extern Data G_DATA;

#endif  /* __data_h */