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

#include "loader.h"

#include "definitions.h"
#include "res.h"
#include "item.h"

//#define __DEBUG__

BOOL load_map(Game* game, uint8_t sector, uint8_t level) {
    char mapname[32];
    char *mapData;
    uint32_t mapLength;

    sprintf(mapname, "LEVEL%d_%d.MAP", sector, level);
    mapData = res_fetch_file(game->pathData, mapname, &mapLength);
    if (!mapData) {
#ifndef __DEBUG__
        printf("_game_load_map(): error fetching map %d-%d.\n", sector, level);
#endif
        return FALSE;
    }

    map_reset(&game->map);
    map_load(&game->map, mapData, mapLength);

    return TRUE;
}

BOOL load_resource_sound(Game* game, const char *filename) {
    char *data;
    uint32_t length;
    Sound *sound;

#ifdef DEVELOPER
    printf("Loading sound %s...\n", filename);
#endif

    if (!(data = res_fetch_file(game->pathData, filename, &length)) ) {
#ifdef __DEBUG__
        printf("load_resource_sound(): error fetching resource %s.\n", filename);
#endif
        return FALSE;
    }

    sound = (Sound*)malloc(sizeof(Sound));
    sound_load_from_mem(sound, data, length, &game->engine->audio);
    resources_add_sound(&G_RESOURCES, sound);

    return TRUE;
}

BOOL load_resource_music(Game* game, const char *filename) {
    char *data;
    uint32_t length;
    Music *music;

#ifdef DEVELOPER
    printf("Loading music %s...\n", filename);
#endif

    if (!(data = res_fetch_file(game->pathData, filename, &length)) ) {
#ifdef __DEBUG__
        printf("load_resource_music(): error fetching resource %s.\n", filename);
#endif
        return FALSE;
    }

    music = (Music*)malloc(sizeof(Music));
    music_load_from_mem(music, data, length, &game->engine->audio);
    resources_add_music(&G_RESOURCES, music);

    return TRUE;
}

BOOL load_resource_image(Game* game, const char *filename) {
    char *data;
    uint32_t length;
    Image *image;

    const uint32_t paletteIndex = 0;
    const PAL* pal = G_RESOURCES.palettes[paletteIndex];

#ifdef __DEVELOPER__
    printf("Loading graphic %s...\n", filename);
#endif

    if (!(data = res_fetch_file(game->pathData, filename, &length)) ) {
#ifdef __DEBUG__
        printf("load_resource_image(): error fetching resource %s.\n", filename);
#endif
        return FALSE;
    }

    image = (Image*)malloc(sizeof(Image));
    image_create_blank(image);
    //image_load_from_mem(image, data, length, &game->engine->video);
    image_load_img(image, data, length, /*&game->engine->video,*/ pal);
    resources_add_image(&G_RESOURCES, image);

    return TRUE;
}

BOOL load_resource_img(Game* game, const char *filename, uint32_t paletteIndex) {
    char *data;
    uint32_t length;
    Image *image;

    PAL* pal = G_RESOURCES.palettes[paletteIndex];

#ifdef __DEVELOPER__
    printf("Loading graphic %s...\n", filename);
#endif

    if (!(data = res_fetch_file(game->pathData, filename, &length)) ) {
#ifdef __DEBUG__
        printf("load_resource_image(): error fetching resource %s.\n", filename);
#endif
        return FALSE;
    }

    image = (Image*)malloc(sizeof(Image));
	image_create_blank(image);
    image_load_img(image, data, length, pal);
    resources_add_image(&G_RESOURCES, image);

    return TRUE;
}

BOOL load_resource_font(Game* game, const char *filename) {
    char *data;
    uint32_t length, size = 8;
    Ttf *font;

#ifdef __DEVELOPER__
    printf("Loading font %s...\n", filename);
#endif

    if (!(data = res_fetch_file(game->pathData, filename, &length)) ) {
#ifdef __DEBUG__
        printf("load_resource_font(): error fetching resource %s.\n", filename);
#endif
        return FALSE;
    }

    font = (Ttf*)malloc(sizeof(Ttf));
    ttf_loadFromMem(font, data, length, size);
    resources_add_font(&G_RESOURCES, font);

    return TRUE;
}

