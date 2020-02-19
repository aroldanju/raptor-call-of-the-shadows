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

#ifndef __hangar_h
#define __hangar_h

#include "video.h"
#include "resources.h"

#include "data.h"

#define HANGAR_ANIMATION_LEFT   84
#define HANGAR_ANIMATION_TOP    117

enum {
    HANGAROPTION_SUPPLY = 0,
    HANGAROPTION_FLY = 1,
    HANGAROPTION_SAVE = 2,
    HANGAROPTION_EXIT = 3
};

// Arrow keys move cursor to positions [SUPPLY ROOM, FLY MISSION, SAVE PILOT, EXIT HANGAR]
static const uint32_t CURSOR_POSITIONS[][2] = {
        { 232, 121 },   // SUPPLY
        { 91, 118 },    // FLY MISSION
        { 225, 50 },    // SAVE PILOT
        { 153, 178 },   // EXIT HANGAR

};

typedef struct _hangar {
    Image       *cursor;
    Image       *background, *animation;
    Image       *labels[4];
    uint32_t    lastTick;
    BOOL        showAnimation;
    uint8_t     selectedOption;
} Hangar;

void hangar_create(Hangar* hangar, Resources* resources);
void hangar_render(Hangar* hangar, Video* video);
void hangar_release(Hangar* hangar);
void hangar_update(Hangar* hangar, float dt);

void hangar_move_left(Hangar* hangar);
void hangar_move_right(Hangar* hangar);

#endif  // __hangar_h