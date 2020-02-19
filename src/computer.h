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

#ifndef __computer_h
#define __computer_h

#include "resources.h"
#include "definitions.h"

// TODO add bottom buttons

enum {
    COMPUTEROPTION_BRAVO = 0,
    COMPUTEROPTION_TANGO = 1,
    COMPUTEROPTION_OUTER = 2,
    COMPUTEROPTION_AUTO = 3
};

static const uint32_t COMPUTER_LABEL_POSITIONS[4][2] = {
        { 45, 39 }, { 45, 68 }, { 45, 97 }, { 45, 126 }
};

typedef struct _computer {
    Image       *cursor;
    Image *background;
    Image *labelBravo[2];
    Image *labelTango[2];
    Image *labelOuter[2];
    Image *labelAuto[2];

    uint8_t selected;
} Computer;

void computer_create(Computer* computer, Resources* resources);

void computer_render(Computer* computer, Video* video);

void computer_move_down(Computer* computer);

void computer_move_up(Computer* computer);

void computer_release(Computer* computer);

void computer_update(Computer* computer, float dt);

#endif  // __computer_h