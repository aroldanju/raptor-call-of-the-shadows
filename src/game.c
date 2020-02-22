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

#include "res.h"

#include "game.h"
#include "definitions.h"
#include "loader.h"
#include "dialog.h"

Data G_DATA;
Resources G_RESOURCES;

void game_add_energy(Game* game, uint32_t energy) {
    pilot_add_energy(&game->pilot, energy);
}

void game_add_shield(Game* game, uint32_t energy) {
    pilot_add_shield(&game->pilot, energy);
}

void game_drop_megabomb(Game* game) {
    if (game->pilot.bombs > 0) {
        if (scene_drop_megabomb(&game->scene)) {
			sound_play(G_RESOURCES.sounds[SOUND_MACHINEGUN]);
            game->pilot.bombs--;
            game->pilot.items[ITEM_MEGABOMB]--;
        }
    }
}

void on_boss_defeat(Game* game) {
    printf("on_boss_defeat\n");

	game->bossDefeatTick = SDL_GetTicks();
}

void _nav_to_gameover(void* _game) {
	Game* game = (Game*)_game;

	scene_release(&game->scene);

	menu_create(&game->menu, &G_RESOURCES, &game->engine->video);
	game->state = GAMESTATE_MAINMENU;

	// TODO nav to GAMESTATE_GAMEOVER state
}

void on_megabomb(void* _game) {
	Game* game = (Game*)_game;

    fade_set_color(&game->megabombFade, 255, 255, 255);
    fade_out(&game->megabombFade, NULL, NULL);

    scene_destroy_buildings(&game->scene, GAME_MEGABOMB_DAMAGE);
    scene_destroy_enemies(&game->scene, GAME_MEGABOMB_DAMAGE);

    shake_start(&game->shake);

    sound_play(G_RESOURCES.sounds[SOUND_EXPLOSION_SHIP]);
}

void game_add_credits(Game* game, uint32_t credits) {
    pilot_add_cash(&game->pilot, credits);

    if (game->state == GAMESTATE_GAME) {
        hud_on_credits_change(&game->hud);
    }
    else if (game->state == GAMESTATE_STORE) {
        store_on_credits_change(&game->store);
    }
}

void destroy_building(Game* game, uint32_t x, uint32_t y) {
    int i;

    const uint32_t ty = (MAP_HEIGHT - VISIBLE_TILES - game->scene.absoluteOffset) + y;
    const uint32_t tx = x;

    Tile* tile = game->scene.tiles[y][x];

    tile->strength = 0;

    // Destroy tile => exchange tile
    //uint16_t tileId = game->scene.map->tiles[ty][tx];
    uint16_t tileId = game->scene.map->tiles[ty][tx].gid;
    if (tileId > 0) {
        for (i = 0; i < G_DATA.numTiles; i++) {
            const TileData* tileData = &G_DATA.tiles[i];
            if (tileData->gid == tileId - 1) {
                tileId = tileData->exchange;
                //game->scene.map->tiles[ty][tx] = tileId + 1;
                game->scene.map->tiles[ty][tx].gid = tileId + 1;
                image_extract(&game->scene.tiles[y][x]->image, game->scene.tileset, TILE_SIZE * (tileId % 16),
							  TILE_SIZE * (tileId / 16), TILE_SIZE, TILE_SIZE/*, &game->engine->video*/);

                game_add_credits(game, tileData->score);

                tile->explosionTick = SDL_GetTicks();

                break;
            }
        }
    }

    // Explosions
    x = SCENE_LEFT + x * 32;
    y = SCENE_TOP + y * 32 + game->scene.offset;

    if (CHANCE(2) == 0) {
        scene_add_explosion(&game->scene, EXPLOSION_SPARKLE, x + (-8 + rand() % 24), y + (-8 + rand() % 24), TRUE);
    }
    scene_add_explosion(&game->scene, EXPLOSION_BUILDING, x, y, TRUE);

    sound_play(G_RESOURCES.sounds[SOUND_EXPLOSION_BUILDING]);
}

void destroy_enemy(Game* game, Ship *enemy){

	game_add_credits(game, enemy->data->score);

	if (enemy->mapEnemy->boss == 0) {
		// TODO move explosion type to ShipData
		scene_add_explosion(&game->scene, EXPLOSION_LARGE, enemy->actor.x, enemy->actor.y, FALSE);
		sound_play(G_RESOURCES.sounds[SOUND_EXPLOSION_SHIP]);

		enemy->destroy = TRUE;

		// Drop item
		if (enemy->mapEnemy->item != -1) {
			scene_add_item(&game->scene, enemy->mapEnemy->item, enemy->actor.x + 16, enemy->actor.y + 16);
		}
	}
	else {
		on_boss_defeat(game);
	}
}

void on_enemy_destroy(void* _game, Ship* ship) {
    destroy_enemy((Game*)_game, ship);
}

void on_building_destroy(void* _game, uint32_t x, uint32_t y) {
    destroy_building((Game*)_game, x, y);
}

void game_pilot_hit(Game* game, uint32_t damage) {
#ifdef __DEVELOPER__
	if (game->cheat.godMode) {
#ifdef __DEBUG__
		printf("game_pilot_hit(): god mode enabled.\n");
#endif
		return;
	}
#endif

	pilot_hit(&game->pilot, damage);

	// Damaged ship
	if (game->pilot.energy < 20 && !game->scene.pilot.damaged) {
		ship_damage(&game->scene.pilot);
	}

	// Shattered ship
	if (game->pilot.energy == 0 && !game->scene.pilot.burstingUp) {
		ship_burstup(&game->scene.pilot);
	}
}


void game_add_explosion_ship(Game* game, Ship* ship, uint8_t type, int32_t sound) {
	const uint32_t explosionSize = explosion_get_size(type);
	const uint32_t width = ship_get_width(ship);
	const uint32_t height = ship_get_height(ship);

	scene_add_explosion(&game->scene, type,
						ship->actor.x - explosionSize + rand() % (width + explosionSize),
						ship->actor.y - explosionSize + rand() % (height + explosionSize), FALSE);

	if (sound >= 0) {
		sound_play(G_RESOURCES.sounds[sound]);
	}
}


