#include "shiptool.h"

int shiptool_load(ShipData* ship, const char* filename) {
    FILE* file;
    int lines;
    unsigned char frameId;
    char *tkn;
    char buffer[256];
    int bufferLength;
    int i;
    unsigned char hotId;

    lines = 0;
    bufferLength = 256;
    hotId = 0;

    file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening ship file %s.\n", filename);
        return -1;
    }

    ship->magic[0] = 'S';
    ship->magic[1] = 'H';
    ship->magic[2] = 'P';

    ship->hotNumber = 0;
    ship->propellerNumber = 0;

    for (i = 0; i < MAX_SHIP_FRAMES; i++) {
    	ship->frames[i] = 0;
    }

    while(fgets(buffer, bufferLength, file)) {
        if (buffer[0] != '#' && buffer[0] != ' ' && buffer[0] != '\n') {
            if (buffer[strlen(buffer) - 1] == '\n') {
                buffer[strlen(buffer) - 1] = '\0';
            }

            if (lines == 0) {
                ship->energy = atoi(buffer);
            }
            else if (lines == 1) {
                ship->ground = atoi(buffer);
            }
            else if (lines == 2) {
                // Number of frames, Delay between frames
                int k = 0;
                tkn = strtok(buffer, ",");
                while (tkn) {

                    if (k == 0) {
                        ship->numFrames = atoi(tkn);
                    }
                    else {
                        ship->delayFrames = atoi(tkn);
                    }

                    tkn = strtok(NULL, ",");
                    k++;
                }

                ship->numFrames = atoi(buffer);
            }
			else if (lines == 3) {
				frameId = 0;
				tkn = strtok(buffer, ",");
				while (tkn) {
					ship->frames[frameId] = atoi(tkn);
					tkn = strtok(NULL, ",");
					frameId++;
					if (hotId >= MAX_SHIP_FRAMES){
						break;
					}
				}
			}
            else if (lines == 4) {
                ship->score = atoi(buffer);
            }
            else if (lines == 5) {
                ship->hotNumber = atoi(buffer);
            }
            else if (lines >= 6 && lines < 6 + ship->hotNumber){

                frameId = 0;

                tkn = strtok(buffer, ",");
                while (tkn) {
                    if (frameId == 0) {
                        ship->hots[hotId].x = atoi(tkn);
                    }
                    else if (frameId == 1) {
                        ship->hots[hotId].y = atoi(tkn);
                    }
                    else if (frameId == 2) {
                        ship->hots[hotId].w = atoi(tkn);
                    }
                    else if (frameId == 3) {
                        ship->hots[hotId].h = atoi(tkn);
                    }

                    tkn = strtok(NULL, ",");
                    frameId++;
                }

                hotId++;
                /*
                if (hotId >= MAX_SHIP_HOTS){
                    continue;
                }
                */
            }
            else if (lines >= 6 + ship->hotNumber && lines < 6 + ship->hotNumber + 1) {
                ship->propellerNumber = atoi(buffer);
                hotId = 0;
            }
            else if (lines >= 6 + ship->hotNumber + 1 && lines < 6 + ship->hotNumber + 1 + ship->propellerNumber){
                frameId = 0;

                tkn = strtok(buffer, ",");
                while (tkn) {
                    if (frameId == 0) {
                        ship->propellers[hotId].x = atoi(tkn);
                    }
                    else if (frameId == 1) {
                        ship->propellers[hotId].y = atoi(tkn);
                    }
                    else if (frameId == 2) {
                        ship->propellers[hotId].width = atoi(tkn);
                    }

                    tkn = strtok(NULL, ",");
                    frameId++;
                }

                hotId++;
                if (hotId >= MAX_SHIP_PROPELLERS){
                    continue;
                }
            }

            lines++;
        }
    }

    fclose(file);

    return 0;
}

int shiptool_save(ShipData* data, const char *filename) {
    FILE* file;

    if (!(file = fopen(filename, "wb"))) {
        printf("Error creating ship data file %s.\n", filename);
        return -1;
    }

    fwrite(data, sizeof(ShipData), 1, file);

    fclose(file);

    return 0;
}

int shiptool_create(const char *dst, const char *src) {
    ShipData data;

    if (shiptool_load(&data, src) != 0) {
        return -1;
    }

    if (shiptool_save(&data, dst) != 0) {
        return -2;
    }

    return 0;
}