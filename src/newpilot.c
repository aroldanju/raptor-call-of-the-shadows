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

#include "newpilot.h"
#include "definitions.h"

void newpilot_create(NewPilot* newPilot, Pilot* pilot, Resources* resources) {
    int i;

    pilot_create(pilot, "", "");

    newPilot->background = resources->images[IMAGE_NEWPILOT_BACKGROUND];
    newPilot->font = resources->fonts[FONT_DEFAULT];
    newPilot->selectedLine = 0;
    newPilot->pilot = pilot;
    newPilot->pilot->name[0] = '\0';
    newPilot->pilot->callsign[0] = '\0';
    for (i = 0; i < 4; i++) {
        newPilot->pictures[i] = resources->images[IMAGE_MIN_PILOT_START + i];
    }
    label_create(&newPilot->labelName, newPilot->font);
    label_setColor(&newPilot->labelName, 0, 0, 0);
    label_create(&newPilot->labelCallsign, newPilot->font);
    label_setColor(&newPilot->labelCallsign, 0, 0, 0);
}

void newpilot_render(NewPilot* newPilot, Video* video) {
    image_render(newPilot->background, 0, 0, video);

    label_render(&newPilot->labelName, video,187, 133);
    label_render(&newPilot->labelCallsign, video,203, 142);

    if (newPilot->selectedLine == 0) {
        video_render_line(video, 187 + label_get_width(&newPilot->labelName), 133, 187 + label_get_width(&newPilot->labelName), 138, 0, 0, 0);
    }
    else {
        video_render_line(video, 203 + label_get_width(&newPilot->labelCallsign), 142, 203 + label_get_width(&newPilot->labelCallsign), 148, 0, 0, 0);
    }

    image_render(newPilot->pictures[newPilot->pilot->picture], 5, 108, video); // 7, 115
}

void newpilot_release(NewPilot* newPilot) {
    label_destroy(&newPilot->labelName);
    label_destroy(&newPilot->labelCallsign);
}

void newpilot_update(NewPilot* newPilot, float dt) {

}

void newpilot_next_picture(NewPilot* p) {
    if (p->pilot->picture == 3) {
        p->pilot->picture = 0;
    }
    else {
        p->pilot->picture++;
    }
}

void newpilot_text(NewPilot* p, char* text) {

    if (p->selectedLine == 0) {
        if (strlen(p->pilot->name) + strlen(text) >= PILOT_NAME_LENGTH) {
            return;
        }

        strcat(p->pilot->name, text);
        label_setCaption(&p->labelName, p->pilot->name);
    }
    else {
        if (strlen(p->pilot->callsign) + strlen(text) >= PILOT_CALLSIGN_LENGTH) {
            return;
        }

        strcat(p->pilot->callsign, text);
        label_setCaption(&p->labelCallsign, p->pilot->callsign);
    }
}

void newpilot_next_line(NewPilot* p) {
    p->selectedLine = (p->selectedLine + 1) % 2;
}

void newpilot_remove(NewPilot* p) {
    if (p->selectedLine == 0) {
        if (strlen(p->pilot->name) > 0) {
            p->pilot->name[strlen(p->pilot->name) - 1] = '\0';
            label_setCaption(&p->labelName, p->pilot->name);
        }
    }
    else {
        if (strlen(p->pilot->callsign) > 0) {
            p->pilot->callsign[strlen(p->pilot->callsign) - 1] = '\0';
            label_setCaption(&p->labelCallsign, p->pilot->callsign);
        }
    }
}

BOOL newpilot_start(NewPilot* p) {
    if (strlen(p->pilot->name) == 0 || strlen(p->pilot->callsign) == 0) {
        if (p->selectedLine == 0 && strlen(p->pilot->name) > 0) {
            newpilot_next_line(p);
        }
        else if (p->selectedLine == 1) {
            newpilot_next_line(p);
        }

        return FALSE;
    }

    return TRUE;
}
