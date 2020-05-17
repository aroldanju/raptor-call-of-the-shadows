#include "dialog.h"
#include "definitions.h"

void dialog_create(Dialog *dialog, const char *title, uint32_t width, uint32_t height, Resources* resources)	{
    int jr = 0, jo = 0, ir = 0, io = 0;
    int i, j;
	strcpy(dialog->title, title);
	dialog->width = width;
	dialog->height = height;

	Image *background = resources->images[IMAGE_DIALOG_BACKGROUND];

	ir = height / background->surface->h;
	io = height % background->surface->h;
	jr = width / background->surface->w;
	jo = width % background->surface->w;

    /*150/64=2.34375
     150%64=22*/

	dialog->numButtons = 0;

	image_create_blank_sized(&dialog->background, width, height);

	for(i=0; i<ir; i++) {
	    for(j=0;j<jr;j++) {
            image_extract_destination(&dialog->background,
                    background,
                    0, 0, background->surface->w, background->surface->h,
                    j * background->surface->w,
                    i * background->surface->h,
                    background->surface->w, background->surface->h);
        }
	}

    label_create(&dialog->labelTitle, G_RESOURCES.fonts[FONT_BIG]);
	label_setCaption(&dialog->labelTitle, dialog->title);
	label_setColor(&dialog->labelTitle, 212, 116, 24);
}

void dialog_render(Video* video, Dialog *dialog)	{
    const uint32_t x = VIDEO_SCREEN_WIDTH / 2 - dialog->width / 2;
    const uint32_t y = VIDEO_SCREEN_HEIGHT / 2 - dialog->height / 2;

    image_render(&dialog->background, x, y, video);

    // Out border
    video_render_line_alpha(video, x, y, x + dialog->width - 1, y, 255, 255, 255, 64);
    video_render_line_alpha(video, x + dialog->width - 1, y + 1, x + dialog->width - 1, y + dialog->height - 1, 255, 255, 255, 64);
    video_render_line_alpha(video, x, y + 1, x, y + dialog->height - 1, 0, 0, 0, 64);
    video_render_line_alpha(video, x + 1, y + dialog->height - 1, x + dialog->width - 1, y + dialog->height - 1, 0, 0, 0, 64);

    // Middle border
    video_render_rect_alpha(video, x + 3, y + 3, dialog->width - 6, 5, 255, 255, 255, 32);
    video_render_rect_alpha(video, x + 3, y + dialog->height - 3 - 5, dialog->width - 6, 5, 255, 255, 255, 32);
    video_render_rect_alpha(video, x + 3, y + 3 + 5, 5, dialog->height - 3 - 3 - 5 - 5, 255, 255, 255, 32);
    video_render_rect_alpha(video, x + dialog->width - 3 - 5, y + 3 + 5, 5, dialog->height - 3 - 3 - 5 - 5, 255, 255, 255, 32);

    // Content
    video_render_rect_alpha(video, x + 3 + 5, y + 3 + 5, dialog->width - 3 - 3 - 5 - 5, dialog->height - 3 - 3 - 5 - 5, 0, 0, 0, 128);

    label_render(&dialog->labelTitle, video, x + dialog->width / 2 - label_get_width(&dialog->labelTitle) / 2, y + 3 + 5 + 3);
}

void dialog_release(Dialog* dialog) {
    image_destroy(&dialog->background);
}

void dialog_add_button(Dialog *dialog, const char *caption) {

}
