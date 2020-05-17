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

#include "pilot.h"
#include "definitions.h"

#include <stdio.h>
#include <memory.h>

BOOL pilot_save(Pilot* pilot) {
    char filename[32];
    FILE *file;
    uint32_t index = 0;

    sprintf(filename, "%s/SAVE%04d.SAV", DATA_PATH, index);

    file = fopen(filename, "wb");
    if (!file) {
#ifdef __DEBUG__
        printf("pilot_save(): error saving pilot to %s\n", filename);
#endif
        return FALSE;
    }

    fwrite(pilot, sizeof(Pilot), 1, file);

    fclose(file);

    return TRUE;
}

BOOL pilot_load(Pilot* pilot, uint32_t index)   {
    char filename[32];
    FILE *file;

    sprintf(filename, "%s/SAVE%04d.SAV", DATA_PATH, index);

    file = fopen(filename, "rb");
    if (!file) {
#ifdef __DEBUG__
        printf("pilot_load(): error loading pilot to %s\n", filename);
#endif
        return FALSE;
    }

    fread(pilot, sizeof(Pilot), 1, file);

    fclose(file);

    return TRUE;
}

void pilot_create(Pilot* pilot, const char *name, const char* callsign) {
    memcpy(pilot->name, name, PILOT_NAME_LENGTH);
    memcpy(pilot->callsign, callsign, PILOT_CALLSIGN_LENGTH);

}

void pilot_startup(Pilot* pilot) {
    int i;

    pilot->difficulty = DIFFICULTY_ROOKIE;
    pilot->cash = PILOT_CASH_STARTUP;
    pilot->energy = PILOT_ENERGY_STARTUP;
    pilot->shield = PILOT_SHIELD_STARTUP;
    pilot->level[0] = PILOT_WAVE_STARTUP;
    pilot->level[1] = PILOT_WAVE_STARTUP;
    pilot->level[2] = PILOT_WAVE_STARTUP;
    pilot->bombs = PILOT_BOMBS_STARTUP;
	pilot->shield = PILOT_SHIELD_STARTUP;
    pilot->picture = 0;
    pilot->shields = PILOT_SHIELDS_STARTUP;
    strcpy(pilot->name, "");
    strcpy(pilot->callsign, "");

    for (i = 0; i < DATA_MAX_ITEMS; i++) {
        pilot->items[i] = 0;

        if (i == ITEM_ENERGYMODULE) {
            pilot->items[i] = pilot->energy;
        }

        else if (i == ITEM_MACHINEGUN) {
            pilot->items[i] = 1;
        }

        else if (i == ITEM_MEGABOMB) {
            pilot->items[i] = pilot->bombs;
        }
        /*
		else if (i == ITEM_IONSCANNER) {
			pilot->items[i] = 1;
		}
		*/
        /*
        else if (i == ITEM_PULSECANNON) {
            pilot->items[i] = 1;
        }
        */
    }

}

void pilot_add_cash(Pilot* pilot, const uint32_t cash)   {
    pilot->cash += cash;
}

void pilot_remove_cash(Pilot* pilot, uint32_t cash)    {
    if (cash >= pilot->cash) {
        cash = pilot->cash;
    }

    pilot->cash -= cash;
}

BOOL pilot_has_cash(const Pilot* pilot, const uint32_t cash)  {
    return pilot->cash >= cash;
}

void pilot_hit(Pilot* pilot, uint32_t energy)   {

	if (pilot->shields > 0) {
		if (pilot->shield == 0) {
			pilot->shield = PILOT_MAX_SHIELD;
		}

		if (pilot->shield < energy) {
			const int diff = energy - pilot->shield;
			pilot->shield = 0;
			pilot->shields--;
			pilot->items[ITEM_PHASESHIELD] = pilot->shields;
			return pilot_hit(pilot, diff);
		}
		else {
			pilot->shield -= energy;
			return;
		}
	}

	if (pilot->energy < energy) {
		pilot->energy = 0;
		return;
	}

	pilot->energy -= energy;

	// Update energy item
	pilot->items[ITEM_ENERGYMODULE] = pilot->energy;
}

void pilot_add_item(Pilot* pilot, uint32_t id, ItemData* item)	{
	pilot->items[id] += item->pack;

	if (pilot->items[id] > item->maxAmount) {
		pilot->items[id] = item->maxAmount;
	}
}

void pilot_add_shield(Pilot* pilot, uint32_t shield)    {
	if (pilot->shield + shield > PILOT_MAX_SHIELD) {
		int diff = (pilot->shield + shield) - PILOT_MAX_SHIELD;
		pilot->shields++;
		pilot->shield = diff;
	}
	else {
		pilot->shield += shield;
		if (pilot->shield > PILOT_MAX_SHIELD) {
			pilot->shield = PILOT_MAX_SHIELD;
		}

		if (pilot->shields == 0) {
			pilot->shields++;
		}
	}
}

void pilot_add_energy(Pilot* pilot, uint32_t energy)    {
    pilot->energy += energy;
    if (pilot->energy > PILOT_MAX_ENERGY) {
        pilot->energy = PILOT_MAX_ENERGY;
    }
}

/*
void pilot_remove_shield(Pilot* pilot, uint32_t shield) {
    if (pilot->shield < shield) {
        shield = pilot->shield;
    }

    pilot->shield -= shield;
}

void pilot_remove_energy(Pilot* pilot, uint32_t energy) {
    if (pilot->energy < energy) {
        energy = pilot->energy;
    }

    pilot->energy -= energy;
}
*/

void pilot_get_next_wave(const Pilot* pilot, uint8_t* sector, uint8_t *wave)    {

    int i;

    for (i = 0; i < 4; i++) {
    	// Check if some sector is not completed => <10
        if (pilot->level[i] < 10) {
            *sector = (i + 1);
            *wave = pilot->level[i];
            return;
        }
    }
}