void remove_enemy_energy(Game* game, Ship* ship, uint32_t energy) {
	ship_remove_energy(ship, energy);

	if (ship->energy == 0) {
		destroy_enemy(game, ship);
	}

	// If ship is a boss, update scanner with left energy or hide it if boss has been defeated
	if (ship->mapEnemy->boss == 1) {
		if (ship->energy > 0) {
			game->hud.scanner.percent = ((float) ship->energy / (float) ship->data->energy);
		}
		else {
			game->hud.scanner.enabled = FALSE;
		}
	}
}

void on_bullet_update(void* _game, Bullet* bullet) {
    int i, j;
    Game* game = (Game*)_game;

    if (bullet->owner != &game->scene.pilot) {
        // Bullet from enemies
        for (i = 0; i < game->scene.pilot.data->hotNumber; i++) {
            if (bullet->actor.x + bullet->actor.w >= game->scene.pilot.actor.x + game->scene.pilot.data->hots[i].x &&
            		bullet->actor.x <= game->scene.pilot.actor.x + game->scene.pilot.data->hots[i].x + game->scene.pilot.data->hots[i].w &&
                    bullet->actor.y + bullet->actor.h >= game->scene.pilot.actor.y + game->scene.pilot.data->hots[i].y &&
                    bullet->actor.y <= game->scene.pilot.actor.y + game->scene.pilot.data->hots[i].y + game->scene.pilot.data->hots[i].h) {

                // Show hit
                scene_add_explosion(&game->scene, EXPLOSION_HIT, bullet->actor.x - 2 + rand() % 4,
                                    bullet->actor.y - rand() % 2, FALSE);
                sound_play(G_RESOURCES.sounds[SOUND_HIT]);

                bullet->destroy = TRUE;

                game_pilot_hit(game, bullet->weapon->damage);

                break;
            }
        }
    }
    else {
        // Player bullet
        const int32_t x = (bullet->actor.x);
        const int32_t y = (bullet->actor.y);

        // Check air collision
        if (bullet->weapon->air) {
            // Look for enemy at x, y
            Ship* ship = scene_find_enemy_by_position(&game->scene, x, y);
            if (ship) {
            	/*
                // Remove energy
                ship_remove_energy(ship, bullet->weapon->damage);
                if (ship->energy == 0) {
                    destroy_enemy(game, ship);
                }

                if (ship->mapEnemy->boss == 1) {
					game->hud.scanner.percent = ((float) ship->energy / (float) ship->data->energy);
				}
            	*/

				remove_enemy_energy(game, ship, bullet->weapon->damage);

                // Show sparks
                uint8_t sparksType = rand() % 2;
                scene_add_explosion(&game->scene, EXPLOSION_BSPARK + sparksType, bullet->actor.x - 8 + rand() % 16,
                                    bullet->actor.y - rand() % 8, FALSE);

                bullet->destroy = TRUE;
            }
        }

        // Weapon is not for ground nor is destroyed
        if (bullet->destroy || !bullet->weapon->ground) {
            return;
        }

        const int32_t xTile = (x -  SCENE_LEFT) / TILE_SIZE;
		const int32_t yTile = (y - SCENE_TOP - game->scene.offset) / TILE_SIZE;

		// Out of bounds
        if (xTile < 0 || xTile >= MAP_WIDTH) {
            return;
        }

        // Tile "Building"?
        Tile* tile = game->scene.tiles[yTile][xTile];
        if (tile->strength > 0) {
            // Show hit sparks
            uint8_t sparksType = rand() % 2;
            scene_add_explosion(&game->scene, EXPLOSION_BSPARK + sparksType, bullet->actor.x - 8 + rand() % 16,
                                bullet->actor.y - rand() % 8, TRUE);

            if (bullet->weapon->damage > tile->strength) {
                tile->strength = 0;
            } else {
                tile->strength -= bullet->weapon->damage;
            }

            if (tile->strength == 0) {
                destroy_building(game, xTile, yTile);
            }

            bullet->destroy = TRUE;
        }
    }

}

void game_change_weapon(Game* game) {
	Weapon* currentWeapon = game->scene.pilot.selectedWeapon;
	ship_change_weapon(&game->scene.pilot);
	if (game->scene.pilot.selectedWeapon && currentWeapon != game->scene.pilot.selectedWeapon) {
		hud_on_weapon_change(&game->hud, game->scene.pilot.selectedWeapon);
	}
}


void game_add_item(Game* game, Pilot* pilot, uint8_t id) {
	int i;

	// Check if item is a weapon
	if (pilot->items[id] > 0) {
		for (i = 0; i < G_DATA.numWeapons; i++) {
			if (G_DATA.weapons[i]->itemId == id) {
				ship_add_weapon(&game->scene.pilot, G_DATA.weapons[i], &G_RESOURCES);
				game_change_weapon(game);
				return;
			}
		}
	}

	switch (id) {
		case ITEM_ENERGYMODULE:
			pilot->energy = pilot->items[id];
			if (pilot->energy > PILOT_MAX_ENERGY) {
				pilot->energy = PILOT_MAX_ENERGY;
			}
			break;

		case ITEM_MEGABOMB:
			pilot->bombs = pilot->items[id];
			break;

		case ITEM_PHASESHIELD:
			pilot->shields = pilot->items[id];
			if (pilot->shields > 0 && pilot->shield == 0) {
				pilot->shield = PILOT_MAX_SHIELD;
			}
			break;
	}
}


void add_item_by_id(Game* game, uint32_t id) {
	ItemData *itemData = G_DATA.items[id];

	pilot_add_item(&game->pilot, id, itemData);

	if (itemData->score > 0) {
		game_add_credits(game, itemData->score);
	}

	game_add_item(game, &game->pilot, id);
}

void game_get_item(Game* game, Item* item) {
	item->destroy = TRUE;
	add_item_by_id(game, item->id);
	sound_play(G_RESOURCES.sounds[SOUND_BONUS]);
}

void on_item_update(void* _game, Item* item) {
	Game* game = (Game*)_game;
	int j;

	const Actor* pilot = &game->scene.pilot.actor;

	// Game over
	if (game->scene.pilot.destroy) {
		return;
	}

	for (j = 0; j < game->scene.pilot.data->hotNumber; j++) {
		const ShipHot *pilotHot = &game->scene.pilot.data->hots[j];
		if (item->actor.x + 16 >= pilot->x + pilotHot->x &&
				item->actor.x < pilot->x + pilotHot->x + pilotHot->w) {
			if (item->actor.y + 16 >= pilot->y + pilotHot->y &&
					item->actor.y < pilot->y + pilotHot->y + pilotHot->h) {
				game_get_item(game, item);
			}

		}
	}
}

