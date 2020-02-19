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

#include <stdint.h>
#include <memory.h>

#include "map.h"

#ifdef __DEBUG__
#	include <stdio.h>
#endif


void map_reset(Map* map)    {
    int i, j;

    for (i = 0; i < MAP_HEIGHT; i++) {
        for (j = 0; j < MAP_WIDTH; j++) {
            map->tiles[i][j].gid = 0;
            map->tiles[i][j].enemy.type = -1;
            map->tiles[i][j].enemy.x = 0;
            map->tiles[i][j].enemy.y = 0;
            map->tiles[i][j].enemy.behavior = 0;
            map->tiles[i][j].enemy.launched = 0;
            map->tiles[i][j].enemy.gang = 0;
            map->tiles[i][j].enemy.difficulty = 0;
            map->tiles[i][j].enemy.boss = 0;
        }
    }

    /*
    for (i = 0; i < MAP_MAX_ENEMIES; i++) {
        map->enemies[i].type = 0;
        map->enemies[i].x = 0;
        map->enemies[i].y = 0;
        map->enemies[i].behavior = 0;
        map->enemies[i].difficulty = 0;
        map->enemies[i].gang = 0;
        map->enemies[i].launched = 0;
    }

    for (i = 0; i < MAP_HEIGHT; i++) {
        for (j = 0; j < MAP_WIDTH; j++) {
            map->tiles[i][j] = 0;
        }
    }
    */
}

BOOL map_load(Map* map, const char* data, uint32_t length)   {
    if (length != sizeof(Map)) {
        return FALSE;
    }

    memcpy(map, data, length);

    return TRUE;
}

BOOL tiledata_load(TileData* tiles, uint32_t *size, const char *data, uint32_t length)  {

    if (data[0] != 'T' || data[1] != 'I' || data[2] != 'L') {
#ifdef __DEBUG__
        printf("tiledata_load(): invalid tiles file (invalid magic)\n");
#endif
        return FALSE;
    }

    memcpy(size, (data + 3), sizeof(uint32_t));

    if (length - 3 - sizeof(uint32_t) != sizeof(TileData) * (*size)) {
#ifdef __DEBUG__
        printf("tiledata_load(): invalid tiles file (invalid size)\n");
#endif
        return FALSE;
    }

    int i;

    for (i = 0; i < (*size); i++) {
    	uint16_t gid, exchange;
    	uint32_t strength, score;

    	memcpy(&gid, (data + 3 + sizeof(uint32_t) + (sizeof(TileData) * i) + 0), sizeof(gid));
    	memcpy(&exchange, (data + 3 + sizeof(uint32_t) + (sizeof(TileData) * i) + sizeof(gid)), sizeof(exchange));
    	memcpy(&strength, (data + 3 + sizeof(uint32_t) + (sizeof(TileData) * i) + sizeof(gid) + sizeof(exchange)), sizeof(strength));
    	memcpy(&score, (data + 3 + sizeof(uint32_t) + (sizeof(TileData) * i) + sizeof(gid) + sizeof(exchange) + sizeof(strength)), sizeof(score));

    	tiles[i].gid = gid;
    	tiles[i].exchange = exchange;
    	tiles[i].strength = strength;
    	tiles[i].score = score;
	}

    return TRUE;
}
