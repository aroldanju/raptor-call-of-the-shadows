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

#ifndef __hud_h
#define __hud_h

#include "image.h"
#include "video.h"
#include "pilot.h"
#include "ship.h"

#define HUD_SHIELD_LEFT        8
#define HUD_SHIELD_TOP         1
#define HUD_SHIELD_WIDTH       4

#define HUD_CREDITS_LEFT    120
#define HUD_CREDITS_TOP     2

#define HUD_CREDIT_FONT_SIZE    8

#define HUD_BOMBS_LEFT       20
#define HUD_BOMBS_TOP        188
#define HUD_SHIELDS_LEFT     21
#define HUD_SHIELDS_TOP      4

#define HUD_WEAPON_LEFT     280
#define HUD_WEAPON_TOP      8

static const uint8_t HUD_SHIELDS_COLORS[9][3] = {
        {101, 16, 8},
        {113, 24, 12},
        {130, 36, 16},
        {146, 44, 20},
        {162, 56, 28},
        {174, 69, 36},
        {190, 85, 44},
        {207, 97, 52},
        {223, 113, 60}
};

static const uint8_t HUD_IONSCANNER_COLORS[2][3] = { {188, 84, 44}, {100, 16, 8} };

#define HUD_IONSCANNER_LEFT			110
#define HUD_IONSCANNER_TOP			190
#define HUD_IONSCANNER_WIDTH		102
#define HUD_IONSCANNER_HEIGHT		8

typedef struct _hud_scanner {
	float 	position;
	BOOL 	enabled;
	float 	percent;
} HudScanner;

typedef struct _hud {
    Pilot   *pilot;
    Ship    *ship;
    Image   *fontCredits;
    Image   credits[10];
    Image   *bomb, *shield;
    Image   *items;
    Image   selectedWeapon;
    HudScanner scanner;
} Hud;

void hud_create(Hud* hud, Pilot* pilot, Resources* resources);

void hud_render(Hud* hud, Video* video);

void hud_on_credits_change(Hud* hud);

void hud_on_weapon_change(Hud* hid, Weapon* weapon);

void hud_release(Hud* hud);

#endif  /* __hud_h */