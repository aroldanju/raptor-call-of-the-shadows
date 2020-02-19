#ifndef __itemtool_h
#define __itemtool_h

#include "../../item.h"

int itemtool_load(ItemData* item, const char* filename);

int itemtool_save(ItemData* item, const char *filename);

int itemtool_create(const char *dst, const char *src);

#endif  /* __itemtool_h */