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

#include "menu.h"
#include "definitions.h"

void menu_create(Menu* menu, Resources* resources, Video* video) {
	int i;

	menu->selectedOption = 0;
	menu->showReturnOption = FALSE;

    menu->logo = resources->images[IMAGE_LOGO];
    menu->background = resources->images[IMAGE_MENU_BACKGROUND];
    menu->imageLabels = resources->images[IMAGE_MENU_LABELS];
    menu->imageLabelsSelected = resources->images[IMAGE_MENU_LABELS_SELECTED];

    for (i = 0; i < MENU_MAX_OPTIONS; i++) {
    	image_create(&menu->labelsSelected[i]);
    	image_create(&menu->labelsUnselected[i]);
    	image_extract(&menu->labelsSelected[i], menu->imageLabelsSelected, 0, MENU_LABEL_HEIGHT * i, MENU_LABEL_WIDTH, MENU_LABEL_HEIGHT);
		image_extract(&menu->labelsUnselected[i], menu->imageLabels, 0, MENU_LABEL_HEIGHT * i, MENU_LABEL_WIDTH, MENU_LABEL_HEIGHT);
    }
}

void menu_render(Menu* menu, Video* video) {
	int i;
	const int options = menu->showReturnOption ? MENU_MAX_OPTIONS : MENU_MAX_OPTIONS - 1;

    image_render(menu->background, 0, 0, video);

    image_render(menu->logo, VIDEO_SCREEN_WIDTH / 2 - MENU_LOGO_WIDTH / 2, 0, video);

    for (i = 0; i < options; i++) {
    	if (menu->selectedOption == i) {
			image_render(&menu->labelsSelected[i], VIDEO_SCREEN_WIDTH / 2 - MENU_LABEL_WIDTH / 2, MENU_LABELS_TOP + i * (MENU_LABEL_HEIGHT-2), video);
    	}
    	else {
			image_render(&menu->labelsUnselected[i], VIDEO_SCREEN_WIDTH / 2 - MENU_LABEL_WIDTH / 2, MENU_LABELS_TOP + i * (MENU_LABEL_HEIGHT-2), video);
		}
    }
}

void menu_release(Menu* menu) {
	int i;

	for (i = 0; i < MENU_MAX_OPTIONS; i++) {
		image_destroy(&menu->labelsSelected[i]);
		image_destroy(&menu->labelsUnselected[i]);
	}
}

void menu_update(Menu* menu, float dt) {

}

void menu_move_down(Menu* menu)	{
	menu->selectedOption++;
	if (menu->showReturnOption) {
		if (menu->selectedOption >= MENU_MAX_OPTIONS) {
			menu->selectedOption = 0;
		}
	}
	else {
		if (menu->selectedOption >= MENU_MAX_OPTIONS - 1) {
			menu->selectedOption = 0;
		}
	}
}

void menu_move_up(Menu* menu)	{
	if (menu->selectedOption == 0) {
		menu->selectedOption = (menu->showReturnOption ? MENU_MAX_OPTIONS - 1 : MENU_MAX_OPTIONS - 2);
	}
	else {
		menu->selectedOption--;
	}
}

int menu_select(Menu* menu)	{
	return menu->selectedOption;
}