BOOL load_resource_behaviour(Game* game, const char *filename) {
    char *data;
    uint32_t length;
    Behaviour *behaviour;

#ifdef __DEVELOPER__
    printf("Loading behaviour %s...\n", filename);
#endif

    if (!(data = res_fetch_file(game->pathData, filename, &length)) ) {
#ifdef __DEBUG__
        printf("load_resource_behaviour(): error fetching data %s.\n", filename);
#endif
        return FALSE;
    }

    behaviour = (Behaviour*)malloc(sizeof(Behaviour));
    behaviour_load(behaviour, data, length);
    data_add_behaviour(&G_DATA, behaviour);

    return TRUE;
}


BOOL load_resource_weapon(Game* game, const char *filename) {
    char *data;
    uint32_t length;
    WeaponData *weapon;

#ifdef __DEVELOPER__
    printf("Loading weapon %s...\n", filename);
#endif

    if (!(data = res_fetch_file(game->pathData, filename, &length)) ) {
#ifdef __DEBUG__
        printf("load_resource_weapon(): error fetching weapon %s.\n", filename);
#endif
        return FALSE;
    }

    weapon = (WeaponData*)malloc(sizeof(WeaponData));
    weapondata_load(weapon, data, length);
    data_add_weapon(&G_DATA, weapon);

    return TRUE;
}


BOOL load_resource_item(Game* game, const char *filename)   {
    char *data;
    uint32_t length;
    ItemData *item;

#ifdef __DEVELOPER__
    printf("Loading item %s...\n", filename);
#endif

    if (!(data = res_fetch_file(game->pathData, filename, &length)) ) {
#ifdef __DEBUG__
        printf("load_resource_item(): error fetching item %s.\n", filename);
#endif
        return FALSE;
    }

    item = (ItemData*)malloc(sizeof(ItemData));
    item_load(item, data, length);
    data_add_item(&G_DATA, item);

    return TRUE;
}

BOOL load_resource_ship(Game* game, const char *filename) {
    char *data;
    uint32_t length;
    ShipData *ship;

#ifdef __DEVELOPER__
    printf("Loading ship %s...\n", filename);
#endif

    if (!(data = res_fetch_file(game->pathData, filename, &length)) ) {
#ifdef __DEBUG__
        printf("load_resource_ship(): error fetching ship %s.\n", filename);
#endif
        return FALSE;
    }

    ship = (ShipData*)malloc(sizeof(ShipData));
    shipdata_load(ship, data, length);
    data_add_ship(&G_DATA, ship);

    return TRUE;
}

BOOL load_resource_palette(Game* game, const char *filename) {
    char *data;
    uint32_t length;
    PAL *palette;

#ifdef __DEVELOPER__
    printf("Loading palette %s...\n", filename);
#endif

    if (!(data = res_fetch_file(game->pathData, filename, &length)) ) {
#ifdef __DEBUG__
        printf("load_resource_palette(): error fetching palette %s.\n", filename);
#endif
        return FALSE;
    }

    palette = (PAL*)malloc(sizeof(PAL));
    image_pal_load(palette, data, length);
    resources_add_palette(&G_RESOURCES, palette);

    return TRUE;
}


BOOL load_resource_tiles(Game* game, const char *filename) {
    char *data;
    uint32_t length;
    uint32_t size = 0;

#ifdef __DEVELOPER__
    printf("Loading tiles %s...\n", filename);
#endif

    if (!(data = res_fetch_file(game->pathData, filename, &length)) ) {
#ifdef __DEBUG__
        printf("load_resource_tiles(): error fetching tiles %s.\n", filename);
#endif
        return FALSE;
    }

    if (!tiledata_load(G_DATA.tiles, &size, data, length)) {
#ifdef __DEBUG__
        printf("load_resource_tiles(): corrupted tiles file %s.\n", filename);
#endif
        return FALSE;
    }

    G_DATA.numTiles = size;

    return TRUE;
}

