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

#include "hud.h"

#include "definitions.h"

uint8_t* get_shields_color_index(uint32_t i) {
    uint32_t colorIndex = 0;

    if (i == 0) {
        colorIndex = 0;
    }
    else {
        colorIndex = (i-1) / 11;
    }

    return (uint8_t*)HUD_SHIELDS_COLORS[colorIndex];
}

void hud_create(Hud* hud, Pilot* pilot, Resources* resources) {
    hud->pilot = pilot;
    hud->fontCredits = resources->images[6];

    hud->scanner.position = 0.0f;
    hud->scanner.enabled = FALSE;
    hud->scanner.percent = 0.0f;

    image_extract(&hud->credits[0], hud->fontCredits, 10 * 8, 0, 8, 8);
    hud_on_credits_change(hud);

    hud->bomb = G_RESOURCES.images[IMAGE_BOMB];
    hud->shield = G_RESOURCES.images[IMAGE_SHIELD];
    hud->items = G_RESOURCES.images[IMAGE_BONUS];

    image_create(&hud->selectedWeapon);
}

void hud_render(Hud* hud, Video* video)   {
    int i;

    // Energy
    if (hud->pilot->energy > 0) {
        for (i = 0; i < hud->pilot->energy; i++) {
            uint8_t *color = get_shields_color_index(i);
            video_render_line(video, VIDEO_SCREEN_WIDTH - HUD_SHIELD_LEFT - HUD_SHIELD_WIDTH, VIDEO_SCREEN_HEIGHT - 1 - (i) * 2, VIDEO_SCREEN_WIDTH - HUD_SHIELD_LEFT, VIDEO_SCREEN_HEIGHT - 1 - (i) * 2, color[0], color[1], color[2]);
        }
    }

    // Shield
    if (hud->pilot->shield > 0) {
        for (i = 0; i < hud->pilot->shield; i++) {
            uint8_t *color = get_shields_color_index(i);
            video_render_line(video, HUD_SHIELD_LEFT, VIDEO_SCREEN_HEIGHT - 1 - (i) * 2, HUD_SHIELD_LEFT + HUD_SHIELD_WIDTH, VIDEO_SCREEN_HEIGHT - 1 - (i) * 2, color[0], color[1], color[2]);
        }
    }

    // Credits
    for (i = 0; i < 10; i++) {
        image_render(&hud->credits[i], HUD_CREDITS_LEFT + HUD_CREDIT_FONT_SIZE * i, HUD_CREDITS_TOP, video);
    }

    // Bombs
    for (i = 0; i < hud->pilot->bombs; i++) {
        image_render(hud->bomb, HUD_BOMBS_LEFT + (13 * i), HUD_BOMBS_TOP, video);
    }

    // Shields
    for (i = 0; i < hud->pilot->shields; i++) {
        image_render(hud->shield, HUD_SHIELDS_LEFT + (11 * i), HUD_SHIELDS_TOP, video);
    }

    // Selected weapon
    image_render(&hud->selectedWeapon, HUD_WEAPON_LEFT, HUD_WEAPON_TOP, video);

    if (hud->pilot->items[ITEM_IONSCANNER] > 0) {
    	if (!hud->scanner.enabled) {
    		video_render_line(video, HUD_IONSCANNER_LEFT + hud->scanner.position, HUD_IONSCANNER_TOP, HUD_IONSCANNER_LEFT + hud->scanner.position, HUD_IONSCANNER_TOP + 3, HUD_IONSCANNER_COLORS[0][0], HUD_IONSCANNER_COLORS[0][1], HUD_IONSCANNER_COLORS[0][2]);
    		video_render_line(video, VIDEO_SCREEN_WIDTH - HUD_IONSCANNER_LEFT - hud->scanner.position, HUD_IONSCANNER_TOP, VIDEO_SCREEN_WIDTH - HUD_IONSCANNER_LEFT - hud->scanner.position, HUD_IONSCANNER_TOP + 3, HUD_IONSCANNER_COLORS[0][0], HUD_IONSCANNER_COLORS[0][1], HUD_IONSCANNER_COLORS[0][2]);

    		hud->scanner.position += 0.7f;
    		if (hud->scanner.position >= 51) {
    			hud->scanner.position = 0;
    		}
    	}
    	else {
    		// Show scanner
    		video_render_rect(video, HUD_IONSCANNER_LEFT, HUD_IONSCANNER_TOP, HUD_IONSCANNER_WIDTH, HUD_IONSCANNER_HEIGHT, HUD_IONSCANNER_COLORS[1][0], HUD_IONSCANNER_COLORS[1][1], HUD_IONSCANNER_COLORS[1][2]);
    		video_render_rect(video,
    				HUD_IONSCANNER_LEFT + 1,
    				HUD_IONSCANNER_TOP + 1,
    				((HUD_IONSCANNER_WIDTH-2) * hud->scanner.percent),
    				HUD_IONSCANNER_HEIGHT - 2, HUD_IONSCANNER_COLORS[0][0], HUD_IONSCANNER_COLORS[0][1], HUD_IONSCANNER_COLORS[0][2]);
    	}
    }
}

void hud_on_credits_change(Hud* hud)    {
    char credits[16];
    int i;

    sprintf(credits, "$%09d", hud->pilot->cash);

    for (i = 1; i < strlen(credits); i++) {
        image_extract(&hud->credits[i], hud->fontCredits, (credits[i] - 48) * 8, 0, 8, 8);
    }
}

void hud_on_weapon_change(Hud* hud, Weapon* weapon) {
    if (weapon) {
        image_destroy(&hud->selectedWeapon);
        image_extract(&hud->selectedWeapon, hud->items, 0, G_DATA.items[weapon->data->itemId]->thumbnail * 16, 16, 16);
    }
}

void hud_release(Hud* hud) {
    int i;

    image_destroy(&hud->selectedWeapon);

    for (i = 0; i < 10; i++) {
        image_destroy(&hud->credits[i]);
    }
}