void game_process_ship(Game* game, Ship* ship) {
	int i;

	if (!ship->destroy && ship->damaged) {
		if (ship->mapEnemy && ship->mapEnemy->boss == 0) {
			return;
		}

		if (CHANCE(10) == 0) {
			game_add_explosion_ship(game, ship, EXPLOSION_HIT, -1);
		}

		if (ship->burstingUp) {
			if (CHANCE(8) == 0) {
				game_add_explosion_ship(game, ship, EXPLOSION_LARGE, SOUND_EXPLOSION_SHIP);
			}

			if (SDL_GetTicks() - ship->burstingUpTick >= GAME_BURSTINGUP_TIME) {
				ship->destroy = TRUE;
				ship->destroyTick = SDL_GetTicks();

				for (i = 0; i < GAME_GAMEOVER_EXPLOSIONS; i++) {
					game_add_explosion_ship(game, ship, EXPLOSION_EXTRALARGE, -1);
				}

				sound_play(G_RESOURCES.sounds[SOUND_EXPLOSION_SHIP]);
			}
		}
	}
}

void on_enemy_appear(void* _game, Ship* ship) {
	Game* game = (Game*)_game;
	if (ship->mapEnemy->boss == 1) {
		game->hud.scanner.enabled = TRUE;
		game->hud.scanner.percent = ((float)ship->energy / (float)ship->data->energy);
		printf("PERCENT: %.2f\n", game->hud.scanner.percent);
	}
}

void on_enemy_update(void* _game, Ship* ship) {
    int i, j;
    Game* game = (Game*)_game;

	const Actor* pilot = &game->scene.pilot.actor;
	const Actor* enemy = &ship->actor;

	game_process_ship(game, ship);

	// Game over
	if (game->scene.pilot.destroy || ship->burstingUp) {
		return;
	}

    if (ship->destroy || ship->burstingUp){
        return;
    }

    if (ship->data->ground == 1) {
        return;
    }

	if (SDL_GetTicks() - game->scene.pilot.lastCollisionTick >= GAME_COLLISION_DELAY) {
		for (i = 0; i < ship->data->hotNumber; i++) {

		    if (ship->destroy){
		        break;
		    }

			for (j = 0; j < game->scene.pilot.data->hotNumber; j++) {
				const ShipHot *enemyHot = &ship->data->hots[i];
				const ShipHot *pilotHot = &game->scene.pilot.data->hots[j];

				if (enemy->x + enemyHot->x + enemyHot->w >= pilot->x + pilotHot->x &&
					enemy->x < pilot->x + pilotHot->x + pilotHot->w) {
					if (enemy->y + enemyHot->y + enemyHot->h >= pilot->y + pilotHot->y &&
						enemy->y < pilot->y + pilotHot->y + pilotHot->h) {

						/*
						ship_remove_energy(ship, GAME_DAMAGE_COLLISION);
						if (ship->energy == 0) {
							destroy_enemy(game, ship);
						}

						// If ship is a boss, update scanner with left energy or hide it if boss has been defeated
						if (ship->mapEnemy->boss == 1) {
							if (ship->energy > 0) {
								game->hud.scanner.percent = ((float) ship->energy / (float) ship->data->energy);
							}
							else {
								game->hud.scanner.enabled = FALSE;
							}
						}
						*/

						remove_enemy_energy(game, ship, GAME_DAMAGE_COLLISION);

						game_pilot_hit(game, GAME_DAMAGE_COLLISION);
						game->scene.pilot.lastCollisionTick = SDL_GetTicks();

						break;
					}
				}
			}

			if (SDL_GetTicks() - game->scene.pilot.lastCollisionTick >= GAME_COLLISION_DELAY) {
				break;
			}
		}
	}
}

void on_enemy_shot(void* _game, Ship* ship, uint8_t weaponId) {
    Game* game = (Game*)_game;

    if (weaponId >= G_DATA.numWeapons) {
#ifdef __DEBUG__
        printf("on_enemy_shot(): invalid weapon id %d\n", weaponId);
#endif
        return;
    }

    // Ship is bursting up, it can not shot
    if (ship->burstingUp) {
        return;
    }

    const struct _weapon_data* weapon = G_DATA.weapons[weaponId];
    const struct _behaviour_step *step = &ship->behaviour.behaviour->steps[ship->behaviour.currentBehaviourStep];

    Bullet* bullet = scene_add_bullet(&game->scene, weapon, ship->actor.x + step->x, ship->actor.y + step->y);
    if (bullet) {
    	bullet->actor.x += bullet->actor.w / 2;
        bullet->owner = ship;
        sound_play(G_RESOURCES.sounds[weapon->soundId]);
    }
}

void create_pilot(Game* game) {
    int i, j;
    BOOL isWeapon;

    game->scene.pilot.selectedWeapon = NULL;
    game->scene.pilot.selectedWeaponIndex = 0;

    // Transform items to real pilot features
    for (i = 0; i < DATA_MAX_ITEMS; i++) {
    	game_add_item(game, &game->pilot, i);

    	/*
        isWeapon = FALSE;

        // Check if item is a weapon
        if (game->pilot.items[i] > 0) {
            for (j = 0; j < G_DATA.numWeapons; j++) {
                if (G_DATA.weapons[j]->itemId == i) {
                    ship_add_weapon(&game->scene.pilot, G_DATA.weapons[j], &G_RESOURCES);
                    isWeapon = TRUE;
                }
            }
        }

        if (!isWeapon) {
        	game_add_item(&game->pilot, i);
        }
    	*/
    }

    game_change_weapon(game);
}

#ifdef __DEVELOPER__
void game_toggle_console(Game* game) {
	game->console.open = !game->console.open;

	game->console.code[0] = '\0';
	label_setCaption(&game->console.labelCode, "");
}

void game_write_code(Game* game, char *text) {
	strcat(game->console.code, text);
	game->console.code[25] = '\0';
	label_setCaption(&game->console.labelCode, game->console.code);
}

