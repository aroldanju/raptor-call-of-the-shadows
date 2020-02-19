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

#ifndef __store_h
#define __store_h

#include "resources.h"
#include "label.h"

#include "definitions.h"
#include "pilot.h"

#define STORE_CREDIT_FONT_SIZE  8

#define STORE_PICTURE_LEFT  24
#define STORE_PICTURE_TOP   72

#define STORE_NAME_LEFT     48
#define STORE_NAME_TOP      125

#define STORE_CREDITS_LEFT  19
#define STORE_CREDITS_TOP   142

#define STORE_TITLE_LEFT    180
#define STORE_TITLE_TOP     52

#define STORE_DESCRIPTION_LEFT  128
#define STORE_DESCRIPTION_TOP   71

#define STORE_IMAGE_LEFT        128
#define STORE_IMAGE_TOP         42

#define STORE_DETAIL_LEFT       180
#define STORE_DETAIL_TOP        59

#define STORE_AMOUNT_LEFT       144
#define STORE_AMOUNT_TOP        136

#define STORE_COST_LEFT       225
#define STORE_COST_TOP        136

#define STORE_LABEL_AMOUNT_LEFT     128
#define STORE_LABEL_AMOUNT_TOP      126

#define STORE_LABEL_COST_LEFT       226
#define STORE_LABEL_COST_TOP         126

// Positions for buttons
#define STORE_BUTTON_BUY_LEFT       117
#define STORE_BUTTON_BUY_TOP        161
#define STORE_BUTTON_SELL_LEFT       140
#define STORE_BUTTON_SELL_TOP        161
#define STORE_BUTTON_ACTION_LEFT       176
#define STORE_BUTTON_ACTION_TOP        155

enum {
    STOREACTION_BUY,
    STOREACTION_SELL
};

typedef struct _store {
    Image   *background;
    Ttf     *font;
    Label   labelTitle, labelDescription, labelDetail, labelName;
    Sound   *soundAction, *soundError;
    Image   *fontCredits;
    Image   credits[10];
    Image   labelAmount[8], labelCost[8], labelResale[8];
    Image   *sheetIcons;
    Image   icon;
    Image   *pictures[4];

    Image   buttonActions[2][2];
    Image   buttonSelection[2];
    Image   *textAmount, *textCost, *textResale;

    uint8_t     action;
    uint8_t     currentItem;
    Pilot       *pilot;

    char        amount[8], cost[8], resale[8];
} Store;

void store_init(Store* store, Pilot* pilot, Resources *resources);

void store_release(Store* store);

void store_update(Store* store, float dt);

void store_render(Store* store, Video* video);

void store_move_left(Store* store);

void store_move_right(Store* store);

void store_select(Store* store);

void store_change_action(Store* store);

void store_on_credits_change(Store* store);

#endif  /* __store_h */