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

#ifndef __newpilot_h
#define __newpilot_h

#include "label.h"
#include "video.h"
#include "resources.h"

#include "data.h"
#include "pilot.h"

typedef struct _new_pilot {
    Image       *background;
    Ttf         *font;
    Label       labelName, labelCallsign;
    Image       *pictures[4];
    uint8_t     selectedLine;
    Pilot       *pilot;
} NewPilot;

void newpilot_create(NewPilot* newPilot, Pilot* pilot, Resources* resources);

void newpilot_render(NewPilot* newPilot, Video* video);

void newpilot_release(NewPilot* newPilot);

void newpilot_update(NewPilot* newPilot, float dt);

void newpilot_next_picture(NewPilot* p);

void newpilot_text(NewPilot* p, char *text);

void newpilot_next_line(NewPilot* p);

void newpilot_remove(NewPilot* p);

BOOL newpilot_start(NewPilot* p);

#endif  /* __newpilot_h */