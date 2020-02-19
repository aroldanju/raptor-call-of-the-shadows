#ifndef __imagetool_h
#define __imagetool_h

#include "image.h"

int imagetool_create_image(const char *source, const char *output, const char *palette);
int imagetool_create_palette(const char *source, const char *output);

/**
 *  Creates a palette file PAL from a BMP palette
 */
int bmp_to_pal(PAL* pal, const char* filename) ;

int bmp_to_img(IMG* img, const char* filename, const char* palette_file);

int save_pal(PAL* pal, const char *filename);

int save_img(IMG* img, const char *filename);

/**
 *  Creates a BMP palette from Raptor palette file
 */
int pal_to_bmp(const char *filename, const char *output);

int bmp_to_gpl(const char* filename, const char* output);

#endif  // __imagetool_h