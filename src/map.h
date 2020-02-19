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

#ifndef __map_h
#define __map_h

#include "defs.h"

#define MAP_WIDTH   9
#define MAP_HEIGHT  150

#define MAP_MAX_ENEMIES 350

typedef struct _tile_data {
    uint16_t gid;
    uint16_t exchange;
    uint32_t strength;
    uint32_t score;
} TileData;

struct _map_enemy {
    int16_t type;
    uint8_t x;
    uint8_t y;
    uint8_t behavior;
    uint8_t difficulty;
	int8_t  item;
	uint8_t gang;
	uint8_t launched;
	uint8_t boss;
};

struct _map_tile {
    uint16_t gid;
    struct _map_enemy enemy;
};

typedef struct _map {
	// TODO uint8_t tileset;
    //struct _map_enemy enemies[MAP_MAX_ENEMIES];
    //uint16_t tiles[MAP_HEIGHT][MAP_WIDTH];

    struct _map_tile tiles[MAP_HEIGHT][MAP_WIDTH];
} Map;

void map_reset(Map* map);

BOOL map_load(Map* map, const char* data, uint32_t length);

BOOL tiledata_load(TileData* ship, uint32_t *size, const char *data, uint32_t length);

#endif  /* __map_h */