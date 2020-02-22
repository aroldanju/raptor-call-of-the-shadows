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

#include "scene.h"

#include "res.h"
#include "definitions.h"
#include "game.h"

#ifdef __DEBUG__
#	include <stdio.h>
#endif

BOOL scene_create(Scene* scene, Map* map, Resources *resources, int32_t offset)   {
    int i, j, k;
    uint32_t length;

    // Megabomb
    scene->megabomb.dropped = FALSE;
    scene->megabomb.actor.x = scene->megabomb.actor.y = 0;
    scene->megabomb.actor.xVel = scene->megabomb.actor.yVel = GAME_MEGABOMB_VELOCITY;
    sprite_create(&scene->megabomb.actor.sprite);
    Animation* megabombAnimation = sprite_createState(&scene->megabomb.actor.sprite);
    megabombAnimation->delay = 50;
    for (i = 0; i < IMAGE_MEGABOMB_BULLET_SIZE; i++) {
        animation_addFrame(megabombAnimation, resources->images[IMAGE_MEGABOMB_BULLET_START + i]);
    }

    // Empty handlers
    scene->on_bullet_update = NULL;
    scene->on_building_destroy = NULL;
    scene->on_enemy_destroy = NULL;
    scene->on_enemy_shot = NULL;
    scene->on_enemy_update = NULL;
    scene->on_megabomb = NULL;

    scene->resources = resources;
    scene->map = map;
    scene->absoluteOffset = offset;
    scene->offset = 0.0f;
    scene->tileset = G_RESOURCES.images[IMAGE_TILESET_A];
    //scene->tileset = tileset;

    // Creates pilot
    ship_create_empty(&scene->pilot);
    scene->pilot.actor.x = SCENE_LEFT + 128;
    scene->pilot.actor.y = SCENE_BOUND_BOTTOM;
    scene->pilot.actor.xVel = scene->pilot.actor.yVel = 0.0f;
    sprite_create(&scene->pilot.actor.sprite);
    sprite_addState(&scene->pilot.actor.sprite, resources->images[3]);
    Animation* left = sprite_createState(&scene->pilot.actor.sprite);
    left->delay = 150;
    left->infinite = FALSE;
    animation_addFrame(left, resources->images[3]);
    animation_addFrame(left, resources->images[2]);
    animation_addFrame(left, resources->images[1]);
    Animation* right = sprite_createState(&scene->pilot.actor.sprite);
    right->infinite = FALSE;
    right->delay = 150;
    animation_addFrame(right, resources->images[3]);
    animation_addFrame(right, resources->images[4]);
    animation_addFrame(right, resources->images[5]);
    scene->pilot.actor.sprite.currentState = 0;

    scene->pilot.data = G_DATA.ships[SHIP_PILOT];
    scene->pilot.created = TRUE;
    scene->pilot.destroy = FALSE;
	scene->pilot.numWeapons = 0;
    scene->pilot.lastCollisionTick = SDL_GetTicks();

    for (i = 0; i < VISIBLE_TILES; i++) {
        for (j = 0; j < MAP_WIDTH; j++) {
            int row = scene->absoluteOffset + i;
            scene->tiles[i][j] = (Tile*)malloc(sizeof(Tile));
            Tile* tile = scene->tiles[i][j];

            tile->strength = 0;
            tile->x = j;
            tile->y = (MAP_HEIGHT - VISIBLE_TILES) + row;
            tile->explosionTick = 0;
            tile->spread = FALSE;

            //uint16_t tileId = scene->map->tiles[(MAP_HEIGHT - VISIBLE_TILES) + i][j];
            uint16_t tileId = scene->map->tiles[(MAP_HEIGHT - VISIBLE_TILES - scene->absoluteOffset) + i][j].gid;
            if (tileId > 0) {
                tileId--;
                tile->tileId = tileId;
                image_extract(&tile->image, scene->tileset, TILE_SIZE * (tileId % 16), TILE_SIZE * (tileId / 16), TILE_SIZE, TILE_SIZE);
                for (k = 0; k < G_DATA.numTiles; k++) {
                    if (G_DATA.tiles[k].gid == tileId) {
                        tile->strength = G_DATA.tiles[k].strength;
                        break;
                    }
                }
            }
        }
    }

    for (i = 0; i < SCENE_MAX_ENEMIES; i++) {
        Ship *ship = &scene->enemies[i];
        ship->created = FALSE;
        ship->destroy = TRUE;
    }

    for (i = 0; i < SCENE_MAX_BULLETS; i++) {
        Bullet *bullet = &scene->bullets[i];
        bullet->created = FALSE;
        bullet->destroy = TRUE;
    }

    for (i = 0; i < SCENE_MAX_ITEMS; i++) {
        Item *item = &scene->items[i];
		item->created = FALSE;
		item->destroy = TRUE;
    }

    for (i = 0; i < SCENE_MAX_EXPLOSIONS; i++) {
        Explosion *explosion = &scene->explosions[i];
        explosion->created = FALSE;
        explosion->destroy = TRUE;

        explosion = &scene->groundExplosions[i];
        explosion->created = FALSE;
        explosion->destroy = TRUE;
    }

    return TRUE;
}