void game_send_code(Game* game) {
	//game->console.open = !game->console.open;

	int i;

	if (strcmp(game->console.code, "CASH") == 0) {
		game_add_credits(game, 10000);
		sound_play(G_RESOURCES.sounds[SOUND_STORE]);

		label_setCaption(&game->console.labelFeedback, "Add $10,000");
	}
	else if (strcmp(game->console.code, "ENERGY") == 0) {
		//game_add_energy(game, 25);
		add_item_by_id(game, ITEM_ENERGYMODULE);
		sound_play(G_RESOURCES.sounds[SOUND_STORE]);

		label_setCaption(&game->console.labelFeedback, "Add 25 energy points");
	}
	else if (strcmp(game->console.code, "SHIELD") == 0) {
		//game_add_shield(game, 25);
		add_item_by_id(game, ITEM_PHASESHIELD);
		sound_play(G_RESOURCES.sounds[SOUND_STORE]);

		label_setCaption(&game->console.labelFeedback, "Add 25 shield points");
	}
	else if (strcmp(game->console.code, "GOD") == 0) {
		game->cheat.godMode = !game->cheat.godMode;
		sound_play(G_RESOURCES.sounds[SOUND_STORE]);

		if (game->cheat.godMode) {
			label_setCaption(&game->console.labelFeedback, "God mode enabled");
		}
		else {
			label_setCaption(&game->console.labelFeedback, "God mode disabled");
		}
	}
	else if (strcmp(game->console.code, "BANG") == 0) {
		sound_play(G_RESOURCES.sounds[SOUND_STORE]);
		on_megabomb(game);

		label_setCaption(&game->console.labelFeedback, "Scene explode");
	}
	else if (strcmp(game->console.code, "MUSIC") == 0) {
	    audio_toggle_music(&game->engine->audio);
	    if (game->engine->audio.settings.musicEnabled) {
            label_setCaption(&game->console.labelFeedback, "Music ON");
        }
	    else {
            label_setCaption(&game->console.labelFeedback, "Music OFF");
	    }

		sound_play(G_RESOURCES.sounds[SOUND_STORE]);
	}
	else if (strcmp(game->console.code, "AIRAIRMISSILE") == 0) {
		add_item_by_id(game, ITEM_AIRAIRMISSILE);
		game_change_weapon(game);
		sound_play(G_RESOURCES.sounds[SOUND_STORE]);
		label_setCaption(&game->console.labelFeedback, "Add 1x Air/Air Missile");
	}

	game->console.feedbackTick = SDL_GetTicks();
	game->console.code[0] = '\0';
	label_setCaption(&game->console.labelCode, "");

	game->console.open = !game->console.open;
}

void game_remove_code(Game* game) {
	if (strlen(game->console.code) > 0) {
		game->console.code[strlen(game->console.code) - 1] = '\0';
		label_setCaption(&game->console.labelCode, game->console.code);
	}
}
#endif

Music* fetch_wave_music(uint8_t sector, uint8_t wave) {
	// TODO choose right music theme
	return G_RESOURCES.musics[MUSIC_SECTOR_BRAVO_1];
}

void game_start_current_wave(Game* game) {

	if (!scene_create(&game->scene, &game->map, &G_RESOURCES, game->arguments.offsetStartup)) {
		printf("FATAL: Error creating scene.\n");
		game->quit = TRUE;
		return;
	}

	// Startup values
	game->levelPassed = FALSE;
	game->bossDefeatTick = 0;
	game->xAcc = game->yAcc = 0.0f;
	game->pressedLeft = game->pressedDown = game->pressedUp = game->pressedRight = game->pressedFire = FALSE;

	// Configure handlers
	game->scene.on_bullet_update = on_bullet_update;
	game->scene.on_item_update = on_item_update;
	game->scene.on_enemy_update = on_enemy_update;
	game->scene.on_enemy_appear = on_enemy_appear;
	game->scene.on_enemy_shot = on_enemy_shot;
	game->scene.on_enemy_destroy = on_enemy_destroy;
	game->scene.on_building_destroy = on_building_destroy;
	game->scene.on_megabomb = on_megabomb;
	game->scene.game = game;

	// Create HUD
	Resources* resources = &G_RESOURCES;
	hud_create(&game->hud, &game->pilot, resources);

	// Pilot
	create_pilot(game);

	music_play(fetch_wave_music(game->sector, game->wave));

	game->state = GAMESTATE_GAME;
}

void game_start_custom_wave(Game* game, uint8_t sector, uint8_t wave) {
	if (!load_map(game, sector, wave)) {
		printf("FATAL: Error fetching map.\n");
		game->quit = TRUE;
		return;
	}

	game->sector = sector - 1;
	game->wave = wave;

	game_start_current_wave(game);
}

void game_start_wave(Game* game) {

    uint8_t sector, wave = 1;
    pilot_get_next_wave(&game->pilot, &sector, &wave);

    switch (game->computer.selected) {
        case COMPUTEROPTION_AUTO:
            pilot_get_next_wave(&game->pilot, &sector, &wave);
            break;

        case COMPUTEROPTION_OUTER:
            sector = 3;
            wave = game->pilot.level[sector - 1];
            break;

        case COMPUTEROPTION_TANGO:
            sector = 2;
            wave = game->pilot.level[sector - 1];
            break;

        case COMPUTEROPTION_BRAVO:
            sector = 1;
            wave = game->pilot.level[sector - 1];
            break;
    }

	printf("SECTOR: WAVE -> %d:%d\n", sector, wave);

    if (!load_map(game, sector, wave)) {
        printf("FATAL: Error fetching map.\n");
        game->quit = TRUE;
        return;
    }

	game->sector = sector - 1;
	game->wave = wave;

    game_start_current_wave(game);
}

void game_pause(Game* game) {
    if (!game->paused) {
        game->paused = TRUE;
        game->pauseTick = SDL_GetTicks();
        sound_play(G_RESOURCES.sounds[SOUND_PAUSE]);
    }
    else {
        game->paused = FALSE;
    }
}

void _nav_to_store(void *_game) {
    Game* game = (Game*)_game;

    store_init(&game->store, &game->pilot, &G_RESOURCES);
    game->state = GAMESTATE_STORE;

    hangar_release(&game->hangar);
}

void _nav_to_deaperture(void *_game) {
    Game* game = (Game*)_game;

    computer_create(&game->computer, &G_RESOURCES);
    game->state = GAMESTATE_DEAPERTURE;

    hangar_release(&game->hangar);
}

void _nav_to_register(void *_game) {
    Game* game = (Game*)_game;

	pilot_startup(&game->pilot);

    newpilot_create(&game->newPilot, &game->pilot, &G_RESOURCES);
	game->lastState = game->state;
    game->state = GAMESTATE_NEWGAME;

	switch (game->lastState) {
		case GAMESTATE_MAINMENU:
			menu_release(&game->menu);
			break;
	}

    music_play(G_RESOURCES.musics[MUSIC_HANGAR]);
}

