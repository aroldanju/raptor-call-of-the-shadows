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

#include "store.h"

void _on_credits_change(Store* store)    {
    char credits[8];
    int i;

    sprintf(credits, "%07d", store->pilot->cash);

    for (i = 0; i < strlen(credits); i++) {
        image_extract(&store->credits[i], store->fontCredits, (credits[i] - 48) * 8, 0, 8, 8);
    }
}

void _on_amount_change(Store* store, uint8_t amount) {
    int i;

    sprintf(store->amount, "%02d", amount);

    for (i = 0; i < strlen(store->amount); i++) {
        image_extract(&store->labelAmount[i], store->fontCredits, (store->amount[i] - 48) * 8, 0, 8, 8);
    }
}

void _on_cost_change(Store* store, uint32_t amount) {
    int i;

    sprintf(store->cost, "%d", amount);

    for (i = 0; i < strlen(store->cost); i++) {
        image_extract(&store->labelCost[i], store->fontCredits, (store->cost[i] - 48) * 8, 0, 8, 8);
    }
}

void _on_resale_change(Store* store, uint32_t amount) {
    int i;

    sprintf(store->resale, "%d", amount);

    for (i = 0; i < strlen(store->resale); i++) {
        image_extract(&store->labelResale[i], store->fontCredits, (store->resale[i] - 48) * 8, 0, 8, 8);
    }
}

void _on_change_item(Store* store) {

    const ItemData *currentItem = G_DATA.items[store->currentItem];

    _on_credits_change(store);
    _on_cost_change(store, currentItem->cost);
    _on_resale_change(store, currentItem->resale);
    _on_amount_change(store, store->pilot->items[store->currentItem]);

    label_setCaption(&store->labelTitle, (const char*)currentItem->title);
    label_setCaption(&store->labelDescription, (const char*)currentItem->description);
    label_setCaption(&store->labelDetail, (const char*)currentItem->subtitle);

    image_destroy(&store->icon);
    image_extract(&store->icon, store->sheetIcons, 0, 24 * currentItem->icon, 24, 24);
}


void store_init(Store* store,  Pilot* pilot, Resources *resources) {
    store->pilot = pilot;

    store->currentItem = 0;
    store->action = STOREACTION_BUY;

    store->sheetIcons = resources->images[IMAGE_STORE_ITEMS];
    store->icon.texture = NULL;
    store->icon.surface = NULL;
    store->soundAction = resources->sounds[SOUND_PAUSE];
    store->soundError = resources->sounds[SOUND_STORE];

    store->fontCredits = resources->images[IMAGE_STORE_FONT];

    store->background = resources->images[IMAGE_STORE_BACKGROUND];
    store->font = resources->fonts[FONT_DEFAULT];
    store->textAmount = resources->images[IMAGE_STORE_HAVE];
    store->textResale = resources->images[IMAGE_STORE_RESALE];
    store->textCost = resources->images[IMAGE_STORE_COST];

    label_create(&store->labelName, store->font);
    label_setColor(&store->labelName, 207, 97, 52);

    label_create(&store->labelTitle, store->font);
    label_setColor(&store->labelTitle, 247, 203, 28);

    label_create(&store->labelDetail, store->font);
    label_setColor(&store->labelDetail, 166, 40, 48);

    label_create(&store->labelDescription, store->font);
    label_setColor(&store->labelDescription, 207, 97, 52);

    // Extract images for buttons
    image_extract(&store->buttonActions[0][0], resources->images[IMAGE_STORE_ACTIONS], 0, 0, 16, 8);
    image_extract(&store->buttonActions[0][1], resources->images[IMAGE_STORE_ACTIONS], 16, 0, 16, 8);
    image_extract(&store->buttonActions[1][0], resources->images[IMAGE_STORE_ACTIONS], 0, 8, 16, 8);
    image_extract(&store->buttonActions[1][1], resources->images[IMAGE_STORE_ACTIONS], 16, 8, 16, 8);

    image_extract(&store->buttonSelection[0], resources->images[IMAGE_STORE_SELECT], 0, 0, 48, 24);
    image_extract(&store->buttonSelection[1], resources->images[IMAGE_STORE_SELECT], 0, 24, 48, 24);

    int i;
    for (i= 0; i < 4; i++) {
        store->pictures[i] = resources->images[IMAGE_PICTURE_PILOT_START + i];
    }

    // Dynamic labels
    label_setCaption(&store->labelName, pilot->name);

    _on_change_item(store);
}

