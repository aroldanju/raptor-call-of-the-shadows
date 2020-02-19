#ifndef __dialog_h
#define __dialog_h

#include <resources.h>

typedef struct _dialog_button {
	char text[8];
} DialogButton;

typedef struct _dialog {
	char 		title[32];
	uint32_t 	width;
	Image		background;
} Dialog;

void dialog_create(Dialog *dialog, const char *title, uint32_t width, Resources* resources);

void dialog_render(Video* video, Dialog *dialog);

#endif	/* __dialog_h */