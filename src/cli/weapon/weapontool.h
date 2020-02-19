#ifndef __weapontool_h
#define __weapontool_h

#include "../../weapon.h"

int weapontool_load(WeaponData* weapon, const char* filename);

int weapontool_save(WeaponData* data, const char *filename);

int weapontool_create(const char *dst, const char *src);

#endif  /* __weapontool_h */