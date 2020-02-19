#include "itemtool.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int itemtool_load(ItemData* item, const char* filename) {
    FILE* file;
    int lines;
    unsigned char frameId;
    char *tkn;
    char buffer[256];
    int bufferLength;

    lines = 0;
    frameId = 0;
    bufferLength = 256;

    file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening item file %s.\n", filename);
        return -1;
    }

    item->magic[0] = 'I';
    item->magic[1] = 'T';
    item->magic[2] = 'M';

    while(fgets(buffer, bufferLength, file)) {
        if (buffer[0] != '#' && buffer[0] != '\n') {
            if (buffer[strlen(buffer) - 1] == '\n') {
                buffer[strlen(buffer) - 1] = '\0';
            }

            if (lines == 0) {
                strcpy((char*)item->title, buffer);
            }
            else if (lines == 1) {
                strcpy((char*)item->subtitle, buffer);
            }
            else if (lines == 2) {
                strcpy((char*)item->description, buffer);
            }
            else if (lines == 3) {
                item->cost = atoi(buffer);
            }
            else if (lines == 4) {
                item->resale = atoi(buffer);
            }
            else if (lines == 5) {
                item->pack = atoi(buffer);
            }
            else if (lines == 6) {
                item->maxAmount = atoi(buffer);
            }
            else if (lines == 7) {
                item->minAmount = atoi(buffer);
            }
            else if (lines == 8) {
                item->icon = atoi(buffer);
            }
            else if (lines == 9) {
                item->duplicate = atoi(buffer);
            }
            else if (lines == 10) {
                item->thumbnail = atoi(buffer);
            }
            else if (lines == 11) {
                item->frames = atoi(buffer);
            }
            else if (lines == 12) {
                item->score = atoi(buffer);
            }

            lines++;
        }
    }

    fclose(file);

    return 0;
}

int itemtool_save(ItemData* data, const char *filename) {
    FILE* file;

    if (!(file = fopen(filename, "wb"))) {
        printf("Error creating item data file %s.\n", filename);
        return -1;
    }

    fwrite(data, sizeof(ItemData), 1, file);

    fclose(file);

    return 0;
}

int itemtool_create(const char *dst, const char *src) {
    ItemData data;

    if (itemtool_load(&data, src) != 0) {
        return -1;
    }

    if (itemtool_save(&data, dst) != 0) {
        return -2;
    }

    return 0;
}