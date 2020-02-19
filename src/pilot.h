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

#ifndef __pilot_h
#define __pilot_h

#include "defs.h"

#include "definitions.h"
#include "data.h"

#include <stdint.h>

#define PILOT_NAME_LENGTH       17   // 16 + \0
#define PILOT_CALLSIGN_LENGTH   12   // 11 + \0

// TODO: move to config file
#define PILOT_CASH_STARTUP      10000
#define PILOT_ENERGY_STARTUP    75
#define PILOT_SHIELD_STARTUP    0
//#define PILOT_SECTOR_STARTUP    1
#define PILOT_WAVE_STARTUP      1
#define PILOT_SHIELDS_STARTUP   0
#define PILOT_BOMBS_STARTUP     1

// This limits are specified in ITM files
//#define PILOT_MAX_SHIELDS       5
//#define PILOT_MAX_BOMBS         5

#define PILOT_MAX_SHIELD        100
#define PILOT_MAX_ENERGY        100

enum {
    PILOTDIFFICULTY_TRAINING,
    PILOTDIFFICULTY_ROOKIE,
    PILOTDIFFICULTY_VETERAN,
    PILOTDIFFICULTY_ELITE
};

typedef struct _pilot {
    // Profile
    char        name[PILOT_NAME_LENGTH];
    char        callsign[PILOT_CALLSIGN_LENGTH];
    char        picture;
    char        difficulty;

    // Game
    uint32_t    cash;
    uint32_t    energy;
    uint32_t    shield;
    //uint8_t     sector;
    //uint8_t     wave;

    uint8_t     bombs;
    uint8_t     shields;

    // [SECTOR] wave
    uint8_t     level[4];

    uint32_t    items[DATA_MAX_ITEMS];

} Pilot;

BOOL pilot_save(Pilot* pilot);

BOOL pilot_load(Pilot* pilot, uint32_t index);

void pilot_startup(Pilot* pilot);

void pilot_create(Pilot* pilot, const char *name, const char* callsign);

void pilot_add_cash(Pilot* pilot, const uint32_t cash);

void pilot_remove_cash(Pilot* pilot, const uint32_t cash);

BOOL pilot_has_cash(const Pilot* pilot, const uint32_t cash);

void pilot_hit(Pilot* pilot, uint32_t energy);

void pilot_add_shield(Pilot* pilot, uint32_t shield);

void pilot_add_energy(Pilot* pilot, uint32_t energy);

/*
void pilot_remove_shield(Pilot* pilot, uint32_t shield);

void pilot_remove_energy(Pilot* pilot, uint32_t energy);
*/

void pilot_get_next_wave(const Pilot* pilot, uint8_t* sector, uint8_t *wave);

void pilot_add_item(Pilot* pilot, uint32_t id, ItemData* item);

#endif  // __pilot_h