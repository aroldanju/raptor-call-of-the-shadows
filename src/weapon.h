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

#ifndef __weapon_h
#define __weapon_h

#include "defs.h"
#include "sound.h"

#include "definitions.h"

#define MAX_WEAPONDATA_BULLET_FRAMES	4

typedef struct _weapon_data {
    unsigned char magic[3];
    unsigned int itemId;
    unsigned char alwaysEquipped;
    unsigned int delay;
    unsigned char twin;
    unsigned char air;
    unsigned char ground;
    unsigned int behaviour;
    unsigned int damage;
    unsigned short soundId;
    unsigned char bulletFramesNumber;
    unsigned short bulletFrames[MAX_WEAPONDATA_BULLET_FRAMES];
} WeaponData;

typedef struct _weapon {
    const struct _weapon_data *data;
    uint32_t    lastShot;
    Sound       *sound;
} Weapon;

BOOL weapon_can_shot(Weapon* weapon);

BOOL weapondata_load(WeaponData* weapon, const char *data, uint32_t length);

#endif  /* __weapon_h */