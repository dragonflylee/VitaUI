
#include <vita2d.h>

const float FONT_SIZE = 20.f;

static vita2d_font *g_font;

void font_init()
{
	g_font = vita2d_load_font_file("sa0:/data/font/pvf/cn0.pvf");
}

void font_destroy()
{
    vita2d_free_font(g_font);
}

int font_draw(int x, int y, unsigned int color, const char *text)
{
	if (!g_font) return -1;
	return vita2d_font_draw_text(g_font, x, y + 20, color, FONT_SIZE, text);
}

int font_width(const char *text)
{
	if (!g_font) return -1;
	return vita2d_font_text_width(g_font, FONT_SIZE, text);
}