#include "maptool.h"

#include <stdint.h>
#include <string.h>

#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>

#include "../../map.h"

int maptool_save_tiles(const TileData* tiles, uint32_t number, const char* filename)    {
    FILE* file;
    const char magic[] = {'T', 'I', 'L'};

    if (!(file = fopen(filename, "wb"))) {
        printf("Error creating tile data file %s.\n", filename);
        return -1;
    }

    fwrite(magic, 3, 1, file);
    fwrite(&number, sizeof(uint32_t), 1, file);
    fwrite(tiles, sizeof(TileData), number, file);

    fclose(file);

    return 0;
}

int maptool_create_tiles(const char *dst, const char *src) {
    TileData data[256];
    uint32_t number = 0;

    if (maptool_load_tiles(data, &number, src) != 0) {
        return -1;
    }

    if (maptool_save_tiles((const TileData*)data, number, dst) != 0) {
        return -2;
    }

    return 0;
}

int maptool_load_tiles(TileData* tiles, uint32_t *number, const char* filename)  {
    FILE* file;
    int i;
    char *tkn;
    char buffer[256];
    int bufferLength;
    int tileId;

    bufferLength = 256;
    *number = 0;

    file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening tiles file %s.\n", filename);
        return -1;
    }

    tileId = 0;

    while(fgets(buffer, bufferLength, file)) {
        if (buffer[0] != '#' && buffer[0] != ' ' && buffer[0] != '\n') {
            if (buffer[strlen(buffer) - 1] == '\n') {
                buffer[strlen(buffer) - 1] = '\0';
            }

            i = 0;

            tkn = strtok(buffer, ",");
            while (tkn) {

                if (i == 0) {
                    tiles[tileId].gid = atoi(tkn);
                }
                else if (i == 1) {
                    tiles[tileId].exchange = atoi(tkn);
                }
                else if (i == 2) {
                    tiles[tileId].strength = atoi(tkn);
                }
                else if (i == 3) {
                    tiles[tileId].score = atoi(tkn);
                }

                tkn = strtok(NULL, ",");
                i++;
            }

            tileId++;
        }
    }

    fclose(file);

    *number = tileId;

    return 0;
}