void store_release(Store* store)    {
    int i, j;

    label_destroy(&store->labelName);
    label_destroy(&store->labelDescription);
    label_destroy(&store->labelTitle);
    label_destroy(&store->labelDetail);

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            image_destroy(&store->buttonActions[i][j]);
        }
    }

    for (i = 0; i < 2; i++) {
        image_destroy(&store->buttonSelection[i]);
    }

    image_destroy(&store->icon);

    for (i = 0; i < 7; i++) {
        image_destroy(&store->credits[i]);
    }

    for (i = 0; i < strlen(store->amount); i++) {
        image_destroy(&store->labelAmount[i]);
    }

    for (i = 0; i < strlen(store->cost); i++) {
        image_destroy(&store->labelCost[i]);
    }

    for (i = 0; i < strlen(store->resale); i++) {
        image_destroy(&store->labelResale[i]);
    }

}

void store_update(Store* store, float dt)   {
	// None
}

void store_render(Store* store, Video* video)   {
    int i;

    image_render(store->background, 0, 0, video);

    // Render static labels
    if (store->action == STOREACTION_BUY) {
        image_render(store->textCost, STORE_LABEL_COST_LEFT, STORE_LABEL_COST_TOP, video);
    }
    else {
        image_render(store->textResale, STORE_LABEL_COST_LEFT, STORE_LABEL_COST_TOP, video);
    }
    image_render(store->textAmount, STORE_LABEL_AMOUNT_LEFT, STORE_LABEL_AMOUNT_TOP, video);

    // Picture
    image_render(store->pictures[store->pilot->picture], 21, 71, video);

    // Icon
    image_render(&store->icon, STORE_IMAGE_LEFT, STORE_IMAGE_TOP, video);

    // Render buttons
    Image* buttonBuy, *buttonSell, *buttonAction;

    if (store->action == STOREACTION_BUY) {
        buttonBuy = &store->buttonActions[0][0];
        buttonSell = &store->buttonActions[1][1];
        buttonAction = &store->buttonSelection[0];
    }
    else {
        buttonBuy = &store->buttonActions[0][1];
        buttonSell = &store->buttonActions[1][0];
        buttonAction = &store->buttonSelection[1];
    }

    image_render(buttonBuy, STORE_BUTTON_BUY_LEFT, STORE_BUTTON_BUY_TOP, video);
    image_render(buttonSell, STORE_BUTTON_SELL_LEFT, STORE_BUTTON_SELL_TOP, video);
    image_render(buttonAction, STORE_BUTTON_ACTION_LEFT, STORE_BUTTON_ACTION_TOP, video);

    // Dynamic labels
    label_render(&store->labelTitle, video, STORE_TITLE_LEFT, STORE_TITLE_TOP);
    label_render(&store->labelDetail, video, STORE_DETAIL_LEFT, STORE_DETAIL_TOP);
    label_render_wrap(&store->labelDescription, video, STORE_DESCRIPTION_LEFT, STORE_DESCRIPTION_TOP, 156);
    label_render(&store->labelName, video, STORE_NAME_LEFT - label_get_width(&store->labelName) / 2 , STORE_NAME_TOP);

    // Credits

    for (i = 0; i < 7; i++) {
        image_render(&store->credits[i], STORE_CREDITS_LEFT + STORE_CREDIT_FONT_SIZE * i, STORE_CREDITS_TOP, video);
    }

    for (i = 0; i < strlen(store->amount); i++) {
        image_render(&store->labelAmount[i], STORE_AMOUNT_LEFT + STORE_CREDIT_FONT_SIZE * i, STORE_AMOUNT_TOP, video);
    }

    if (store->action == STOREACTION_BUY) {
        for (i = 0; i < strlen(store->cost); i++) {
            image_render(&store->labelCost[i], STORE_COST_LEFT + STORE_CREDIT_FONT_SIZE * i, STORE_COST_TOP, video);
        }
    }
    else {
        for (i = 0; i < strlen(store->resale); i++) {
            image_render(&store->labelResale[i], STORE_COST_LEFT + STORE_CREDIT_FONT_SIZE * i, STORE_COST_TOP, video);
        }
    }
}

