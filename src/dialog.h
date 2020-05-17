#ifndef __dialog_h
#define __dialog_h

#include <resources.h>
#include <label.h>

#define DIALOG_MAX_BUTTONS	2

typedef struct _dialog_button {
	char caption[8];
	Label labelCaption;
} DialogButton;

typedef struct _dialog {
	char 		title[32];
	uint32_t 	width, height;
	Image		background;
	Label 		labelTitle;

	uint8_t 		numButtons;
	DialogButton 	buttons[DIALOG_MAX_BUTTONS];
} Dialog;

void dialog_add_button(Dialog *dialog, const char *caption);

void dialog_create(Dialog *dialog, const char *title, uint32_t width, uint32_t height, Resources* resources);

void dialog_render(Video* video, Dialog *dialog);

void dialog_release(Dialog* dialog);

#endif	/* __dialog_h */