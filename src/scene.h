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

#ifndef __scene_h
#define __scene_h

#include "resources.h"
#include "image.h"

#include "map.h"
#include "actor.h"
#include "bullet.h"
#include "ship.h"
#include "explosion.h"
#include "data.h"
#include "dialog.h"

#define VISIBLE_TILES   8
#define SCENE_LEFT      16
#define SCENE_TOP       -54
//#define SCENE_SPEED     0.4f
//#define SCENE_SPEED     0.1f

#define SCENE_BOUND_LEFT    7
#define SCENE_BOUND_RIGHT   (VIDEO_SCREEN_WIDTH - SCENE_BOUND_LEFT - 27)
#define SCENE_BOUND_TOP     0
#define SCENE_BOUND_BOTTOM  (VIDEO_SCREEN_HEIGHT - SCENE_BOUND_TOP - 31)

#define SCENE_MAX_ENEMIES       16
#define SCENE_MAX_BULLETS       128
#define SCENE_MAX_EXPLOSIONS    32
#define SCENE_MAX_ITEMS			32

#define SCENE_MAX_SPREAD        16

typedef struct _tile {
    uint16_t    tileId;
    int32_t     x, y;
    uint32_t    explosionTick;
    uint8_t     strength;
    Image       image;
    BOOL        spread;
} Tile;

typedef struct _scene_megabomb {
	BOOL dropped;
	Actor actor;
	double angle;
} SceneMegabomb;

typedef struct _scene {

    Resources   *resources;

    Map *map;
    uint32_t absoluteOffset;
    float offset;
    Image *tileset;

    Tile    	*tiles[VISIBLE_TILES][MAP_WIDTH];
    Ship        pilot;
    Ship        enemies[SCENE_MAX_ENEMIES];
    Bullet      bullets[SCENE_MAX_BULLETS];
    Explosion   explosions[SCENE_MAX_EXPLOSIONS];
    Explosion   groundExplosions[SCENE_MAX_EXPLOSIONS];
    Item 		items[SCENE_MAX_ITEMS];

    SceneMegabomb megabomb;

    void *game;
    void (*on_bullet_update)(void*, Bullet*);
    void (*on_enemy_update)(void*, Ship*);
    void (*on_enemy_appear)(void*, Ship*);
    void (*on_item_update)(void*, Item*);
    void (*on_enemy_shot)(void *, Ship*, uint8_t);
    void (*on_enemy_destroy)(void*, Ship*);
    void (*on_building_destroy)(void*, uint32_t x, uint32_t y);
    void (*on_megabomb)(void *);

} Scene;

BOOL scene_create(Scene* scene, Map* map, Resources *resources, int offset);

void scene_release(Scene* scene);

void scene_update(Scene* scene, float dt);

void scene_render(Scene* scene, Video* video, float xoffset, float yoffset);

Bullet* scene_add_bullet(Scene* scene, const struct _weapon_data* weapon, int32_t x, int32_t y);

Explosion* scene_add_explosion(Scene* scene, uint8_t type, int32_t x, int32_t y, BOOL ground);

//Ship* scene_add_enemy(Scene* scene, uint32_t index);

Item* scene_add_item(Scene* scene, uint8_t type, int32_t x, int32_t y);

Ship* scene_find_enemy_by_position(Scene* scene, int32_t x, int32_t y);
Ship* scene_find_enemy_by_position_and_size(Scene* scene, int32_t x, int32_t y, int32_t w, int32_t h);

void scene_destroy_enemies(Scene* scene, uint32_t energy);

void scene_destroy_buildings(Scene* scene, uint32_t energy);

BOOL scene_drop_megabomb(Scene* scene);

#endif  /* __scene_h */