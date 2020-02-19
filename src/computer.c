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

#include "computer.h"

void computer_create(Computer* computer, Resources* resources)  {
    int i;

    computer->background = resources->images[IMAGE_FLY_BACKGROUND];
    computer->cursor = resources->images[IMAGE_CURSOR];

    for (i = 0; i < 2; i++) { computer->labelBravo[i] = resources->images[IMAGE_FLY_TEXT_BRAVO + i]; }
    for (i = 0; i < 2; i++) { computer->labelTango[i] = resources->images[IMAGE_FLY_TEXT_TANGO + i]; }
    for (i = 0; i < 2; i++) { computer->labelOuter[i] = resources->images[IMAGE_FLY_TEXT_OUTER + i]; }
    for (i = 0; i < 2; i++) { computer->labelAuto[i] = resources->images[IMAGE_FLY_TEXT_AUTO + i]; }

    computer->selected = COMPUTEROPTION_AUTO;
}

void computer_render(Computer* computer, Video* video)  {
    image_render(computer->background, 0, 0, video);

    image_render(computer->labelBravo[0], COMPUTER_LABEL_POSITIONS[0][0], COMPUTER_LABEL_POSITIONS[0][1], video);
    image_render(computer->labelTango[0], COMPUTER_LABEL_POSITIONS[1][0], COMPUTER_LABEL_POSITIONS[1][1], video);
    image_render(computer->labelOuter[0], COMPUTER_LABEL_POSITIONS[2][0], COMPUTER_LABEL_POSITIONS[2][1], video);
    image_render(computer->labelAuto[0], COMPUTER_LABEL_POSITIONS[3][0], COMPUTER_LABEL_POSITIONS[3][1], video);

    Image* selected = NULL;

    switch (computer->selected) {
        case COMPUTEROPTION_AUTO:
            selected = computer->labelAuto[1];
            break;
        case COMPUTEROPTION_BRAVO:
            selected = computer->labelBravo[1];
            break;
        case COMPUTEROPTION_TANGO:
            selected = computer->labelTango[1];
            break;
        case COMPUTEROPTION_OUTER:
            selected = computer->labelOuter[1];
            break;
    }


    if (selected) {
        image_render(selected, COMPUTER_LABEL_POSITIONS[computer->selected][0],
                     COMPUTER_LABEL_POSITIONS[computer->selected][1], video);
    }

    image_render(computer->cursor, 156, 126, video);
}

void computer_move_down(Computer* computer) {
    if (computer->selected == 3) {
        computer->selected = 0;
    }
    else {
        computer->selected++;
    }
}

void computer_move_up(Computer* computer)   {
    if (computer->selected == 0) {
        computer->selected = 3;
    }
    else {
        computer->selected--;
    }
}

void computer_release(Computer* computer)   {
    // None
}

void computer_update(Computer* computer, float dt)  {
    // Nothing
}