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

#include "hangar.h"
#include "definitions.h"

void hangar_create(Hangar* hangar, Resources* resources)  {
    int i;

    hangar->background = resources->images[IMAGE_HANGAR_START];
    hangar->animation = resources->images[IMAGE_HANGAR_START + 1];
    hangar->cursor = resources->images[IMAGE_CURSOR];
    hangar->selectedOption = 0;
    hangar->showAnimation = TRUE;

    for (i = 0; i < 4; i++) {
        hangar->labels[i] = resources->images[IMAGE_HANGAR_TEXT_START + i];
    }
}

void hangar_render(Hangar* hangar, Video* video)    {
    // Background
    image_render(hangar->background, 0, 0, video);
    if (hangar->showAnimation) {
        image_render(hangar->animation, HANGAR_ANIMATION_LEFT, HANGAR_ANIMATION_TOP, video);
    }

    // Cursor
    image_render(hangar->cursor, CURSOR_POSITIONS[hangar->selectedOption][0], CURSOR_POSITIONS[hangar->selectedOption][1], video);

    // Label
    image_render(hangar->labels[hangar->selectedOption], 93, 185, video);
}

void hangar_release(Hangar* hangar) {

}

void hangar_update(Hangar* hangar, float dt)    {
    if (SDL_GetTicks() - hangar->lastTick >= 50) {
        hangar->lastTick = SDL_GetTicks();
        if (rand() % 3 == 0) {
            hangar->showAnimation = !hangar->showAnimation;
        }
    }
}

void hangar_move_left(Hangar* hangar)   {
    if (hangar->selectedOption == 0) {
        hangar->selectedOption = 3;
    }
    else {
        hangar->selectedOption--;
    }
}

void hangar_move_right(Hangar* hangar){
    if (hangar->selectedOption == 3) {
        hangar->selectedOption = 0;
    }
    else {
        hangar->selectedOption++;
    }
}
