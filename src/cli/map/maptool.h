#ifndef __maptool_h
#define __maptool_h

#include <stdint.h>
#include "../../map.h"

enum {
    MAPTOOL_ERROR_NONE = 0,
    MAPTOOL_ERROR_OPEN = -1,
    MAPTOOL_ERROR_ROOT = -2,
    MAPTOOL_ERROR_WIDTH = -3,
    MAPTOOL_ERROR_HEIGHT = -4,
    MAPTOOL_ERROR_SAVE = -5
};

int maptool_create_map(const char* filename, const char *output);

int maptool_create_tiles(const char* filename, const char* output);

/**
 *  Read MAP Raptor file format and save an TMX layer
 */
int map_to_tmx(const char* mapfile, const char *output);

int maptool_load_tiles(TileData* tiles, uint32_t *number, const char* filename);
int maptool_save_tiles(const TileData* tiles, uint32_t number, const char* filename);

/* Transform tile ITM */
int maptool_itm_to_tiles(const char *filename, const char *output);

#endif  // __maptool_h