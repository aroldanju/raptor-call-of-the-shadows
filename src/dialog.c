#include "dialog.h"
#include "definitions.h"

void dialog_create(Dialog *dialog, const char *title, uint32_t width, Resources* resources)	{
	strcpy(dialog->title, title);
	dialog->width = width;

	Image *background = resources->images[IMAGE_DIALOG_BACKGROUND];

	SDL_Rect rect = (SDL_Rect){ 0, 0, width % background->surface->w, 64 };

	/*
	background.


	const float w = (float)width / (float)background->surface->w;
	printf("W: %d\n", width % background->surface->w);
	*/

}

void dialog_render(Video* video, Dialog *dialog)	{

}