void _nav_to_hangar(void *_game) {
    Game *game = (Game*)_game;

    hangar_create(&game->hangar, &G_RESOURCES);
    game->lastState = game->state;
    game->state = GAMESTATE_HANGAR;

    switch (game->lastState) {
        case GAMESTATE_STORE:
            store_release(&game->store);
            break;

        case GAMESTATE_DEAPERTURE:
            computer_release(&game->computer);
            break;

        case GAMESTATE_NEWGAME:
            newpilot_release(&game->newPilot);
            break;

    	case GAMESTATE_GAME:
    		scene_release(&game->scene);
    		music_play(G_RESOURCES.musics[MUSIC_HANGAR]);
    		break;
    }

}

void _nav_to_menu(void *_game) {
	Game *game = (Game*)_game;

	menu_create(&game->menu, &G_RESOURCES, &game->engine->video);
	game->lastState = game->state;
	game->state = GAMESTATE_MAINMENU;

	switch (game->lastState) {
		case GAMESTATE_HANGAR:
			hangar_release(&game->hangar);
			// Enable return to hangar option
			game->menu.showReturnOption = TRUE;
			break;

		case GAMESTATE_NEWGAME:
			newpilot_release(&game->newPilot);
			game->menu.showReturnOption = FALSE;
			break;

		default:
			music_play(G_RESOURCES.musics[MUSIC_MENU]);
			break;

			/*
		case GAMESTATE_GAMEOVER:
			computer_release(&game->computer);
			break;

		case GAMESTATE_NONE:
			newpilot_release(&game->newPilot);
			break;
			*/
	}
}

void _quit(void* _game) {
	Game* game = (Game*)_game;

	game->quit = 1;
}

void quit(Game* game) {
	fade_set_color(&game->fade, 255, 0, 0);
	fade_enable(&game->fade, _quit, game);
}

void game_event(Game* game) {
    SDL_Event event;

    // Ignore events when fade is enabled
    if (game->fade.enabled) {
        return;
    }

    while (SDL_PollEvent(&event)) {

#ifdef __DEVELOPER__
        // Quit is always available
        if (event.type == SDL_QUIT) {
            //game->quit = TRUE;
            quit(game);
        }
#endif

        if (game->state == GAMESTATE_GAME) {
        	// Ignore events when level was passed
        	if (!game->levelPassed) {
				if (event.type == SDL_KEYDOWN) {
					if (event.key.keysym.sym == SDLK_LEFT) {
						if (!game->pressedLeft) {
							game->pressedLeft = TRUE;
							sprite_set_current_state(&game->scene.pilot.actor.sprite, 1);
						}
					} else if (event.key.keysym.sym == SDLK_RIGHT) {
						if (!game->pressedRight) {
							game->pressedRight = TRUE;
							sprite_set_current_state(&game->scene.pilot.actor.sprite, 2);
						}
					} else if (event.key.keysym.sym == SDLK_UP) {
						if (!game->pressedUp) {
							game->pressedUp = TRUE;
						}
					} else if (event.key.keysym.sym == SDLK_DOWN) {
						if (!game->pressedDown) {
							game->pressedDown = TRUE;
						}
					} else if (event.key.keysym.sym == SDLK_LCTRL) {
						if (!game->pressedFire) {
							game->pressedFire = TRUE;
						}
					} else if (event.key.keysym.sym == SDLK_ESCAPE) {
						game_pause(game);
					}
					else if (event.key.keysym.sym == SDLK_TAB) {
						game_change_weapon(game);
					}
					else if (event.key.keysym.sym == SDLK_SPACE) {
	#ifdef __DEVELOPER__
						if (!game->console.open) {
							game_drop_megabomb(game);
						}
	#else
						game_drop_megabomb(game);
	#endif
					}
	#ifdef __DEVELOPER__
					else if (event.key.keysym.sym == SDLK_F1) {
						game_toggle_console(game);
					}
					else if (event.key.keysym.sym == SDLK_RETURN) {
						if (game->console.open) {
							game_send_code(game);
						}
					}
					else if (event.key.keysym.sym == SDLK_BACKSPACE) {
						if (game->console.open) {
							game_remove_code(game);
						}
					}
	#endif
					else if (event.key.keysym.sym == SDLK_q) {
						//ship_add_explosion(&game->scene.pilot, EXPLOSION_LARGE);
						ship_damage(&game->scene.pilot);
					}
					else if (event.key.keysym.sym == SDLK_w) {
						//ship_add_explosion(&game->scene.pilot, EXPLOSION_LARGE);
						ship_burstup(&game->scene.pilot);
					}
				} else if (event.type == SDL_KEYUP) {
					if (event.key.keysym.sym == SDLK_LEFT) {
						game->pressedLeft = FALSE;
						sprite_set_current_state(&game->scene.pilot.actor.sprite, 0);
					} else if (event.key.keysym.sym == SDLK_RIGHT) {
						game->pressedRight = FALSE;
						sprite_set_current_state(&game->scene.pilot.actor.sprite, 0);
					}
					if (event.key.keysym.sym == SDLK_UP) {
						game->pressedUp = FALSE;
					} else if (event.key.keysym.sym == SDLK_DOWN) {
						game->pressedDown = FALSE;
					} else if (event.key.keysym.sym == SDLK_LCTRL) {
						game->pressedFire = FALSE;
						game->lastRegenerationTick = SDL_GetTicks();
					}
				}
	#ifdef __DEVELOPER__
				else if (event.type == SDL_TEXTINPUT) {
					if (game->console.open) {
						game_write_code(game, event.text.text);
					}
				}
	#endif

			}
        }
        else if (game->state == GAMESTATE_HANGAR) {
            // Arrow keys move cursor to positions [SUPPLY ROOM, FLY MISSION, SAVE PILOT, EXIT HANGAR]
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_LEFT) {
                    hangar_move_left(&game->hangar);
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    hangar_move_right(&game->hangar);
                }
                else if (event.key.keysym.sym == SDLK_RETURN) {
                    uint8_t hangarOption = game->hangar.selectedOption;
                    switch (hangarOption) {
                        case HANGAROPTION_SUPPLY:
                            sound_play(G_RESOURCES.sounds[SOUND_HANGAR]);
                            fade_enable(&game->fade, _nav_to_store, game);
                            break;
                        case HANGAROPTION_FLY:
                            sound_play(G_RESOURCES.sounds[SOUND_HANGAR]);
                            fade_enable(&game->fade, _nav_to_deaperture, game);
                            break;
                        case HANGAROPTION_EXIT:
							fade_enable(&game->fade, _nav_to_menu, game);
                            break;
                        case HANGAROPTION_SAVE:
                            // TODO show dialog
                            if (!pilot_save(&game->pilot)) {
                                printf("Error saving pilot.\n");
                            }
                            break;
                    }
                }
				else if (event.key.keysym.sym == SDLK_ESCAPE) {
					fade_enable(&game->fade, _nav_to_menu, game);
				}
            }
        }
        else if (game->state == GAMESTATE_DEAPERTURE) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    sound_play(G_RESOURCES.sounds[SOUND_HANGAR]);
                    fade_enable(&game->fade, _nav_to_hangar, game);
                }
                else if (event.key.keysym.sym == SDLK_DOWN) {
                    computer_move_down(&game->computer);
                }
                else if (event.key.keysym.sym == SDLK_UP) {
                    computer_move_up(&game->computer);
                }
                else if (event.key.keysym.sym == SDLK_RETURN) {
                    game_start_wave(game);
                }
            }
        }
        else if (game->state == GAMESTATE_STORE) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    sound_play(G_RESOURCES.sounds[SOUND_HANGAR]);
                    fade_enable(&game->fade, _nav_to_hangar, game);
                }
                else if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_LEFT) {
                    store_move_left(&game->store);
                }
                else if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_RIGHT) {
                    store_move_right(&game->store);
                }
                else if (event.key.keysym.sym == SDLK_RETURN) {
                    store_select(&game->store);
                }
                else if (event.key.keysym.sym == SDLK_SPACE) {
                    store_change_action(&game->store);
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE) {
					fade_enable(&game->fade, _nav_to_hangar, game);
                }
            }
        }
        else if (game->state == GAMESTATE_NEWGAME) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_LCTRL) {
                    newpilot_next_picture(&game->newPilot);
                }
                else if (event.key.keysym.sym == SDLK_TAB) {
                    newpilot_next_line(&game->newPilot);
                }
                else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    newpilot_remove(&game->newPilot);
                }
                else if (event.key.keysym.sym == SDLK_RETURN) {
                    if (newpilot_start(&game->newPilot)) {
                        sound_play(G_RESOURCES.sounds[SOUND_HANGAR]);
                        fade_enable(&game->fade, _nav_to_hangar, game);
                    }
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE) {
                	fade_enable(&game->fade, _nav_to_menu, game);
                    //printf("Not implemented yet.\n");
                    //sound_play(G_RESOURCES.sounds[SOUND_HANGAR]);
                    //fade_enable(&game->fade, _nav_to_hangar, game);
                }
            }
            else if (event.type == SDL_TEXTINPUT) {
                newpilot_text(&game->newPilot, event.text.text);
            }
        }
        else if (game->state == GAMESTATE_MAINMENU) {
        	if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_RETURN) {
					//fade_enable(&game->fade, _nav_to_register, game);
					switch (menu_select(&game->menu)) {
						case MENUOPTION_NEWMISSION:
							fade_enable(&game->fade, _nav_to_register, game);
							break;
						case MENUOPTION_LOADMISSION:
							if (pilot_load(&game->pilot, 0)) {
								fade_enable(&game->fade, _nav_to_hangar, game);
							}

							break;
						case MENUOPTION_GAMEOPTIONS: printf("Not implemented yet.\n"); break;
						case MENUOPTION_ORDERINFO: printf("Not implemented yet.\n"); break;
						case MENUOPTION_CREDITS: printf("Not implemented yet.\n"); break;
						case MENUOPTION_QUIT:
							// TODO show dialog
							quit(game);
							break;
						case MENUOPTION_RETURNTOGAME:
							fade_enable(&game->fade, _nav_to_hangar, game);
							break;

					}
				}
				else if (event.key.keysym.sym == SDLK_DOWN) {
					menu_move_down(&game->menu);
				}
				else if (event.key.keysym.sym == SDLK_UP) {
					menu_move_up(&game->menu);
				}
				else if (event.key.keysym.sym == SDLK_ESCAPE) {
					if (game->menu.showReturnOption) {
						// If player is in game, return to hangar
						_nav_to_hangar(game);
					}
				}
			}
        }
    }
}

