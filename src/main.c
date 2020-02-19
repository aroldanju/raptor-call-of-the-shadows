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

#include <stdio.h>

#include "res.h"
#include "ini.h"
#include "engine.h"
#include "game.h"
#include "definitions.h"

void read_arguments(Game *game, int argc, char* argv[]) {
	int i;

	game->arguments.levelStartup = -1;
	game->arguments.offsetStartup = 0;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--level") == 0) {
			if (argc >= i + 1) {
				game->arguments.levelStartup = atoi(argv[i+1]);
				i++;
			}
		}
		else if (strcmp(argv[i], "--offset") == 0) {
			if (argc >= i + 1) {
				game->arguments.offsetStartup = atoi(argv[i+1]);
				i++;
			}
		}
	}
}

int main(int argc, char* argv[]) {

    char path[256];
    Engine engine;
    Game game;
    INI ini;

    // Load INI file
    sprintf(path, "%s%s", DATA_PATH, FILENAME_INI);
    if (ini_load(&ini, path) != 0)  {
        printf("Error initializing: %s file not found.\n", FILENAME_INI);
        return 1;
    }

    // Configure engine
    engine.video.width = VIDEO_SCREEN_WIDTH;
    engine.video.height = VIDEO_SCREEN_HEIGHT;
    engine.video.windowWidth = atoi(ini_get_value(&ini, "DISPLAY", "width"));
    engine.video.windowHeight = atoi(ini_get_value(&ini, "DISPLAY", "height"));
    engine.video.fps = atoi(ini_get_value(&ini, "DISPLAY", "fps"));
    engine.video.renderQuality = atoi(ini_get_value(&ini, "DISPLAY", "quality"));
    strcpy(engine.video.title, VIDEO_SCREEN_TITLE);
    sprintf(path, "%s%s", DATA_PATH, FILENAME_ICON);
    strcpy(engine.video.icon, path);

    engine.audio.settings.musicVol = atoi(ini_get_value(&ini, "AUDIO", "music"));
    engine.audio.settings.soundVol = atoi(ini_get_value(&ini, "AUDIO", "sound"));

    // Init engine
    if (!engine_init(&engine)) {
        printf("Error initializing engine.\n");
        return 1;
    }

    read_arguments(&game, argc, argv);

    // Init game stuff
    sprintf(game.pathData, "%s%s", DATA_PATH, FILENAME_DATA);
    game.engine = &engine;
    game_init(&game);

    // Run game
    game_loop(&game);

    // Dispose game
    game_dispose(&game);

    // Release engine
    engine_release(&engine);

    return 0;
}