BOOL load_resources(Game* game) {

	// Boilerplate horror

    printf("Loading resources...\n");

    if (!load_resource_palette(game, "PALETTE.PAL")) {
        printf("Error fetching PALETTE.PAL\n");
        return FALSE;
    }

    if (!load_resource_image(game, "TILES_A.IMG")) {
        printf("Error fetching TILES_A.IMG\n");
        return FALSE;
    }

    if (!load_resource_image(game, "PILOT_1.IMG")) {
        printf("Error fetching PILOT_1.IMG\n");
        return FALSE;
    }

    if (!load_resource_image(game, "PILOT_2.IMG")) {
        printf("Error fetching PILOT_2.IMG\n");
        return FALSE;
    }

    if (!load_resource_image(game, "PILOT_3.IMG")) {
        printf("Error fetching PILOT_3.IMG\n");
        return FALSE;
    }

    if (!load_resource_image(game, "PILOT_4.IMG")) {
        printf("Error fetching PILOT_4.IMG\n");
        return FALSE;
    }

    if (!load_resource_image(game, "PILOT_5.IMG")) {
        printf("Error fetching PILOT_5.IMG\n");
        return FALSE;
    }

    if (!load_resource_image(game, "CREDITS.IMG")) {
        printf("Error fetching CREDITS.IMG\n");
        return FALSE;
    }

    if (!load_resource_image(game, "BOMB.IMG")) {
        printf("Error fetching BOMB.IMG\n");
        return FALSE;
    }

    if (!load_resource_image(game, "SHIELD.IMG")) {
        printf("Error fetching SHIELD.IMG\n");
        return FALSE;
    }

    if (!load_resource_image(game, "SHOT_1.IMG")) {
        printf("Error fetching SHOT_1.IMG\n");
        return FALSE;
    }

    if (!load_resource_image(game, "SHOT_2.IMG")) {
        printf("Error fetching SHOT_2.IMG\n");
        return FALSE;
    }

    if (!load_resource_image(game, "SHOT_3.IMG")) {
        printf("Error fetching SHOT_3.IMG\n");
        return FALSE;
    }

    if (!load_resource_image(game, "SHOT_4.IMG")) {
        printf("Error fetching SHOT_4.IMG\n");
        return FALSE;
    }

    int i;
    char filename[32];
    for (i = 1; i < 27; i++) {
        sprintf(filename, "FLARE_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    for (i = 1; i < 36; i++) {
        sprintf(filename, "BOOMB_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    for (i = 1; i < 18; i++) {
        sprintf(filename, "SPARKLE_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    if (!load_resource_image(game, "ENEMY_001.IMG")) {
        printf("Error fetching ENEMY_001.IMG\n");
        return FALSE;
    }

    for (i = 1; i < 5; i++) {
        sprintf(filename, "MGUN_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    for (i = 1; i < 10; i++) {
        sprintf(filename, "BSPARK_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    for (i = 1; i < 10; i++) {
        sprintf(filename, "OSPARK_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    for (i = 1; i < 3; i++) {
        sprintf(filename, "HANGAR_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    if (!load_resource_image(game, "CURSOR.IMG")) {
        printf("Error fetching CURSOR.IMG.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "H_SUPPLY.IMG")) {
        printf("Error fetching H_SUPPLY.IMG.\n");
        return FALSE;
    }
    if (!load_resource_image(game, "H_FLY.IMG")) {
        printf("Error fetching H_FLY.IMG.\n");
        return FALSE;
    }
    if (!load_resource_image(game, "H_SAVE.IMG")) {
        printf("Error fetching H_SAVE.IMG.\n");
        return FALSE;
    }
    if (!load_resource_image(game, "H_EXIT.IMG")) {
        printf("Error fetching H_EXIT.IMG.\n");
        return FALSE;
    }

    for (i = 1; i < 3; i++) {
        sprintf(filename, "F_BRAVO_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    for (i = 1; i < 3; i++) {
        sprintf(filename, "F_TANGO_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    for (i = 1; i < 3; i++) {
        sprintf(filename, "F_OUTER_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    for (i = 1; i < 3; i++) {
        sprintf(filename, "F_AUTO_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    if (!load_resource_image(game, "COMPUTER.IMG")) {
        printf("Error fetching COMPUTER.IMG.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "STORE.IMG")) {
        printf("Error fetching STORE.IMG.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "S_ACTION.IMG")) {
        printf("Error fetching S_ACTION.IMG.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "S_COST.IMG")) {
        printf("Error fetching S_COST.IMG.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "S_HAVE.IMG")) {
        printf("Error fetching S_HAVE.IMG.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "S_RESALE.IMG")) {
        printf("Error fetching S_RESALE.IMG.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "S_SELECT.IMG")) {
        printf("Error fetching S_SELECT.IMG.\n");
        return FALSE;
    }


    if (!load_resource_sound(game, "MACHINEGUN.WAV")) {
        printf("Error fetching MACHINEGUN.WAV\n");
        return FALSE;
    }

    if (!load_resource_sound(game, "PAUSE.WAV")) {
        printf("Error fetching PAUSE.WAV\n");
        return FALSE;
    }

    if (!load_resource_sound(game, "EXPBUILDING.WAV")) {
        printf("Error fetching EXPBUILDING.WAV\n");
        return FALSE;
    }

    if (!load_resource_sound(game, "STORE.WAV")) {
        printf("Error fetching STORE.WAV\n");
        return FALSE;
    }

    if (!load_resource_sound(game, "HANGAR.WAV")) {
        printf("Error fetching HANGAR.WAV\n");
        return FALSE;
    }

    if (!load_resource_sound(game, "MISSILE_1.WAV")) {
        printf("Error fetching MISSILE_1.WAV\n");
        return FALSE;
    }

    if (!load_resource_sound(game, "MISSILE_2.WAV")) {
        printf("Error fetching MISSILE_2.WAV\n");
        return FALSE;
    }

    if (!load_resource_sound(game, "EXPSHIP.WAV")) {
        printf("Error fetching EXPSHIP.WAV\n");
        return FALSE;
    }

    if (!load_resource_sound(game, "ENEMY_SHOT.WAV")) {
        printf("Error fetching ENEMY_SHOT.WAV\n");
        return FALSE;
    }

    if (!load_resource_sound(game, "HIT.WAV")) {
        printf("Error fetching HIT.WAV\n");
        return FALSE;
    }

    if (!load_resource_sound(game, "BONUS.WAV")) {
        printf("Error fetching BONUS.WAV\n");
        return FALSE;
    }

    if (!load_resource_music(game, "MUS_APOGEE.MID")) {
        printf("Error fetching MUS_APOGEE.MID\n");
        return FALSE;
    }

    if (!load_resource_music(game, "MUS_HANGAR.MID")) {
        printf("Error fetching MUS_HANGAR.MID\n");
        return FALSE;
    }

    if (!load_resource_music(game, "MUS_WAVE_1.MID")) {
        printf("Error fetching MUS_WAVE_1.MID\n");
        return FALSE;
    }

    if (!load_resource_music(game, "MUS_MENU.MID")) {
        printf("Error fetching MUS_MENU.MID\n");
        return FALSE;
    }



    for (i = 1; i < 4; i++) {
        sprintf(filename, "BEHAVIOUR_%d.BEH", i);
        if (!load_resource_behaviour(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    if (!load_resource_behaviour(game, "BHV_MGUN.BEH")) {
        printf("Error fetching BHV_MGUN.BEH.\n");
        return FALSE;
    }

    if (!load_resource_behaviour(game, "BHV_ENSHOT.BEH")) {
        printf("Error fetching BHV_ENSHOT.BEH.\n");
        return FALSE;
    }

    if (!load_resource_behaviour(game, "BHV_AAMI.BEH")) {
        printf("Error fetching BHV_AAMI.BEH.\n");
        return FALSE;
    }

    if (!load_resource_behaviour(game, "BHV_HELI1.BEH")) {
        printf("Error fetching BHV_HELI1.BEH.\n");
        return FALSE;
    }

    if (!load_resource_behaviour(game, "BHV_ENMI.BEH")) {
        printf("Error fetching BHV_ENMI.BEH.\n");
        return FALSE;
    }

	if (!load_resource_behaviour(game, "BHV_HELI2.BEH")) {
		printf("Error fetching BHV_HELI2.BEH.\n");
		return FALSE;
	}

	if (!load_resource_behaviour(game, "BHV_ATRACK.BEH")) {
		printf("Error fetching BHV_ATRACK.BEH.\n");
		return FALSE;
	}

	if (!load_resource_behaviour(game, "BHV_HELI3.BEH")) {
		printf("Error fetching BHV_HELI3.BEH.\n");
		return FALSE;
	}

	if (!load_resource_behaviour(game, "BHV_HELI4.BEH")) {
		printf("Error fetching BHV_HELI4.BEH.\n");
		return FALSE;
	}

	if (!load_resource_behaviour(game, "BHV_SB1.BEH")) {
		printf("Error fetching BHV_SB1.BEH.\n");
		return FALSE;
	}

	if (!load_resource_behaviour(game, "BHV_SB2.BEH")) {
		printf("Error fetching BHV_SB2.BEH.\n");
		return FALSE;
	}

	if (!load_resource_behaviour(game, "BHV_BDING.BEH")) {
		printf("Error fetching BHV_BDING.BEH.\n");
		return FALSE;
	}
	if (!load_resource_behaviour(game, "BHV_EN004_1.BEH")) {
		printf("Error fetching BHV_EN004_1.BEH.\n");
		return FALSE;
	}
	if (!load_resource_behaviour(game, "BHV_EN004_2.BEH")) {
		printf("Error fetching BHV_EN004_2.BEH.\n");
		return FALSE;
	}
	if (!load_resource_behaviour(game, "BHV_EN005_1.BEH")) {
		printf("Error fetching BHV_EN005_1.BEH.\n");
		return FALSE;
	}
	if (!load_resource_behaviour(game, "BHV_EN005_2.BEH")) {
		printf("Error fetching BHV_EN005_2.BEH.\n");
		return FALSE;
	}
	if (!load_resource_behaviour(game, "BHV_EN005_3.BEH")) {
		printf("Error fetching BHV_EN005_3.BEH.\n");
		return FALSE;
	}
	if (!load_resource_behaviour(game, "BHV_EN005_4.BEH")) {
		printf("Error fetching BHV_EN005_4.BEH.\n");
		return FALSE;
	}
	if (!load_resource_behaviour(game, "BHV_EN005_5.BEH")) {
		printf("Error fetching BHV_EN005_5.BEH.\n");
		return FALSE;
	}
	if (!load_resource_behaviour(game, "BHV_EN005_6.BEH")) {
		printf("Error fetching BHV_EN005_6.BEH.\n");
		return FALSE;
	}
	if (!load_resource_behaviour(game, "BHV_SHIP_R.BEH")) {
		printf("Error fetching BHV_SHIP_R.BEH.\n");
		return FALSE;
	}
	if (!load_resource_behaviour(game, "BHV_SHOTTRG.BEH")) {
		printf("Error fetching BHV_SHOTTRG.BEH.\n");
		return FALSE;
	}
    for (i = 1; i <= 7; i++) {
        sprintf(filename, "BHV_EN006_%d.BEH", i);
        if (!load_resource_behaviour(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    if (!load_resource_behaviour(game, "BHV_DISHOTR.BEH")) {
        printf("Error fetching BHV_DISHOTR.BEH.\n");
        return FALSE;
    }
    if (!load_resource_behaviour(game, "BHV_DISHOTL.BEH")) {
        printf("Error fetching BHV_DISHOTL.BEH.\n");
        return FALSE;
    }

    for (i = 1; i <= 3; i++) {
        sprintf(filename, "BHV_EN007_%d.BEH", i);
        if (!load_resource_behaviour(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    if (!load_resource_behaviour(game, "BHV_SHIP_L.BEH")) {
        printf("Error fetching BHV_SHIP_L.BEH.\n");
        return FALSE;
    }

    if (!load_resource_behaviour(game, "BHV_TARGET.BEH")) {
        printf("Error fetching BHV_TARGET.BEH.\n");
        return FALSE;
    }

    if (!load_resource_behaviour(game, "BHV_BOSS_1.BEH")) {
        printf("Error fetching BHV_BOSS_1.BEH.\n");
        return FALSE;
    }

    if (!load_resource_font(game, "GAME.TTF")) {
        printf("Error fetching GAME.TTF.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "STOREFNT.IMG")) {
        printf("Error fetching STOREFNT.IMG.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "STORE_ITEM.IMG")) {
        printf("Error fetching STORE_ITEM.IMG.\n");
        return FALSE;
    }
    if (!load_resource_image(game, "BG.IMG")) {
        printf("Error fetching BG.IMG.\n");
        return FALSE;
    }

    if (!load_resource_img(game, "NEWPILOT.IMG", 0)) {
        printf("Error fetching NEWPILOT.IMG.\n");
        return FALSE;
    }

    for (i = 1; i < 5; i++) {
        sprintf(filename, "PIC_PILOT_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    for (i = 1; i < 5; i++) {
        sprintf(filename, "MIN_PILOT_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    if (!load_resource_image(game, "BONUS.IMG")) {
        printf("Error fetching BONUS.IMG.\n");
        return FALSE;
    }

    for (i = 1; i < 3; i++) {
        sprintf(filename, "SHT_MAIR_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    for (i = 1; i < 3; i++) {
        sprintf(filename, "SHT_E_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    for (i = 1; i < IMAGE_EXPLOSION_SHIP_SIZE + 1; i++) {
        sprintf(filename, "AIRBOOM_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    for (i = 1; i < IMAGE_EXPLOSION_HIT_SIZE + 1; i++) {
        sprintf(filename, "HIT_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    for (i = 1; i < IMAGE_ENEMY_HELICOPTER1_SIZE + 1; i++) {
        sprintf(filename, "EN_HELI1_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

    if (!load_resource_image(game, "EN_MI_1.IMG")) {
        printf("Error fetching EN_MI_1.IMG.\n");
        return FALSE;
    }
    if (!load_resource_image(game, "EN_MI_2.IMG")) {
        printf("Error fetching EN_MI_2.IMG.\n");
        return FALSE;
    }

	if (!load_resource_image(game, "MENU.IMG")) {
		printf("Error fetching MENU.IMG.\n");
		return FALSE;
	}
	if (!load_resource_image(game, "MENU_S.IMG")) {
		printf("Error fetching MENU_S.IMG.\n");
		return FALSE;
	}
	if (!load_resource_image(game, "LOGO.IMG")) {
		printf("Error fetching LOGO.IMG.\n");
		return FALSE;
	}

	if (!load_resource_image(game, "OPT_SELECT.IMG")) {
		printf("Error fetching OPT_SELECT.IMG.\n");
		return FALSE;
	}
	if (!load_resource_image(game, "OPT_SLIDE.IMG")) {
		printf("Error fetching OPT_SLIDE.IMG.\n");
		return FALSE;
	}
	if (!load_resource_image(game, "DIALOG_BTN.IMG")) {
		printf("Error fetching DIALOG_BTN.IMG.\n");
		return FALSE;
	}
	if (!load_resource_image(game, "DIALOG_BG.IMG")) {
		printf("Error fetching DIALOG_BG.IMG.\n");
		return FALSE;
	}

	// Weapons

    if (!load_resource_weapon(game, "WPN_MGUN.WPN")) {
        printf("Error fetching WPN_MGUN.WPN.\n");
        return FALSE;
    }

    if (!load_resource_weapon(game, "WPN_ENEMY.WPN")) {
        printf("Error fetching WPN_ENEMY.WPN.\n");
        return FALSE;
    }

    if (!load_resource_weapon(game, "WPN_AAMI.WPN")) {
        printf("Error fetching WPN_AAMI.WPN.\n");
        return FALSE;
    }

    if (!load_resource_weapon(game, "WPN_ENMI.WPN")) {
        printf("Error fetching WPN_ENMI.WPN.\n");
        return FALSE;
    }

	if (!load_resource_weapon(game, "WPN_ATRACK.WPN")) {
		printf("Error fetching WPN_ATRACK.WPN.\n");
		return FALSE;
	}

    if (!load_resource_weapon(game, "WPN_EN_TRG.WPN")) {
        printf("Error fetching WPN_EN_TRG.WPN.\n");
        return FALSE;
    }
    if (!load_resource_weapon(game, "WPN_EN_DR.WPN")) {
        printf("Error fetching WPN_EN_DR.WPN.\n");
        return FALSE;
    }
    if (!load_resource_weapon(game, "WPN_EN_DL.WPN")) {
        printf("Error fetching WPN_EN_DL.WPN.\n");
        return FALSE;
    }


    // Ships

    if (!load_resource_ship(game, "SHP_PILOT.SHP")) {
        printf("Error fetching SHP_PILOT.SHP.\n");
        return FALSE;
    }

    if (!load_resource_ship(game, "SHP_ENEMY1.SHP")) {
        printf("Error fetching SHP_ENEMY1.SHP.\n");
        return FALSE;
    }

    if (!load_resource_ship(game, "SHP_HELI1.SHP")) {
        printf("Error fetching SHP_HELI1.SHP.\n");
        return FALSE;
    }

    if (!load_resource_ship(game, "SHP_BONUS1.SHP")) {
        printf("Error fetching SHP_BONUS1.SHP.\n");
        return FALSE;
    }

    if (!load_resource_ship(game, "SHP_B_BONUS.SHP")) {
        printf("Error fetching SHP_B_BONUS.SHP.\n");
        return FALSE;
    }
    if (!load_resource_ship(game, "SHP_ENEMY4.SHP")) {
        printf("Error fetching SHP_ENEMY4.SHP.\n");
        return FALSE;
    }
    if (!load_resource_ship(game, "SHP_ENEMY5.SHP")) {
        printf("Error fetching SHP_ENEMY5.SHP.\n");
        return FALSE;
    }
    if (!load_resource_ship(game, "SHP_SHIP_1.SHP")) {
        printf("Error fetching SHP_SHIP_1.SHP.\n");
        return FALSE;
    }
    if (!load_resource_ship(game, "SHP_ENEMY6.SHP")) {
        printf("Error fetching SHP_ENEMY6.SHP.\n");
        return FALSE;
    }
    if (!load_resource_ship(game, "SHP_ENEMY7.SHP")) {
        printf("Error fetching SHP_ENEMY7.SHP.\n");
        return FALSE;
    }
    if (!load_resource_ship(game, "SHP_SHIP_2.SHP")) {
        printf("Error fetching SHP_SHIP_1.SHP.\n");
        return FALSE;
    }

    if (!load_resource_ship(game, "SHP_BALL.SHP")) {
        printf("Error fetching SHP_BALL.SHP.\n");
        return FALSE;
    }

    if (!load_resource_ship(game, "SHP_BOSS_1.SHP")) {
        printf("Error fetching SHP_BOSS_1.SHP.\n");
        return FALSE;
    }

    if (!load_resource_tiles(game, "TILES.TIL")) {
        printf("Error fetching TILES.TIL.\n");
        return FALSE;
    }

    // Items

    if (!load_resource_item(game, "ITM_ENERGY.ITM")) {
        printf("Error fetching ITM_ENERGY.ITM.\n");
        return FALSE;
    }
    if (!load_resource_item(game, "ITM_MGUN.ITM")) {
        printf("Error fetching ITM_MGUN.ITM.\n");
        return FALSE;
    }
    if (!load_resource_item(game, "ITM_SCANNER.ITM")) {
        printf("Error fetching ITM_SCANNER.ITM.\n");
        return FALSE;
    }
    if (!load_resource_item(game, "ITM_BOMB.ITM")) {
        printf("Error fetching ITM_BOMB.ITM.\n");
        return FALSE;
    }
    if (!load_resource_item(game, "ITM_AAMI.ITM")) {
        printf("Error fetching ITM_ENERGY.ITM.\n");
        return FALSE;
    }
    if (!load_resource_item(game, "ITM_SHIELD.ITM")) {
        printf("Error fetching ITM_SHIELD.ITM.\n");
        return FALSE;
    }
    if (!load_resource_item(game, "ITM_PLASMA.ITM")) {
        printf("Error fetching ITM_PLASMA.ITM.\n");
        return FALSE;
    }
    if (!load_resource_item(game, "ITM_AGMI.ITM")) {
        printf("Error fetching ITM_AGMI.ITM.\n");
        return FALSE;
    }
    if (!load_resource_item(game, "ITM_DUMBMI.ITM")) {
        printf("Error fetching ITM_DUMBMI.ITM.\n");
        return FALSE;
    }
    if (!load_resource_item(game, "ITM_MICROMI.ITM")) {
        printf("Error fetching ITM_MICROMI.ITM.\n");
        return FALSE;
    }
    if (!load_resource_item(game, "ITM_MIPOD.ITM")) {
        printf("Error fetching ITM_MIPOD.ITM.\n");
        return FALSE;
    }
    if (!load_resource_item(game, "ITM_ATMGUN.ITM")) {
        printf("Error fetching ITM_ATMGUN.ITM.\n");
        return FALSE;
    }
    if (!load_resource_item(game, "ITM_PULSE.ITM")) {
        printf("Error fetching ITM_PULSE.ITM.\n");
        return FALSE;
    }
    if (!load_resource_item(game, "ITM_EWPN.ITM")) {
        printf("Error fetching ITM_EWPN.ITM.\n");
        return FALSE;
    }
    if (!load_resource_item(game, "ITM_BONUS.ITM")) {
        printf("Error fetching ITM_BONUS.ITM.\n");
        return FALSE;
    }

    for (i = 1; i < IMAGE_MEGABOMB_BULLET_SIZE + 1; i++) {
        sprintf(filename, "MEGABM_%d.IMG", i);
        if (!load_resource_image(game, filename)) {
            printf("Error fetching %s.\n", filename);
            return FALSE;
        }
    }

	if (!load_resource_image(game, "EN_BONUS1.IMG")) {
		printf("Error fetching EN_BONUS1.IMG.\n");
		return FALSE;
	}

	for (i = 1; i < IMAGE_EXPLOSION_SIZE + 1; i++) {
		sprintf(filename, "PEXPLO_%d.IMG", i);
		if (!load_resource_image(game, filename)) {
			printf("Error fetching %s.\n", filename);
			return FALSE;
		}
	}

	for (i = 1; i < IMAGE_BONUS_EXPLOSION_SIZE + 1; i++) {
		sprintf(filename, "B_BANG_%d.IMG", i);
		if (!load_resource_image(game, filename)) {
			printf("Error fetching %s.\n", filename);
			return FALSE;
		}
	}

	for (i = 1; i < IMAGE_EXPLOSION_EXTRALARGE_SIZE + 1; i++) {
		sprintf(filename, "XL_EXPL_%d.IMG", i);
		if (!load_resource_image(game, filename)) {
			printf("Error fetching %s.\n", filename);
			return FALSE;
		}
	}

	for (i = 1; i < IMAGE_GLOW_ITEM_SIZE + 1; i++) {
		sprintf(filename, "ITEMGLOW_%d.IMG", i);
		if (!load_resource_image(game, filename)) {
			printf("Error fetching %s.\n", filename);
			return FALSE;
		}
	}

    if (!load_resource_image(game, "B_BONUS_1.IMG")) {
        printf("Error fetching B_BONUS_1.IMG.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "ENEMY_004.IMG")) {
        printf("Error fetching ENEMY_004.IMG.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "ENEMY_005.IMG")) {
        printf("Error fetching ENEMY_005.IMG.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "ENEMYSHP1_1.IMG")) {
        printf("Error fetching ENEMYSHP1_1.IMG.\n");
        return FALSE;
    }
    if (!load_resource_image(game, "ENEMYSHP1_2.IMG")) {
        printf("Error fetching ENEMYSHP1_2.IMG.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "ENEMY_006.IMG")) {
        printf("Error fetching ENEMY_006.IMG.\n");
        return FALSE;
    }
    if (!load_resource_image(game, "ENEMY_007.IMG")) {
        printf("Error fetching ENEMY_007.IMG.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "ENEMYSHP2_1.IMG")) {
        printf("Error fetching ENEMYSHP2_1.IMG.\n");
        return FALSE;
    }
    if (!load_resource_image(game, "ENEMYSHP2_2.IMG")) {
        printf("Error fetching ENEMYSHP2_2.IMG.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "ENEMY_BALL1.IMG")) {
        printf("Error fetching ENEMY_BALL1.IMG.\n");
        return FALSE;
    }
    if (!load_resource_image(game, "ENEMY_BALL2.IMG")) {
        printf("Error fetching ENEMY_BALL2.IMG.\n");
        return FALSE;
    }

    if (!load_resource_image(game, "BOSS_1.IMG")) {
        printf("Error fetching BOSS_1.IMG.\n");
        return FALSE;
    }

    // Propeller template
    Animation *animation = (Animation*)malloc(sizeof(Animation));
    animation_create(animation, 30);
    int j;
    for (j = 0; j < IMAGE_PROPELLER_SIZE; j++) {
        Image *propellerImage = (Image *) malloc(sizeof(Image));
        const uint32_t h = 4 + rand() % (PROPELLER_HEIGHT - 4);
        image_create_blank_sized(propellerImage, 1, PROPELLER_HEIGHT);
        for (i = 0; i < h; i++) {
            uint32_t pixel = SDL_MapRGBA(propellerImage->surface->format, 255, 255, 255, 30 - (5 * i));
            //uint32_t pixel = SDL_MapRGBA(propellerImage->surface->format, 255, 255, 255, 255);
            surface_set_pixel(propellerImage->surface, 0, PROPELLER_HEIGHT - i, pixel);
            image_set_alpha(propellerImage, 32);
        }
        resources_add_image(&G_RESOURCES, propellerImage);
        animation_addFrame(animation, G_RESOURCES.images[IMAGE_PROPELLER_START + j]);
    }
    G_RESOURCES.numAnimations = 1;
    G_RESOURCES.animations[0] = animation;

    return TRUE;
}