void game_shot(Game* game, Ship* ship, Weapon* weapon) {

    weapon->lastShot = SDL_GetTicks();

    if (weapon->data->twin) {
        // Twin shot
        Bullet *bullet1 = NULL, *bullet2 = NULL;

        if (game->scene.pilot.actor.sprite.currentState == 0) {
            bullet1 = scene_add_bullet(&game->scene, weapon->data, game->scene.pilot.actor.x + 9, game->scene.pilot.actor.y + 15);
            bullet2 = scene_add_bullet(&game->scene, weapon->data, game->scene.pilot.actor.x + 23, game->scene.pilot.actor.y + 15);
        } else if (game->scene.pilot.actor.sprite.currentState == 1) {
            bullet1 = scene_add_bullet(&game->scene, weapon->data, game->scene.pilot.actor.x + 9,
                                       game->scene.pilot.actor.y + 15);
            bullet2 = scene_add_bullet(&game->scene, weapon->data, game->scene.pilot.actor.x + 17,
                                       game->scene.pilot.actor.y + 15);
        } else if (game->scene.pilot.actor.sprite.currentState == 2) {
            bullet1 = scene_add_bullet(&game->scene, weapon->data, game->scene.pilot.actor.x + 8,
                                       game->scene.pilot.actor.y + 15);
            bullet2 = scene_add_bullet(&game->scene, weapon->data, game->scene.pilot.actor.x + 16,
                                       game->scene.pilot.actor.y + 15);
        }

        // Set owner
        if (bullet1) {
            bullet1->owner = &game->scene.pilot;
        }

        if (bullet2) {
            bullet2->owner = &game->scene.pilot;
        }
    } else {
        // Single shot
        Bullet *bullet = scene_add_bullet(&game->scene, weapon->data,
                                          game->scene.pilot.actor.x + 10,
                                          game->scene.pilot.actor.y + 11);
        if (bullet) {
            bullet->owner = &game->scene.pilot;
        }
    }

    sound_play(weapon->sound);
}

