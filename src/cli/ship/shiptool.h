#ifndef __shiptool_h
#define __shiptool_h

#include "../../ship.h"

//BOOL ship_load_from_mem(ShipData* ship, const char *data, uint32_t length);

int shiptool_load(ShipData* ship, const char* filename);

int shiptool_save(ShipData* data, const char *filename);

int shiptool_create(const char *dst, const char *src);

#endif  /* __shiptool_h */