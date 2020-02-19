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

#ifndef __ship_h
#define __ship_h

#include "resources.h"

#include "actor.h"
#include "weapon.h"
#include "behaviour.h"
#include "map.h"
#include "explosion.h"

#define MAX_SHIP_HOTS		2
#define MAX_SHIP_FRAMES		16

typedef struct _ship_hot {
    uint8_t x, y, w, h;
} ShipHot;

typedef struct _ship_data {
    unsigned char magic[3];
    uint32_t 	energy;
    uint8_t     ground;
    uint8_t 	numFrames;
    uint16_t 	frames[MAX_SHIP_FRAMES];
    uint32_t 	score;
    uint8_t 	hotNumber;
    ShipHot     hots[MAX_SHIP_HOTS];
} ShipData;

#define SHIP_MAX_WEAPONS    	16
//#define SHIP_MAX_EXPLOSIONS		16

typedef struct _ship {
    Actor       actor;
    //Weapon      weapon;
    BOOL        destroy;
    BOOL        created;

    /*
    Behaviour   *behaviour;
    uint32_t    lastBehaviourTick;
    uint8_t     currentBehaviourStep;
    struct _behaviour_step *lastBehaviourStep;
    */
    Behaviourable behaviour;

    // Available weapons
    uint32_t    numWeapons;
    Weapon      weapons[SHIP_MAX_WEAPONS];

    // Current selected selectable weapon
    Weapon      *selectedWeapon;
    uint8_t     selectedWeaponIndex;

    void (*on_shot)(void* game, struct _ship* ship, uint8_t weaponId);
    void *game;

    uint32_t    energy;
    const ShipData    *data;
    uint32_t 	lastCollisionTick;

    BOOL 		damaged;
    BOOL 		burstingUp;
    uint32_t 	burstingUpTick;

    //Explosion 	explosions[SHIP_MAX_EXPLOSIONS];
    uint32_t 	destroyTick;

    struct _map_enemy* mapEnemy;

    struct _ship *target;

    Image *lastFrame;
    Image shadow;

} Ship;

void ship_create_empty(Ship* ship);

void ship_update(Ship* ship, float dt);

BOOL ship_create_from_enemy(Ship* ship, const struct _map_enemy* enemy, Resources* resources);

void ship_render(Ship* ship, Video* video);

void ship_release(Ship* ship);

void ship_add_weapon(Ship* ship, WeaponData* weapon, Resources* resources);

void ship_change_weapon(Ship* ship);

void ship_remove_energy(Ship* ship, uint32_t damage);

BOOL shipdata_load(ShipData* ship, const char *data, uint32_t length);

void ship_damage(Ship* ship);

void ship_burstup(Ship* ship);

//void ship_add_explosion(Ship* ship, uint8_t type);

uint32_t ship_get_width(Ship *ship);

uint32_t ship_get_height(Ship* ship);

#endif  // __ship_h