void game_pilot_update(Game* game, float delta) {
	int i;

	Ship* pilot = &game->scene.pilot;

	if (game->levelPassed) {
		ship_update(pilot, delta);

		// Check if pilot is out of screen to start fade out to hangar
		if (pilot->actor.y <= -64) {
			fade_enable(&game->fade, _nav_to_hangar, game);
		}
		return;
	}

	if (game->pressedRight) {
		game->xAcc += PILOT_ACCELERATION;
		if (game->xAcc > PILOT_MAX_X_ACCELERATION) {
			game->xAcc = PILOT_MAX_X_ACCELERATION;
		}
	} else {
		if (game->pressedLeft) {
			game->xAcc -= PILOT_ACCELERATION;
			if (game->xAcc < -PILOT_MAX_X_ACCELERATION) {
				game->xAcc = -PILOT_MAX_X_ACCELERATION;
			}
		} else {
			if (game->xAcc != 0.0f) {
				// TODO right deceleration
				game->xAcc += PILOT_DECELERATION;
				if (game->xAcc > 0.0f) {
					game->xAcc = 0.0f;
				}
			}
		}
	}

	if (game->pressedDown) {
		game->yAcc += PILOT_ACCELERATION;
		if (game->yAcc > PILOT_MAX_Y_ACCELERATION) {
			game->yAcc = PILOT_MAX_Y_ACCELERATION;
		}
	} else {
		if (game->pressedUp) {
			game->yAcc -= PILOT_ACCELERATION;
			if (game->yAcc < -PILOT_MAX_Y_ACCELERATION) {
				game->yAcc = -PILOT_MAX_Y_ACCELERATION;
			}
		} else {
			if (game->yAcc != 0.0f) {
				game->yAcc += PILOT_DECELERATION;
				if (game->yAcc > 0.0f) {
					game->yAcc = 0.0f;
				}
			}
		}
	}

	if (game->xAcc != 0.0f) {
		game->scene.pilot.actor.x += (int) (game->xAcc);
		if (game->scene.pilot.actor.x < SCENE_BOUND_LEFT) {
			game->scene.pilot.actor.x = SCENE_BOUND_LEFT;
		} else if (game->scene.pilot.actor.x > SCENE_BOUND_RIGHT) {
			game->scene.pilot.actor.x = SCENE_BOUND_RIGHT;
		}
	}

	if (game->yAcc != 0.0f) {
		game->scene.pilot.actor.y += (int) (game->yAcc);
		if (game->scene.pilot.actor.y < SCENE_BOUND_TOP) {
			game->scene.pilot.actor.y = SCENE_BOUND_TOP;
		} else if (game->scene.pilot.actor.y > SCENE_BOUND_BOTTOM) {
			game->scene.pilot.actor.y = SCENE_BOUND_BOTTOM;
		}
	}

	// Fire
	if (game->pressedFire) {

		for (i = 0; i < game->scene.pilot.numWeapons; i++) {
			Weapon *weapon = &game->scene.pilot.weapons[i];
			if (weapon->data->alwaysEquipped) {
				// Always equiped then shot
				if (weapon_can_shot(weapon)) {
					game_shot(game, pilot, weapon);
				}
			}
			else {
				if (weapon == game->scene.pilot.selectedWeapon) {
					// Check if this weapon is the selected weapon
					if (weapon_can_shot(weapon)) {
						game_shot(game, pilot, weapon);
					}
				}
			}
		}
	}

	ship_update(pilot, delta);
	game_process_ship(game, pilot);

	if (!game->fade.enabled && pilot->destroy && SDL_GetTicks() - pilot->destroyTick >= GAME_GAMEOVER_DELAY) {
		fade_enable(&game->fade, _nav_to_gameover, game);
	}
}

void game_update(Game* game, float delta) {

    int i,j;

    if (game->fade.enabled) {
        return;
    }

    if (game->state == GAMESTATE_HANGAR) {
        hangar_update(&game->hangar, delta);
    }
    else if (game->state == GAMESTATE_DEAPERTURE) {
        computer_update(&game->computer, delta);
    }
    else if (game->state == GAMESTATE_STORE) {
        store_update(&game->store, delta);
    }
    else if (game->state == GAMESTATE_STORE) {
        newpilot_update(&game->newPilot, delta);
    }
	else if (game->state == GAMESTATE_MAINMENU) {
		menu_update(&game->menu, delta);
	}
    else if (game->state == GAMESTATE_GAME) {

		if (Mix_PlayingMusic() == -1) {
			music_play(G_RESOURCES.musics[MUSIC_SECTOR_BRAVO_1]);
		}

#ifdef __DEVELOPER__
    	// When console is open, skip update
    	if (game->console.open) {
    		return;
    	}
#endif

        if (game->paused) {

        } else {

    	    // Update propeller template
    	    animation_update(G_RESOURCES.animations[0]);

        	game_pilot_update(game, delta);

            scene_update(&game->scene, delta);

            // Check explosion spread
            for (i = 0; i < VISIBLE_TILES; i++) {
                for (j = 0; j < MAP_WIDTH; j++) {
                    Tile *tile = game->scene.tiles[i][j];
                    if (tile && !tile->spread && tile->explosionTick != 0 &&
                        SDL_GetTicks() - tile->explosionTick >= GAME_EXPLOSION_SPREAD_DELAY) {

                        // Look for neighbour buildings
                        int32_t x, y;
                        x = j - 1;
                        y = i;
                        if (x > 0 && x < MAP_WIDTH && y > 0 && y < VISIBLE_TILES) {
                            Tile *neighbourTile = game->scene.tiles[y][x];
                            if (neighbourTile->strength > 0 && !neighbourTile->spread && tile->explosionTick != 0) {
                                destroy_building(game, x, y);
                            }
                        }
                        x = j + 1;
                        y = i;
                        if (x > 0 && x < MAP_WIDTH && y > 0 && y < VISIBLE_TILES) {
                            Tile *neighbourTile = game->scene.tiles[y][x];
                            if (neighbourTile->strength > 0 && !neighbourTile->spread && tile->explosionTick != 0) {
                                destroy_building(game, x, y);
                            }
                        }
                        x = j;
                        y = i - 1;
                        if (x > 0 && x < MAP_WIDTH && y > 0 && y < VISIBLE_TILES) {
                            Tile *neighbourTile = game->scene.tiles[y][x];
                            if (neighbourTile->strength > 0 && !neighbourTile->spread && tile->explosionTick != 0) {
                                destroy_building(game, x, y);
                            }
                        }

                        // Not check anymore
                        tile->spread = TRUE;
                    }
                }
            }

            // Regeneration
            if (!game->pressedFire && SDL_GetTicks() - game->lastRegenerationTick >= GAME_REGENERATION_DELAY) {
                game_add_energy(game, 1);
                game->lastRegenerationTick = SDL_GetTicks();
            }

			if (game->shake.enabled) {
				shake_update(&game->shake, delta);
			}

			// Boss defeat?
			if (!game->levelPassed && game->bossDefeatTick > 0 && SDL_GetTicks() - game->bossDefeatTick >= GAME_BOSS_DEFAT_DELAY) {
				game->levelPassed = TRUE;
				game->scene.pilot.actor.xVel = 0.0f;
				game->scene.pilot.actor.yVel = -5.0f;

				// Reset startup level offset
				game->arguments.offsetStartup = 0;

				// Next wave

				game->pilot.level[game->sector]++;
			}
        }
    }
}

