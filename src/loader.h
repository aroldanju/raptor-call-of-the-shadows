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

#ifndef __loader_h
#define __loader_h

#include "game.h"

BOOL load_map(Game* game, uint8_t sector, uint8_t level);

BOOL  load_resource_sound(Game* game, const char *filename);

BOOL  load_resource_music(Game* game, const char *filename);

BOOL load_resource_image(Game* game, const char *filename);

BOOL load_resource_img(Game* game, const char *filename, uint32_t paletteIndex);

BOOL load_resource_font(Game* game, const char *filename);

BOOL load_resource_behaviour(Game* game, const char *filename);

BOOL load_resource_weapon(Game* game, const char *filename) ;

BOOL load_resource_item(Game* game, const char *filename) ;

BOOL load_resource_ship(Game* game, const char *filename) ;

BOOL load_resource_palette(Game* game, const char *filename);

BOOL load_resources(Game* game);


#endif  /* __loader_h */