void scene_release(Scene* scene)    {
    int i, j;

    for (i = 0; i < VISIBLE_TILES; i++) {
        for (j = 0; j < MAP_WIDTH; j++) {
            image_destroy(&scene->tiles[i][j]->image);
            free(scene->tiles[i][j]);
        }
    }

    for (i = 0; i < SCENE_MAX_EXPLOSIONS; i++) {
        explosion_release(&scene->explosions[i]);
    }

    for (i = 0; i < SCENE_MAX_EXPLOSIONS; i++) {
        explosion_release(&scene->groundExplosions[i]);
    }

    for (i = 0; i < SCENE_MAX_BULLETS; i++) {
        bullet_release(&scene->bullets[i]);
    }

    for (i = 0; i < SCENE_MAX_ITEMS; i++) {
        item_release(&scene->items[i]);
    }
}


Ship* add_enemy(Scene* scene, struct _map_enemy *enemy)  {
    int i;

    for (i = 0; i < SCENE_MAX_ENEMIES; i++) {
        Ship* ship = &scene->enemies[i];
        if (ship->destroy) {
            if (!ship_create_from_enemy(ship, enemy, scene->resources)) {
				return NULL;
			}

			ship->actor.x = SCENE_LEFT + enemy->x * TILE_SIZE - (ship->actor.w / 2 - TILE_SIZE / 2);
			ship->actor.y = SCENE_TOP;

			return ship;
        }
    }

#ifdef __DEBUG__
    printf("add_enemy(): any enemy slot was found\n");
#endif

    return NULL;
}

BOOL scene_drop_megabomb(Scene* scene) {
    if (!scene->megabomb.dropped) {
        scene->megabomb.dropped = TRUE;
        scene->megabomb.actor.x = scene->pilot.actor.x + 12;
        scene->megabomb.actor.y = scene->pilot.actor.y + 16;

        scene->megabomb.angle = atan2(
        		GAME_MEGABOMB_TARGET_Y - (scene->megabomb.actor.y + GAME_MEGABOMB_BULLET_SIZE / 2),
                GAME_MEGABOMB_TARGET_X - (scene->megabomb.actor.x + GAME_MEGABOMB_BULLET_SIZE / 2));

        return TRUE;
    }

    return FALSE;
}

void clone_tile(Tile* dst, Tile* src) {
    dst->explosionTick = src->explosionTick;
    dst->spread = src->spread;
    dst->strength = src->strength;
    dst->x = src->x;
    dst->y = src->y;
    dst->tileId = src->tileId;

    image_destroy(&dst->image);
    image_copy(&dst->image, &src->image);
}

void launch_enemies_gang(Scene* scene, uint8_t gang) {
    int j, row;

    Game* game = (Game*)scene->game;
    const uint32_t offset = MAP_HEIGHT - scene->absoluteOffset - VISIBLE_TILES;

    for (row = 1; row < GAME_MAX_GANG; row++) {

        for (j = 0; j < MAP_WIDTH; j++) {
            struct _map_tile *tile = &scene->map->tiles[offset - row][j];
            if (tile->enemy.type != -1 && tile->enemy.launched == 0) {
                if (tile->enemy.difficulty <= game->pilot.difficulty && tile->enemy.gang == gang) {
                    tile->enemy.launched = 1;
                    Ship *ship = add_enemy(scene, &tile->enemy);
                    if (ship) {
                        ship->actor.y -= row * TILE_SIZE;
                        ship->game = scene->game;
                        ship->on_shot = scene->on_enemy_shot;

						if (scene->on_enemy_appear) {
							scene->on_enemy_appear(game, ship);
						}
                    }
                }
            }
        }
    }
}