void store_move_left(Store* store) {
    if (store->currentItem == 0) {
        store->currentItem = DATA_MAX_ITEMS - 1;
    }
    else {
        store->currentItem--;
    }

    // Null item
    if (!G_DATA.items[store->currentItem]) {
        return store_move_left(store);
    }

    if (G_DATA.items[store->currentItem]->cost == 0) {
        return store_move_left(store);
    }
    else {
        if (store->action == STOREACTION_BUY) {
            if (store->pilot->items[store->currentItem] >= G_DATA.items[store->currentItem]->maxAmount &&
                G_DATA.items[store->currentItem]->duplicate == 0) {
                return store_move_left(store);
            }
        } else {
            if (store->pilot->items[store->currentItem] == 0) {
                return store_move_left(store);
            }
        }
    }

    sound_play(store->soundAction);
    _on_change_item(store);
}

void store_move_right(Store* store) {
    if (store->currentItem == DATA_MAX_ITEMS - 1) {
        store->currentItem = 0;
    }
    else {
        store->currentItem++;
    }

    // Null item
    if (!G_DATA.items[store->currentItem]) {
        return store_move_right(store);
    }

    if (G_DATA.items[store->currentItem]->cost == 0) {
        // Not "OBTENIBLE" item
        return store_move_right(store);
    }
    else {
        if (store->action == STOREACTION_BUY) {
            if (store->pilot->items[store->currentItem] >= G_DATA.items[store->currentItem]->maxAmount &&
                    G_DATA.items[store->currentItem]->duplicate == 0) {
                return store_move_right(store);
            }
        } else {
            if (store->pilot->items[store->currentItem] == 0) {
                return store_move_right(store);
            }
        }
    }

    sound_play(store->soundAction);
    _on_change_item(store);
}

void store_select(Store* store) {

	// TODO show store error screen

    const ItemData* item = G_DATA.items[store->currentItem];

    if (store->action == STOREACTION_BUY) {
        if (!pilot_has_cash(store->pilot, item->cost)) {
#ifdef __DEBUG__
            printf("store_select(): not enough credits (%d/%d).\n", store->pilot->cash, item->cost);
#endif
            sound_play(store->soundError);
            return;
        }

        if (store->pilot->items[store->currentItem] >= item->maxAmount) {
#ifdef __DEBUG__
            printf("store_select(): max amount reached.\n");
#endif
            sound_play(store->soundError);
            return;
        }

        pilot_remove_cash(store->pilot, item->cost);
        store->pilot->items[store->currentItem] += item->pack;
        if (store->pilot->items[store->currentItem] > item->maxAmount) {
            store->pilot->items[store->currentItem] = item->maxAmount;
        }

        sound_play(store->soundAction);
        _on_amount_change(store, store->pilot->items[store->currentItem]);
        _on_credits_change(store);
    }
    else {
        if (store->pilot->items[store->currentItem] <= item->minAmount) {
#ifdef __DEBUG__
            printf("store_select(): min amount reached (%d/%d).\n", store->pilot->items[store->currentItem], item->minAmount);
#endif
            sound_play(store->soundError);
            return;
        }

        pilot_add_cash(store->pilot, item->resale);
        store->pilot->items[store->currentItem] -= item->pack;
        if (store->pilot->items[store->currentItem] < item->minAmount) {
            store->pilot->items[store->currentItem] = item->minAmount;
        }

        sound_play(store->soundAction);
        _on_amount_change(store, store->pilot->items[store->currentItem]);
        _on_credits_change(store);
    }
}

void store_change_action(Store* store) {
    if (store->action == STOREACTION_BUY) {
        store->action = STOREACTION_SELL;
    }
    else {
        store->action = STOREACTION_BUY;
    }

    if (store->action == STOREACTION_BUY) {
        if (store->pilot->items[store->currentItem] >= G_DATA.items[store->currentItem]->maxAmount &&
            G_DATA.items[store->currentItem]->duplicate == 0) {
            return store_move_right(store);
        }
    }
    else {
        if (store->pilot->items[store->currentItem] == 0) {
            return store_move_right(store);
        }
    }

    sound_play(store->soundAction);
}

void store_on_credits_change(Store* store) {
    _on_credits_change(store);
}