int maptool_create_map(const char* filename, const char *output)    {
    // Read .TMX file

    fflush(stdout);

    xmlDocPtr       doc;
    xmlNode         *nodeRoot, *node, *nodeLayer, *nodeObject;
    xmlAttribute    *attribute;

    uint32_t numObj = 0;

    Map map;

    int i, j;

    /*
    for (i = 0; i < MAP_MAX_ENEMIES; i++) {
        map.enemies[i].type = 0;
        map.enemies[i].x = 0;
        map.enemies[i].y = 0;
        map.enemies[i].behavior = 0;
    }

    for (i = 0; i < MAP_HEIGHT; i++) {
        for (j = 0; j < MAP_WIDTH; j++) {
            map.tiles[i][j] = 0;
        }
    }
    */

    //map_reset(&map);
    for (i = 0; i < MAP_HEIGHT; i++) {
        for (j = 0; j < MAP_WIDTH; j++) {
            map.tiles[i][j].gid = 0;
            map.tiles[i][j].enemy.type = -1;
            map.tiles[i][j].enemy.x = 0;
            map.tiles[i][j].enemy.y = 0;
            map.tiles[i][j].enemy.behavior = 0;
            map.tiles[i][j].enemy.launched = 0;
            map.tiles[i][j].enemy.gang = 0;
            map.tiles[i][j].enemy.item = -1;
            map.tiles[i][j].enemy.difficulty = 0;
            map.tiles[i][j].enemy.boss = 0;
        }
    }

    if ( !(doc = xmlReadFile(filename, NULL, 0))) {
        //printf("Error opening Tiled file %s\n", filename);
        return -1;
    }

    nodeRoot = xmlDocGetRootElement(doc);
    if ( nodeRoot == NULL || strcmp(nodeRoot->name, "map") != 0 ) {
        //printf("Error reading Tiled file %s. Invalid root node '%s'.", filename, nodeRoot->name);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return -2;
    }

    // Reads root node attributes
    for (attribute = nodeRoot->properties; attribute != NULL; attribute = attribute->next ) {
        if ( strcmp(attribute->name, "width") == 0 ) {
            xmlChar* value = xmlNodeListGetString(nodeRoot->doc, attribute->children, 1);
            if (atoi(value) != 9) {
                //printf("Invalid map width %d\n", atoi(value));
                free(value);
                return -3;
            }
            free(value);
        }
        else if ( strcmp(attribute->name, "height") == 0 ) {
            xmlChar* value = xmlNodeListGetString(nodeRoot->doc, attribute->children, 1);
            if (atoi(value) != 150) {
                //printf("Invalid map height %d\n", atoi(value));
                free(value);
                return -4;
            }
            free(value);
        }
    }

    node = nodeRoot->children;
    while ( node ) {
        /*
        if (strcmp(node->name, "properties") == 0) {
            nodeProperty = node->children;
            while ( nodeProperty ) {
                if ( strcmp(nodeProperty->name, "property") == 0 ) {
                    for ( attribute = nodeProperty->properties; attribute != NULL; attribute = attribute->next ) {
                        if ( strcmp(attribute->name, "name") == 0 ) {
                            xmlChar* value = xmlNodeListGetString(nodeProperty->doc, attribute->children, 1);
                            strcpy(map->properties[map->numProperties].name, value);
                            free(value);
                        }
                        else if ( strcmp(attribute->name, "value") == 0 ) {
                            xmlChar* value = xmlNodeListGetString(nodeProperty->doc, attribute->children, 1);
                            strcpy(map->properties[map->numProperties].value, value);
                            free(value);
                        }
                    }

                    map->numProperties++;
                }

                nodeProperty = nodeProperty->next;
            }
        }
        else
            */
        if (strcmp(node->name, "layer") == 0) {
            nodeLayer = node->children;
            while (nodeLayer) {
                if (strcmp(nodeLayer->name, "data") == 0 ) {

                    int x = 0, y = 0;

                    xmlChar* data = xmlNodeGetContent(nodeLayer);

                    char *pt;
                    pt = strtok(data, ",");
                    while (pt != NULL) {
                        //map.tiles[y][x] = atoi(pt);
                        map.tiles[y][x].gid = atoi(pt);

                        x++;
                        if (x >= MAP_WIDTH) {
                            x = 0;
                            y++;
                        }

                        pt = strtok(NULL, ",");
                    }

                    free(data);
                }

                nodeLayer = nodeLayer->next;
            }
        }
        else if ( strcmp(node->name, "objectgroup") == 0 ) {
            nodeObject = node->children;
            while (nodeObject) {
                if (strcmp(nodeObject->name, "object") == 0) {

                    /*
                    struct _map_enemy *enemy = &map.enemies[numObj];

                    enemy->item = -1;
                    enemy->difficulty = 0;
                    enemy->x = enemy->y = 0;
                    enemy->type = 0;
                    enemy->behavior = 0;
                    enemy->gang = 0;
                    enemy->launched = 0;
                    */

                    int32_t type = -1;
                    int32_t behaviour = -1;
                    int32_t enemyX = 0, enemyY = 0;

                    for (attribute = nodeObject->properties; attribute != NULL; attribute = attribute->next ) {
                        if ( strcmp(attribute->name, "name") == 0 ) {
                            xmlChar* value = xmlNodeListGetString(nodeObject->doc, attribute->children, 1);
                            //enemy->type = atoi(value);
                            type = atoi(value);
                            free(value);
                        }
                        else if ( strcmp(attribute->name, "type") == 0 ) {
                            xmlChar* value = xmlNodeListGetString(nodeObject->doc, attribute->children, 1);
                            //enemy->behavior = atoi(value);
                            behaviour = atoi(value);
                            free(value);
                        }
                        else if ( strcmp(attribute->name, "x") == 0 ) {
                            xmlChar* value = xmlNodeListGetString(nodeObject->doc, attribute->children, 1);
                            //enemy->x = atoi(value) / 32;
                            enemyX = atoi(value) / 32;
                            free(value);
                        }
                        else if ( strcmp(attribute->name, "y") == 0 ) {
                            xmlChar* value = xmlNodeListGetString(nodeObject->doc, attribute->children, 1);
                            //enemy->y = atoi(value) / 32;
                            enemyY = atoi(value) / 32;
                            free(value);
                        }
                    }

                    /*
                    <properties>
                    <property name="difficulty" type="int" value="2"/>
                                                                 </properties>
                                                                 */

                    struct _map_enemy *enemy = &map.tiles[enemyY][enemyX].enemy;

                    enemy->behavior = (behaviour > 0 ? behaviour : 0);
                    enemy->type = (type > 0 ? type : 0);
                    enemy->x = enemyX;
                    enemy->y = enemyY;

                    xmlNode *nodeProperties = nodeObject->children;
                    while (nodeProperties) {
                        if (strcmp((const char*)nodeProperties->name, "properties") == 0) {
                            xmlNode *nodeProperty = nodeProperties->children;
                            while (nodeProperty) {
                                if (strcmp((const char*)nodeProperty->name, "property") == 0) {
                                    for (attribute = nodeProperty->properties; attribute != NULL; attribute = attribute->next ) {
                                        if (strcmp(attribute->name, "name") == 0 ) {
                                            xmlChar* value = xmlNodeListGetString(nodeProperty->doc, attribute->children, 1);
                                            if (strcmp(value, "difficulty") == 0) {
                                                attribute = attribute->next;
                                                attribute = attribute->next;
                                                value = xmlNodeListGetString(nodeProperty->doc, attribute->children, 1);
                                                enemy->difficulty = atoi(value);
                                            }
                                            else if (strcmp(value, "item") == 0) {
												attribute = attribute->next;
												attribute = attribute->next;
												value = xmlNodeListGetString(nodeProperty->doc, attribute->children, 1);
												enemy->item = atoi(value);
											}
                                            else if (strcmp(value, "gang") == 0) {
												attribute = attribute->next;
												attribute = attribute->next;
												value = xmlNodeListGetString(nodeProperty->doc, attribute->children, 1);
												enemy->gang = atoi(value);
											}
                                            else if (strcmp(value, "boss") == 0) {
												attribute = attribute->next;
												attribute = attribute->next;
												value = xmlNodeListGetString(nodeProperty->doc, attribute->children, 1);
												enemy->boss = atoi(value);
											}

                                            free(value);
                                        }
                                    }
                                }

                                nodeProperty = nodeProperty->next;
                            }

                            break;
                        }

                        nodeProperties = nodeProperties->next;
                    }


                    /*
                    xmlNode *nodeProperty = nodeObject->children;
                    if (nodeProperty) {
                        printf("PROP: %s\n", nodeProperty->name);
                        nodeProperty = nodeProperty->children;
                        while (nodeProperty) {

                            for (attribute = nodeProperty->properties; attribute != NULL; attribute = attribute->next ) {
                                printf("%s\n", attribute->name);
                                if (strcmp(attribute->name, "name") == 0 ) {
                                    xmlChar* value = xmlNodeListGetString(nodeProperty->doc, attribute->children, 1);
                                    if (strcmp(value, "behaviour") == 0) {
                                        attribute = attribute->next;
                                        attribute = attribute->next;
                                        free(value);
                                        value = xmlNodeListGetString(nodeProperty->doc, attribute->children, 1);
                                        enemy->behavior = atoi(value);
                                        free(value);

                                        printf("BEHAVIOUR: %d\n", enemy->behavior);
                                    }

                                    free(value);
                                }
                            }

                            nodeProperty = nodeProperty->next;
                        }
                    }
                    */

                    numObj++;

                    if (numObj >= MAP_MAX_ENEMIES) {
                        printf("Max enemies reached\n");
                        break;
                    }
                }

                nodeObject = nodeObject->next;
            }
        }

        node = node->next;
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();

    FILE* file = fopen(output, "wb");
    if (!file) {
        //printf("Error creating output map file %s\n", output);
        return -5;
    }

    //fwrite(&map.enemies, sizeof(struct _map_enemy) * MAP_MAX_ENEMIES, 1, file);
    //fwrite(&map.tiles, sizeof(uint16_t) * MAP_HEIGHT * MAP_WIDTH, 1, file);

    fwrite(&map.tiles, sizeof(struct _map_tile), MAP_HEIGHT * MAP_WIDTH, file);

    fclose(file);

    return 0;
}

int map_to_tmx(const char* mapfile, const char *output)    {
    FILE *fmap;
    uint32_t tiles[1350];
    int i;
    char gid[8];

    fmap = fopen(mapfile, "rb");
    if (!fmap) {
        printf("Error reading MAP file %s.\n", mapfile);
        return 1;
    }

    // Skip first 12 bytes
    fseek(fmap, 12, SEEK_SET);

    // Read tile info
    fread(tiles, 1350 * sizeof(uint32_t), 1, fmap);

    fclose(fmap);

    fmap = fopen(output, "wb");
    if (!fmap) {
        printf("Error creating OUTPUT %s file.\n", output);
        return 1;
    }

    for (i = 0; i < 1350; i++) {
        if (i == 1350 - 1) {
            sprintf(gid, "%d", tiles[i] + 1);
        }
        else {
            sprintf(gid, "%d,", tiles[i] + 1);
        }

        fwrite(gid, strlen(gid), 1, fmap);

        if ((i+1) % 9 == 0) {
            fwrite("\n", 1, 1, fmap);
        }
    }

    fclose(fmap);
}

int maptool_itm_to_tiles(const char *filename, const char *output) {
    FILE *fin, *fout;
    uint32_t exchange;
    uint16_t strength, score;
    char tileLine[256];
    uint32_t tileId;

    if (!(fin = fopen(filename, "rb"))) {
        printf("maptool_itm_to_tiles(): error opening ITM file '%s'.\n", filename);
        return -1;
    }

    if (!(fout = fopen(output, "wb"))) {
        printf("maptool_itm_to_tiles(): error creating TILE DATA file %s\n", filename);
        return -2;
    }

    tileId = 0;

    while (!feof(fin)) {
        fread(&exchange, 4, 1, fin);
        fread(&strength, 2, 1, fin);
        fread(&score, 2, 1, fin);

        if (strength != 0xffff) {
            sprintf(tileLine, "%d,%d,%d,%d\n", tileId, exchange, strength, score);
            fwrite(tileLine, strlen(tileLine), 1, fout);
        }

        tileId++;
    }

    fclose(fout);
    fclose(fin);

    return 0;
}