void launch_enemies(Scene* scene) {

    int j;

    uint8_t gang = 0;
    Game* game = (Game*)scene->game;
    const uint32_t offset = MAP_HEIGHT - scene->absoluteOffset - VISIBLE_TILES;

    for (j = 0; j < MAP_WIDTH; j++) {
        struct _map_tile *tile = &scene->map->tiles[offset][j];
        if (tile->enemy.type != -1 && tile->enemy.launched == 0 ) {
            if (tile->enemy.difficulty <= game->pilot.difficulty) {
                tile->enemy.launched = 1;
                Ship* ship = add_enemy(scene, &tile->enemy);
                if (ship) {
                    ship->game = scene->game;
                    ship->on_shot = scene->on_enemy_shot;

					if (scene->on_enemy_appear) {
						scene->on_enemy_appear(game, ship);
					}
                }

                gang = tile->enemy.gang;
            }
        }
    }

    if (gang != 0) {
        launch_enemies_gang(scene, gang);
    }
}

void scene_update(Scene* scene, float dt)  {
    int i, j, k;

    if (scene->absoluteOffset < MAP_HEIGHT - VISIBLE_TILES ) {
        scene->offset += GAME_SCENE_SPEED * dt;
        if (scene->offset >= 32.0f) {

            // Creates a new tile row

            scene->absoluteOffset++;
            scene->offset = 0.0f;

            for (i = VISIBLE_TILES - 1; i >= 0; i--) {
                for (j = 0; j < MAP_WIDTH; j++) {
                    if (i == 0) {
                        Tile *tile = scene->tiles[i][j];
                        tile->strength = 0;
                        tile->x = j;
                        tile->y = MAP_HEIGHT - VISIBLE_TILES - scene->absoluteOffset;
                        tile->explosionTick = 0;
                        tile->spread = FALSE;

                        //uint16_t tileId = scene->map->tiles[(MAP_HEIGHT - VISIBLE_TILES - scene->absoluteOffset) ][j];
                        uint16_t tileId = scene->map->tiles[(MAP_HEIGHT - VISIBLE_TILES -
                                                             scene->absoluteOffset)][j].gid;
                        if (tileId > 0) {
                            tileId--;
                            image_extract(&tile->image, scene->tileset, 32 * (tileId % 16), 32 * (tileId / 16), 32, 32);
                            for (k = 0; k < G_DATA.numTiles; k++) {
                                if (G_DATA.tiles[k].gid == tileId) {
                                    tile->strength = G_DATA.tiles[k].strength;
                                    break;
                                }
                            }
                        }
                    } else {
                        clone_tile(scene->tiles[i][j], scene->tiles[i - 1][j]);
                    }
                }

            }

            launch_enemies(scene);
        }
    }

    for (i = 0; i < SCENE_MAX_BULLETS; i++) {
        Bullet* bullet = &scene->bullets[i];
        bullet_update(bullet, dt);
        if (!bullet->destroy && scene->on_bullet_update) {

            // Bullet is ready to look for a target
            if (bullet->actor.targeted && !bullet->target) {
                if (bullet->owner == &scene->pilot) {
                    for (j = 0; j < SCENE_MAX_ENEMIES; j++) {
                        if (scene->enemies[j].created && !scene->enemies[j].destroy) {
                            bullet->target = &scene->enemies[j];
                            bullet->actor.angle = atan2((bullet->target->actor.y + bullet->target->actor.h / 2)- bullet->actor.y, (bullet->target->actor.x + bullet->target->actor.w / 2) - bullet->actor.x);
                            break;
                        }
                    }
                }
                else {
                    bullet->target = &scene->pilot;
                    bullet->actor.angle = atan2((bullet->target->actor.y + 16) - bullet->actor.y,
                                                (bullet->target->actor.x + 16) - bullet->actor.x);
                }
            }

            scene->on_bullet_update(scene->game, bullet);
        }
    }

    for (i = 0; i < SCENE_MAX_ENEMIES; i++) {
    	Ship* enemy = &scene->enemies[i];
        ship_update(enemy, dt);
		if (enemy->created && !enemy->destroy && scene->on_enemy_update) {
			scene->on_enemy_update(scene->game, enemy);

			// Check if enemy is out of screen
			if (!enemy->destroy) {
			    if (enemy->actor.y >= VIDEO_SCREEN_HEIGHT + enemy->actor.h * 2) {
			        enemy->destroy = TRUE;
			    }
			    else {
			        if (enemy->actor.x <= enemy->actor.w * -2 || enemy->actor.x >= VIDEO_SCREEN_WIDTH + enemy->actor.w * 2) {
			            enemy->destroy = TRUE;
			        }
			    }

			    if (!enemy->destroy) {
                    // Enemy is ready to look for a target
                    if (enemy->actor.targeted && !enemy->target) {
                        enemy->target = &scene->pilot;
                        enemy->actor.angle = atan2(enemy->target->actor.y - enemy->actor.y, enemy->target->actor.x - enemy->actor.x);
                    }
			    }
			}
		}
    }

    for (i = 0; i < SCENE_MAX_EXPLOSIONS; i++) {
        explosion_update(&scene->explosions[i], dt);
        explosion_update(&scene->groundExplosions[i], dt);
    }

    for (i = 0; i < SCENE_MAX_ITEMS; i++) {
        item_update(&scene->items[i], dt);
		if (!scene->items[i].destroy && scene->on_item_update) {
			scene->on_item_update(scene->game, &scene->items[i]);
		}
    }


    // Mega-bomb processing
    if (scene->megabomb.dropped) {

        sprite_update(&scene->megabomb.actor.sprite);

        const float x = cos(scene->megabomb.angle) * scene->megabomb.actor.xVel * dt;
        const float y = sin(scene->megabomb.angle) * scene->megabomb.actor.yVel * dt;

        scene->megabomb.actor.x += x;
        scene->megabomb.actor.y += y;

        if (scene->megabomb.actor.x + GAME_MEGABOMB_BULLET_SIZE >= GAME_MEGABOMB_TARGET_X &&
                scene->megabomb.actor.x <= GAME_MEGABOMB_TARGET_X + GAME_MEGABOMB_TARGET_W &&
                scene->megabomb.actor.y + GAME_MEGABOMB_BULLET_SIZE >= GAME_MEGABOMB_TARGET_Y &&
                scene->megabomb.actor.y <= GAME_MEGABOMB_TARGET_Y + GAME_MEGABOMB_TARGET_H)

            if (scene->on_megabomb) {
                scene->on_megabomb(scene->game);
                scene->megabomb.dropped = FALSE;
            }
    }
}