void game_render(Game* game) {
    video_clear(&game->engine->video);

    if (game->state == GAMESTATE_GAME) {
        scene_render(&game->scene, &game->engine->video, game->shake.x, game->shake.y);
        hud_render(&game->hud, &game->engine->video);

		if (game->megabombFade.enabled) {
			fade_render(&game->megabombFade, &game->engine->video);
		}

		if (game->paused) {
            // Black side bands
            video_render_rect(&game->engine->video, 0, 0, 16, VIDEO_SCREEN_HEIGHT, 0, 0, 0);
            video_render_rect(&game->engine->video, VIDEO_SCREEN_WIDTH - 16, 0, 16, VIDEO_SCREEN_HEIGHT, 0, 0, 0);
        }

#ifdef __DEVELOPER__
		if (game->console.open) {
			video_render_rect_alpha(&game->engine->video, 0, 0, VIDEO_SCREEN_WIDTH, VIDEO_SCREEN_HEIGHT, 0, 0, 0, 32);
			video_render_rect_alpha(&game->engine->video, 100, VIDEO_SCREEN_HEIGHT - 16 - 4, 128, 16, 0, 0, 0, 128);
			label_render(&game->console.labelCode, &game->engine->video, 100 + 8, VIDEO_SCREEN_HEIGHT - 16);
		}

		if (SDL_GetTicks() - game->console.feedbackTick < CONSOLE_FEEDBACK_TIME) {
			label_render(&game->console.labelFeedback, &game->engine->video, VIDEO_SCREEN_WIDTH / 2 - label_get_width(&game->console.labelFeedback) / 2, 16);
		}
#endif
    }
    else if (game->state == GAMESTATE_HANGAR) {
        hangar_render(&game->hangar, &game->engine->video);
    }
    else if (game->state == GAMESTATE_DEAPERTURE) {
        computer_render(&game->computer, &game->engine->video);
    }
    else if (game->state == GAMESTATE_STORE) {
        store_render(&game->store, &game->engine->video);
    }
    else if (game->state == GAMESTATE_NEWGAME) {
        newpilot_render(&game->newPilot, &game->engine->video);
    }
    else if (game->state == GAMESTATE_MAINMENU) {
    	menu_render(&game->menu, &game->engine->video);
    }

    if(game->fade.enabled) {
        fade_render(&game->fade, &game->engine->video);
    }

    //image_render(G_RESOURCES.images[IMAGE_BONUS], 40, 40, &game->engine->video);

    video_render(&game->engine->video);
}

int game_init(Game* game) {

    printf("Starting game...\n");

    game->levelPassed = FALSE;
    game->bossDefeatTick = 0;
    game->paused = FALSE;
    game->quit = FALSE;
    game->xAcc = game->yAcc = 0.0f;
    game->pressedLeft = game->pressedRight = game->pressedUp = game->pressedDown = FALSE;
    game->pressedFire = FALSE;
    game->state = GAMESTATE_NONE;

    data_create(&G_DATA);

    fade_create(&game->fade, VIDEO_SCREEN_WIDTH, VIDEO_SCREEN_HEIGHT);
    game->fade.delay = 40;

    fade_create(&game->megabombFade, VIDEO_SCREEN_WIDTH, VIDEO_SCREEN_HEIGHT);
    fade_set_color(&game->megabombFade, 255, 255, 255);
    game->megabombFade.delay = 60;

    shake_create(&game->shake, 4, 750);

    // Load resources
    if (!load_resources(game)) {
        printf("FATAL: Error loading resources.\n");
        game->quit = TRUE;
        return 1;
    }

	pilot_startup(&game->pilot);

    //_nav_to_register(game);
    //game_start_wave(game);

#ifdef __DEVELOPER__
    printf("\n Developer Version\n");
    printf(" F1 ............................................. Open console\n");
    printf("\n");

    // Startup console
    game->console.code[0] = '\0';
    game->console.open = FALSE;
	game->console.feedbackTick = 0;
    label_create(&game->console.labelCode, G_RESOURCES.fonts[0]);
    label_setColor(&game->console.labelCode, 172, 68, 36);
    label_create(&game->console.labelFeedback, G_RESOURCES.fonts[0]);
    label_setColor(&game->console.labelFeedback, 244, 104, 108);

    // Cheats
	game->cheat.godMode = FALSE;
#endif

	if (game->arguments.levelStartup != -1) {
		pilot_create(&game->pilot, "TEST", "TEST");
		game_start_custom_wave(game, 1, game->arguments.levelStartup);
	}
	else {
		_nav_to_menu(game);
	}

    return 0;
}

void game_loop(Game* game) {
    while (!game->quit) {
        game_event(game);

        // Updating
        game_update(game, game->engine->video.delta);

        // Rendering
        game_render(game);
    }
}

void game_dispose(Game* game) {
    if (game->state == GAMESTATE_GAME) {
        hud_release(&game->hud);
        scene_release(&game->scene);
    }
    else if (game->state == GAMESTATE_HANGAR) {
        hangar_release(&game->hangar);
    }
    else if (game->state == GAMESTATE_STORE) {
        store_release(&game->store);
    }
    else if (game->state == GAMESTATE_NEWGAME) {
        newpilot_release(&game->newPilot);
    }
    else if (game->state == GAMESTATE_DEAPERTURE) {
        computer_release(&game->computer);
    }
	else if (game->state == GAMESTATE_MAINMENU) {
		menu_release(&game->menu);
	}

    data_release(&G_DATA);
}
