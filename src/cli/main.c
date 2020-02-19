#include "map/maptool.h"

#include "../../vendor/res/src/res.h"
#include "image/imagetool.h"
#include "behaviour/behaviourtool.h"
#include "weapon/weapontool.h"
#include "ship/shiptool.h"
#include "item/itemtool.h"

#include <stdio.h>
#include <dirent.h>

int create_files(const char* srcdir, const char* dstdir) {
    DIR *d;
    struct dirent *dir;
    char src[256], dst[256];
    char filename[16];
    char palette[256];

    d = opendir(srcdir);
    if (!d) {
        printf("Error opening source directory %s.\n", srcdir);
        return 1;
    }

    // Start with palette
    strcpy(src, srcdir);
    if (src[strlen(src) - 1] != '/') { sprintf(src, "%s/", srcdir); }
    strcat(src, "PALETTE.BMP");
    strcpy(palette, dstdir);
    strcat(palette, "PALETTE.PAL");
    imagetool_create_palette(src, palette);

    while ((dir = readdir(d)) != NULL) {
        if (strlen(dir->d_name) >= 16) {
            printf("   Name for file %s too long %d.\n", dir->d_name, (int)strlen(dir->d_name));
        }
        else {

            strcpy(src, srcdir);
            if (src[strlen(src) - 1] != '/') { sprintf(src, "%s/", srcdir); }
            strcat(src, dir->d_name);

            strcpy(dst, dstdir);
            strcat(dst, dir->d_name);
            dst[strlen(dst) - 3] = '\0';    // remove extension

            if (strncmp(dir->d_name + strlen(dir->d_name) - 3, "tmx", 3) == 0 ||
                strncmp(dir->d_name + strlen(dir->d_name) - 3, "TMX", 3) == 0) {
                strcat(dst, "MAP"); // add extension
                maptool_create_map(src, dst);
                //printf("Map > %s to %s\n", src, dst);
            }
            else if (strncmp(dir->d_name + strlen(dir->d_name) - 3, "bmp", 3) == 0 ||
                     strncmp(dir->d_name + strlen(dir->d_name) - 3, "BMP", 3) == 0) {
                // Skip palette bmp
                if(strcmp(dir->d_name, "PALETTE.BMP") == 0) {
                    continue;
                }
                strcat(dst, "IMG"); // add extension
                imagetool_create_image(src, dst, palette);
                //printf("Image > %s to %s\n", src, dst);
            }
            else if (strncmp(dir->d_name + strlen(dir->d_name) - 3, "txt", 3) == 0 ||
                     strncmp(dir->d_name + strlen(dir->d_name) - 3, "TXT", 3) == 0) {
                strcat(dst, "BEH"); // add extension
                behaviourtool_create_behaviour(src, dst);
                //printf("Behaviour > %s to %s\n", src, dst);
            }
            else if (strncmp(dir->d_name + strlen(dir->d_name) - 3, "wpn", 3) == 0 ||
                     strncmp(dir->d_name + strlen(dir->d_name) - 3, "WPN", 3) == 0) {
                strcat(dst, "WPN"); // add extension
                weapontool_create(dst, src);
                //printf("Weapon > %s to %s\n", src, dst);
            }
            else if (strncmp(dir->d_name + strlen(dir->d_name) - 3, "shp", 3) == 0 ||
                     strncmp(dir->d_name + strlen(dir->d_name) - 3, "SHP", 3) == 0) {
                strcat(dst, "SHP"); // add extension
                shiptool_create(dst, src);
                //printf("Ship > %s to %s\n", src, dst);
            }
            else if (strncmp(dir->d_name + strlen(dir->d_name) - 3, "til", 3) == 0 ||
                     strncmp(dir->d_name + strlen(dir->d_name) - 3, "TIL", 3) == 0) {
                strcat(dst, "TIL"); // add extension
                maptool_create_tiles(dst, src);
                //printf("Tiles > %s to %s\n", src, dst);
            }
            else if (strncmp(dir->d_name + strlen(dir->d_name) - 3, "itm", 3) == 0 ||
                     strncmp(dir->d_name + strlen(dir->d_name) - 3, "ITM", 3) == 0) {
                strcat(dst, "ITM"); // add extension
                itemtool_create(dst, src);
                //printf("Item > %s to %s\n", src, dst);
            }
        }
    }

    closedir(d);

    return 0;
}

int main(int argc, char* argv[]) {
    RES res;
    char srcdir[256], dstdir[256], resfile[256], outputfile[256];

    // ./raptor-cli ../resources/source/ ../resources/ready/ ../resources/RESOURCES.TXT ../resources/dist/RESOURCES.RES

    printf("\n");
    printf("==========================================\n");
    printf("Raptor: Call of the Shadows Packager\n");
    printf("Developed by @roldju\n");
    printf("===========================================\n");
    printf("\n");

    if (argc < 5) {
        printf("usage %s srcdir dstdir resourcesfile outputfile\n", argv[0]);
        return 1;
    }

    strcpy(srcdir, argv[1]);
    strcpy(dstdir, argv[2]);
    strcpy(resfile, argv[3]);
    strcpy(outputfile, argv[4]);

    create_files(srcdir, dstdir);

    printf("Creating package...\n");
    if (res_create(&res, resfile, dstdir) != 0) {
        printf("Error creating RES file.\n");
        return 1;
    }

    if (res_save(&res, outputfile) != 0) {
        printf("Error saving RES file.\n");
        return 1;
    }

    printf("\n  Success\n\n");
    printf("  Packaged %d/%d files in %d bytes\n", res.header.fatSize, res.header.fatSize - res.errors, res.size);
    if (res.errors > 0) {
        printf("    %d files were not packaged.\n", res.errors);
    }
    printf("\n");

    res_dispose(&res);

    return 0;
}