void scene_render(Scene* scene, Video* video, float xoffset, float yoffset)   {
    int i, j;

    for (i = 0; i < VISIBLE_TILES; i++) {
        for (j = 0; j < MAP_WIDTH; j++) {
            float offset = ceilf(SCENE_TOP + (i * TILE_SIZE + scene->offset));
            //image_render(&scene->tiles[i][j], SCENE_LEFT + j * 32, offset, video);
            image_render(&scene->tiles[i][j]->image, SCENE_LEFT + j * TILE_SIZE + xoffset, offset + yoffset, video);
        }
    }

    // Ground ships/buildings
    for (i = 0; i < SCENE_MAX_ENEMIES; i++) {
        if (scene->enemies[i].created && !scene->enemies[i].destroy && scene->enemies[i].data->ground == 1) {
            ship_render(&scene->enemies[i], video);
        }
    }
    for (i = 0; i < SCENE_MAX_ENEMIES; i++) {
        if (scene->enemies[i].created && !scene->enemies[i].destroy && scene->enemies[i].data->ground == 0) {
            ship_render(&scene->enemies[i], video);
        }
    }

    for (i = 0; i < SCENE_MAX_BULLETS; i++) {
        bullet_render(&scene->bullets[i], video);
    }

    for (i = 0; i < SCENE_MAX_EXPLOSIONS; i++) {
        explosion_render(&scene->groundExplosions[i], video);
    }

	// Megabomb
	if (scene->megabomb.dropped) {
		actor_render(&scene->megabomb.actor, video);
	}

	for (i = 0; i < SCENE_MAX_ITEMS; i++) {
		item_render(&scene->items[i], video);
	}

	ship_render(&scene->pilot, video);

    for (i = 0; i < SCENE_MAX_EXPLOSIONS; i++) {
        explosion_render(&scene->explosions[i], video);
    }

    // Black side bands
    video_render_rect(video, 0, 0, 16, VIDEO_SCREEN_HEIGHT, 0, 0, 0);
    video_render_rect(video, VIDEO_SCREEN_WIDTH - 16, 0, 16, VIDEO_SCREEN_HEIGHT, 0, 0, 0);
}

