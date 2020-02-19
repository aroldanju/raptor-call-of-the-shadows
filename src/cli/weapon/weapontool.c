#include "weapontool.h"

int weapontool_load(WeaponData* weapon, const char* filename) {
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
        printf("Error opening weapon file %s.\n", filename);
        return -1;
    }

    weapon->magic[0] = 'W';
    weapon->magic[1] = 'P';
    weapon->magic[2] = 'N';

    while(fgets(buffer, bufferLength, file)) {
        if (buffer[0] != '#' && buffer[0] != ' ' && buffer[0] != '\n') {
            if (buffer[strlen(buffer) - 1] == '\n') {
                buffer[strlen(buffer) - 1] = '\0';
            }

            if (lines == 0) {
                weapon->itemId = atoi(buffer);
            }
            else if (lines == 1) {
                weapon->alwaysEquipped = atoi(buffer);
            }
            else if (lines == 2) {
                weapon->delay = atoi(buffer);
            }
            else if (lines == 3) {
                weapon->twin = atoi(buffer);
            }
            else if (lines == 4) {
                weapon->air = atoi(buffer);
            }
            else if (lines == 5) {
                weapon->ground = atoi(buffer);
            }
            else if (lines == 6) {
                weapon->behaviour = atoi(buffer);
            }
            else if (lines == 7) {
                weapon->damage = atoi(buffer);
            }
            else if (lines == 8) {
                weapon->soundId = atoi(buffer);
            }
            else if (lines == 9) {
                weapon->bulletFramesNumber = atoi(buffer);
            }
            else if (lines == 10) {
                tkn = strtok(buffer, ",");
                while (tkn) {
                    weapon->bulletFrames[frameId++] = atoi(tkn);
                    tkn = strtok(NULL, ",");

                    if (frameId >= MAX_WEAPONDATA_BULLET_FRAMES){
                        break;
                    }
                }
            }

            lines++;
        }
    }

    fclose(file);

    return 0;
}

int weapontool_save(WeaponData* data, const char *filename) {
    FILE* file;

    if (!(file = fopen(filename, "wb"))) {
        printf("Error creating weapon data file %s.\n", filename);
        return -1;
    }

    fwrite(data, sizeof(WeaponData), 1, file);

    fclose(file);

    return 0;
}

int weapontool_create(const char *dst, const char *src) {
    WeaponData data;

    if (weapontool_load(&data, src) != 0) {
        return -1;
    }

    if (weapontool_save(&data, dst) != 0) {
        return -2;
    }

    return 0;
}