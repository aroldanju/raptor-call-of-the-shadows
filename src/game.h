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

#ifndef __game_h
#define __game_h

#include <shake.h>
#include "engine.h"
#include "fade.h"
#include "scene.h"
#include "hud.h"
#include "data.h"
#include "definitions.h"
#include "hangar.h"
#include "computer.h"
#include "store.h"
#include "newpilot.h"
#include "menu.h"

struct _game_arguments {
	int32_t levelStartup;
	int32_t offsetStartup;
	int32_t skillStartup;
};

#ifdef __DEVELOPER__
struct _game_cheat {
	BOOL 	godMode;
};

struct _game_console {
	char 		code[256];
	BOOL 		open;
	Label 		labelCode;
	uint32_t 	feedbackTick;
	Label		labelFeedback;
};
#endif

typedef struct _game {
    char        pathData[256];
    Engine      *engine;
    //Data        data;
    BOOL        quit, paused;
    uint32_t    pauseTick;
    Scene       scene;
    Map         map;
    Hud         hud;
    Pilot       pilot;

    BOOL        pressedLeft, pressedRight, pressedDown, pressedUp, pressedFire;
    float       xAcc, yAcc;
    uint32_t    lastRegenerationTick;
    /*
    BOOL        megabomb;
    uint32_t    megabombTicks;
    */

    GAMESTATE    state, lastState;

    Hangar      hangar;
    Computer    computer;
    Store       store;
    NewPilot    newPilot;
	Menu 		menu;

	Fade 		megabombFade;
    Fade        fade;
    Shake		shake;

    uint32_t 	bossDefeatTick;
    BOOL 		levelPassed;

    uint8_t 	sector, wave;

#ifdef __DEVELOPER__
    struct _game_console console;
    struct _game_cheat cheat;
#endif

    struct _game_arguments arguments;

    Dialog  dialogPause;

} Game;

/**
 *  Sets startup state
 */
int game_init(Game* game);

/**
 *  Game loop
 */
void game_loop(Game* game);

/**
 *  Releases resources
 */
void game_dispose(Game* game);


#endif  /* __game_h */