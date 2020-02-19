#include "behaviourtool.h"
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <stdint.h>

int txt_to_behaviour(Behaviour* behaviour, const char *filename) {
    FILE *file;
    char *tkn1, *tkn2;
    int bufferLength = 255;
    int i;
    char buffer[bufferLength];
    int lines = 0;

    behaviour->numSteps = 0;
    behaviour->infinite = 0;
    behaviour->steps = NULL;
    behaviour->id = 0;

    file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening behaviour text file %s\n", filename);
        return -1;
    }

    behaviour->steps = (struct _behaviour_step*)malloc(sizeof(struct _behaviour_step) * 256);

    while(fgets(buffer, bufferLength, file)) {

        if (buffer[0] != '#' && buffer[0] != ' ' && buffer[0] != '\n') {

            if (buffer[strlen(buffer) - 1] == '\n') {
                buffer[strlen(buffer) - 1] = '\0';
            }

            if (lines == 0) {
                behaviour->id = atoi(buffer);
            }
            else if (lines == 1) {
                behaviour->infinite = atoi(buffer);
            }
            else {

                behaviour->numSteps++;
                i = 0;


                behaviour->steps[behaviour->numSteps - 1].xvel = 0.0f;
                behaviour->steps[behaviour->numSteps - 1].maxxvel = 0.0f;
                behaviour->steps[behaviour->numSteps - 1].yvel = 0.0f;
                behaviour->steps[behaviour->numSteps - 1].maxyvel = 0.0f;
                behaviour->steps[behaviour->numSteps - 1].delay = 0;
                behaviour->steps[behaviour->numSteps - 1].x = 0;
                behaviour->steps[behaviour->numSteps - 1].y = 0;
                behaviour->steps[behaviour->numSteps - 1].weapon = 0;

                tkn1 = strtok(buffer, " ");
                if (tkn1 != NULL) {
                    // Check command
                    if (strcmp(tkn1, "MOVE_X") == 0) {
                        i = 0;
                        behaviour->steps[behaviour->numSteps - 1].command = BEHAVIOURCOMMAND_MOVEX;
                        behaviour->steps[behaviour->numSteps - 1].xvel = 0.0f;
                        behaviour->steps[behaviour->numSteps - 1].maxxvel = 0.0f;
                        tkn1 = strtok(NULL, " ");
                        tkn2 = strtok(tkn1, ",");
                        while (tkn2) {
                            if (i == 0) {
                                behaviour->steps[behaviour->numSteps - 1].xvel = atof(tkn2);
                            }
                            else if (i == 1) {
                                behaviour->steps[behaviour->numSteps - 1].maxxvel = atof(tkn2);
                            }
                            i++;
                            tkn2 = strtok(NULL, ",");
                        }
                    }
                    else if (strcmp(tkn1, "MOVE_Y") == 0) {
                        i = 0;
                        behaviour->steps[behaviour->numSteps - 1].command = BEHAVIOURCOMMAND_MOVEY;
                        behaviour->steps[behaviour->numSteps - 1].yvel = 0.0f;
                        behaviour->steps[behaviour->numSteps - 1].maxyvel = 0.0f;
                        tkn1 = strtok(NULL, " ");
                        tkn2 = strtok(tkn1, ",");
                        while (tkn2) {
                            if (i == 0) {
                                behaviour->steps[behaviour->numSteps - 1].yvel = atof(tkn2);
                            }
                            else if (i == 1) {
                                behaviour->steps[behaviour->numSteps - 1].maxyvel = atof(tkn2);
                            }
                            i++;
                            tkn2 = strtok(NULL, ",");
                        }
                    }
                    else if (strcmp(tkn1, "SHOT") == 0) {
                        i = 0;
                        behaviour->steps[behaviour->numSteps - 1].command = BEHAVIOURCOMMAND_SHOT;
                        behaviour->steps[behaviour->numSteps - 1].target = FALSE;
                        behaviour->steps[behaviour->numSteps - 1].weapon = 0;
                        behaviour->steps[behaviour->numSteps - 1].x = 0;
                        behaviour->steps[behaviour->numSteps - 1].y = 0;
                        tkn1 = strtok(NULL, " ");
                        tkn2 = strtok(tkn1, ",");
                        while (tkn2) {
                            if (i == 0) {
                                behaviour->steps[behaviour->numSteps - 1].weapon = atoi(tkn2);
                            }
                            else if (i == 1) {
                                behaviour->steps[behaviour->numSteps - 1].x = atoi(tkn2);
                            }
                            else if (i == 2) {
                                behaviour->steps[behaviour->numSteps - 1].y = atoi(tkn2);
                            }
                            else if (i == 3) {
                                if (strcmp(tkn2, "TARGET") == 0) {
                                    behaviour->steps[behaviour->numSteps - 1].target = TRUE;
                                }
                            }
                            i++;
                            tkn2 = strtok(NULL, ",");
                        }
                    }
                    else if (strcmp(tkn1, "DELAY") == 0) {
                        i = 0;
                        behaviour->steps[behaviour->numSteps - 1].command = BEHAVIOURCOMMAND_DELAY;
                        tkn1 = strtok(NULL, " ");
                        behaviour->steps[behaviour->numSteps - 1].delay = atoi(tkn1);
                    }
                    else if (strcmp(tkn1, "MOVE_TARGET") == 0) {
                        behaviour->steps[behaviour->numSteps - 1].command = BEHAVIOURCOMMAND_MOVETARGET;
                        tkn1 = strtok(NULL, " ");
                        tkn2 = strtok(tkn1, ",");
                        while (tkn2) {
                            if (i == 0) {
                                behaviour->steps[behaviour->numSteps - 1].xvel = atof(tkn2);
                                behaviour->steps[behaviour->numSteps - 1].maxxvel = atof(tkn2);

                            }
                            else {
                                behaviour->steps[behaviour->numSteps - 1].maxxvel = atof(tkn2);
                            }
                            i++;
                            tkn2 = strtok(NULL, ",");
                        }
                    }
                    else if (strcmp(tkn1, "GOTO") == 0) {
                        behaviour->steps[behaviour->numSteps - 1].command = BEHAVIOURCOMMAND_GOTO;
                        tkn1 = strtok(NULL, " ");
                        behaviour->steps[behaviour->numSteps - 1].data1 = atoi(tkn1);
                    }
                    else if (strcmp(tkn1, "SET_ANGLE") == 0) {
                        behaviour->steps[behaviour->numSteps - 1].command = BEHAVIOURCOMMAND_SET_ANGLE;
                        tkn1 = strtok(NULL, " ");
                        behaviour->steps[behaviour->numSteps - 1].angle = atof(tkn1);
                    }
                    else if (strcmp(tkn1, "SET_VELOCITY") == 0) {
                        behaviour->steps[behaviour->numSteps - 1].command = BEHAVIOURCOMMAND_SET_VELOCITY;
                        tkn1 = strtok(NULL, " ");
                        behaviour->steps[behaviour->numSteps - 1].xvel = atof(tkn1);
                        behaviour->steps[behaviour->numSteps - 1].yvel = behaviour->steps[behaviour->numSteps - 1].xvel;
                    }
                    else {
                        printf("Unknown command %s.\n", tkn1);
                        return -2;
                    }
                }
            }

            lines++;
        }
    }

    fclose(file);

    return 0;
}

int save_behaviour(Behaviour* behaviour, const char *filename) {
    FILE *file;

    file = fopen(filename, "wb");
    if (!file) {
        printf("Error creating output behaviour file %s\n", filename);
        return 1;
    }

    fwrite(&behaviour->id, sizeof(uint32_t), 1, file);
    fwrite(&behaviour->numSteps, sizeof(uint8_t), 1, file);
    fwrite(&behaviour->infinite, sizeof(uint8_t), 1, file);
    fwrite(behaviour->steps, sizeof(struct _behaviour_step) * behaviour->numSteps, 1, file);

    fclose(file);

    return 0;
}


int behaviourtool_create_behaviour(const char* src, const char* output)    {
    Behaviour behaviour;

    if (txt_to_behaviour(&behaviour, src) != 0) {
        printf("behaviourtool_create_behaviour(): error reading raw behaviour\n");
        return -1;
    }

    if (save_behaviour(&behaviour, output) != 0) {
        printf("behaviourtool_create_behaviour(): error saving behaviour file.\n");
        return -1;
    }

    return 0;
}
