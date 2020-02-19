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

#ifndef __menu_h
#define __menu_h

#include "video.h"
#include "resources.h"
#include "data.h"

#define MENU_MAX_OPTIONS 7

#define MENU_LABELS_TOP		89
#define MENU_LABEL_HEIGHT	16
#define MENU_LABEL_WIDTH	150
#define MENU_LOGO_WIDTH		264

enum {
	MENUOPTION_NEWMISSION,
	MENUOPTION_LOADMISSION,
	MENUOPTION_GAMEOPTIONS,
	MENUOPTION_ORDERINFO,
	MENUOPTION_CREDITS,
	MENUOPTION_QUIT,
	MENUOPTION_RETURNTOGAME
};

typedef struct _menu {
	Image 	*logo;
    Image   *background;
    Image 	*imageLabels, *imageLabelsSelected;
    Image 	labelsSelected[MENU_MAX_OPTIONS], labelsUnselected[MENU_MAX_OPTIONS];
    uint8_t selectedOption;
    BOOL 	showReturnOption;
} Menu;

void menu_create(Menu* menu, Resources* resources, Video* video);
void menu_render(Menu* menu, Video* video);
void menu_release(Menu* menu);
void menu_update(Menu* menu, float dt);

void menu_move_down(Menu* menu);
void menu_move_up(Menu* menu);
int menu_select(Menu* menu);

#endif  /* __menu_h */