Bullet* scene_add_bullet(Scene* scene, const struct _weapon_data *weapon, int32_t x, int32_t y) {
    int i;

    for (i = 0; i < SCENE_MAX_BULLETS; i++) {
        Bullet* bullet = &scene->bullets[i];
        if (bullet->destroy) {
            // An empty slot for bullet was found
            bullet_create(bullet, weapon, scene->resources);
            bullet->actor.x = x - (bullet->actor.w / 2);
            bullet->actor.y = y;
            bullet->target = NULL;

            return bullet;
        }
    }

#ifdef __DEBUG__
    printf("scene_add_bullet(): any bullet slot was found\n");
#endif

    return NULL;
}

Explosion* scene_add_explosion(Scene* scene, uint8_t type, int32_t x, int32_t y, BOOL ground)  {
    int i;

    for (i = 0; i < SCENE_MAX_EXPLOSIONS; i++) {
        Explosion* explosion = NULL;
        if (ground) {
            explosion = &scene->groundExplosions[i];
        }
        else {
            explosion = &scene->explosions[i];
        }

        if (explosion->destroy) {
            explosion_create(explosion, type, scene->resources);
            explosion->actor.x = x;
            explosion->actor.y = y;
            explosion->actor.yVel = GAME_SCENE_SPEED;
            return explosion;
        }
    }

#ifdef __DEBUG__
    printf("scene_add_bullet(): any explosion slot was found\n");
#endif

    return NULL;
}

Item* scene_add_item(Scene* scene, uint8_t type, int32_t x, int32_t y)	{
	int i;

	ItemData *data = G_DATA.items[type];

	for (i = 0; i < SCENE_MAX_ITEMS; i++) {
		Item* item = &scene->items[i];
		if (item->destroy) {
			item_create(item, data, type);

			item->actor.x = x;
			item->actor.y = y;

			return item;
		}
	}

#ifdef __DEBUG__
	printf("scene_add_item(): any item slot was found\n");
#endif

	return NULL;
}

Ship* scene_find_enemy_by_position(Scene* scene, int32_t x, int32_t y) {
    int i, j;

    for (i = 0; i < SCENE_MAX_ENEMIES; i++) {
        if (scene->enemies[i].created && !scene->enemies[i].destroy) {
        	if (!scene->enemies[i].burstingUp) {
				const Ship *enemy = &scene->enemies[i];
				for (j = 0; j < enemy->data->hotNumber; j++) {
					if (x >= enemy->actor.x + enemy->data->hots[j].x &&
						x <= enemy->actor.x + enemy->data->hots[j].x + enemy->data->hots[j].w &&
						y >= enemy->actor.y + enemy->data->hots[j].y &&
						y <= enemy->actor.y + enemy->data->hots[j].y + enemy->data->hots[j].h) {
						return &scene->enemies[i];
					}
				}
			}
        }
    }

    return NULL;
}

void scene_destroy_enemies(Scene* scene, uint32_t energy) {
    int i;

    for (i = 0; i < SCENE_MAX_ENEMIES; i++) {
        if (scene->enemies[i].created && !scene->enemies[i].destroy) {
            Ship* enemy = &scene->enemies[i];
            ship_remove_energy(enemy, energy);
            if (enemy->energy == 0) {
                if (scene->on_enemy_destroy) {
                    scene->on_enemy_destroy(scene->game, enemy);
                }
            }
        }
    }
}

void scene_destroy_buildings(Scene* scene, uint32_t energy) {
    int i, j;
    Tile* tile;

    for (i = 0; i < VISIBLE_TILES; i++) {
        for (j = 0; j < MAP_WIDTH; j++) {
            tile = scene->tiles[i][j];
            if (tile->strength > 0) {
                if (energy > tile->strength) {
                    tile->strength = 0;
                } else {
                    tile->strength -= energy;
                }

                if (tile->strength == 0) {
                    if (scene->on_building_destroy) {
                        scene->on_building_destroy(scene->game, j, i);
                    }
                }
            }
        }
    }
}
