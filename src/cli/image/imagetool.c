#include "imagetool.h"

int imagetool_create_image(const char *source, const char *output, const char* palette)  {
    IMG img;

    if (bmp_to_img(&img, source, palette) != 0) {
        printf("imagetool_create_image(): error creating image %s from %s\n", output, source);
        return -1;
    }

    if (save_img(&img, output) != 0) {
        printf("imagetool_create_image(): error saving image %s\n", output);
        return -1;
    }

    return 0;
}

int imagetool_create_palette(const char *source, const char *output) {
    PAL pal;

    if (bmp_to_pal(&pal, source) != 0) {
        printf("imagetool_create_palette(): error creating palette %s from %s\n", output, source);
        return 1;
    }

    if (save_pal(&pal, output) != 0) {
        printf("imagetool_create_palette(): error saving palette %s\n", output);
        return -1;
    }

    return 0;
}

int bmp_to_pal(PAL* pal, const char* filename) {
    SDL_Surface* surface;
    SDL_Color color;
    int i, j;

    surface = SDL_LoadBMP(filename);
    if (!surface) {
        return 1;
    }

    for (i = 0; i < surface->h; i++) {
        for (j = 0; j < surface->w; j++) {
            uint32_t pixel = surface_get_pixel(surface, j, i);
            SDL_GetRGB(pixel, surface->format, &color.r, &color.g, &color.b);
            pal->color[i * 16 + j][0] = color.r;
            pal->color[i * 16 + j][1] = color.g;
            pal->color[i * 16 + j][2] = color.b;
        }
    }

    SDL_FreeSurface(surface);

    return 0;
}

int save_pal(PAL* pal, const char *filename) {
    FILE *file;

    file = fopen(filename, "wb");
    if (!file) {
        printf("Error creating output palette file %s\n", filename);
        return 1;
    }

    fwrite(pal, sizeof(PAL), 1, file);

    fclose(file);

    return 0;
}

int save_img(IMG* img, const char *filename) {
    FILE *file;

    file = fopen(filename, "wb");
    if (!file) {
        printf("Error creating output image file %s\n", filename);
        return 1;
    }

    fwrite(&img->width, sizeof(uint32_t), 1, file);
    fwrite(&img->height, sizeof(uint32_t), 1, file);
    fwrite(img->data, sizeof(uint8_t) * img->width * img->height, 1, file);

    fclose(file);

    return 0;
}

int bmp_to_img(IMG* img, const char* filename, const char* palette_file) {
    SDL_Surface *surface;
    PAL palette;
    int i, j;
    int k, found;

    FILE *fpal = fopen(palette_file, "rb");
    if (!fpal) {
        printf("Error opening palette %s\n", palette_file);
        return -1;
    }

    fread(&palette, sizeof(PAL), 1, fpal);

    fclose(fpal);

    surface = SDL_LoadBMP(filename);
    if (!surface) {
        return -1;
    }

    img->width = surface->w;
    img->height = surface->h;
    img->data = (uint8_t*)malloc(sizeof(uint8_t) * surface->w * surface->h);

    for (i = 0; i < surface->h; i++) {
        for (j = 0; j < surface->w; j++) {
            uint32_t pixel = surface_get_pixel(surface, j, i);
            SDL_Color color;
            SDL_GetRGB(pixel, surface->format, &color.r, &color.g, &color.b);

            found = 0;

            for (k = 0; k < 256; k++) {
                if (palette.color[k][0] == color.r && palette.color[k][1] == color.g && palette.color[k][2] == color.b) {
                    img->data[i * surface->w + j] = (uint8_t)k;
                    found = 1;
                    break;
                }
            }

            if (found == 0) {
                printf("Color (%d, %d, %d) not found in palette.\n", color.r, color.g, color.b);
                img->data[i * surface->w + j] = 0;
            }
        }
    }

    SDL_FreeSurface(surface);

    return 0;
}

int pal_to_bmp(const char *filename, const char *output) {
    char buff[3];
    int i;
    SDL_Surface* surface;
    uint8_t red, green, blue;
    Uint32* surfacePixels;
    FILE* file;

    if((file = fopen(filename, "rb")) != 0){

        // Crea la superficie para guardarla en BMP
        surface = SDL_CreateRGBSurface(0, 16, 16, 32, 0, 0, 0, 0);

        for (i = 0; i < 256; i++)	{
            fread(buff, 3, 1, file);

            red = buff[0] * 4;
            green = buff[1] * 4;
            blue = buff[2] * 4;

            if(red==36&&green==52&&blue==164){
                red=101;
                green=81;
                blue=69;
            }

            surfacePixels = (Uint32*)surface->pixels;
            surfacePixels[i] = SDL_MapRGB(surface->format, red, green, blue);

            printf(" * Color #%d = (%d,%d,%d)\n", i, red, green, blue);
        }

        SDL_SaveBMP(surface, output);
        SDL_FreeSurface(surface);

        fclose(file);
    }
    else {
        printf("Error cargando fichero %s\n", filename);
        return -1;
    }

    return 0;
}

int bmp_to_gpl(const char* filename, const char* output) {
    SDL_Surface* surface;
    int i, j;
    uint8_t r, g, b;

    surface = SDL_LoadBMP(filename);
    if (!surface) {
        printf("bmp_to_gpl(): error loading BMP palette.\n");
        return 1;
    }

    if (surface->w != 16 || surface->h != 16) {
        printf("bmp_to_gpl(): not valid palette.\n");
        return 2;
    }

    for (i = 0; i < 16; i++) {
        for (j = 0; j < 16; j++) {
            uint32_t pixel = surface_get_pixel(surface, j, i);
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            printf("%d %d %d\t#%d\n", r, g, b, (i * 16 + j));
        }
    }

    SDL_FreeSurface(surface);

    return 0;
}