
#include <vita2d.h>

int is_korean_char(const unsigned int c)
{
	unsigned short ch = c;
	// hangul compatibility jamo block
	if (0x3130 <= ch && ch <= 0x318f)
		return 1;
	// hangul syllables block
	if (0xac00 <= ch && ch <= 0xd7af)
		return 1;
	// korean won sign
	if (ch == 0xffe6)
		return 1;
	return 0;
}

int is_chinese_char(const unsigned int c)
{
	unsigned short ch = c;
	// Hangul compatibility jamo block
	if (0x3400 <= ch && ch <= 0x4DB5)
		return 1;
	if (0x4E00 <= ch && ch <= 0x9FA5)
		return 1;
	return 0;
}

int is_latin_char(const unsigned int c)
{
	unsigned short ch = c;
	// basic latin block + latin-1 supplement block
	if (ch <= 0x00ff)
		return 1;
	// cyrillic block
	if (0x0400 <= ch && ch <= 0x04ff)
		return 1;
	return 0;
}

static vita2d_pvf *g_font;

void font_init()
{
    vita2d_system_pvf_config pvf_config[] = {
		{SCE_PVF_LANGUAGE_K, is_korean_char},
		{SCE_PVF_LANGUAGE_C, is_chinese_char},
		{SCE_PVF_LANGUAGE_LATIN, is_latin_char},
		{SCE_PVF_DEFAULT_LANGUAGE_CODE, NULL}};
	g_font = vita2d_load_system_pvf(4, pvf_config);
}

void font_destroy()
{
    vita2d_free_pvf(g_font);
}

int font_draw(int x, int y, unsigned int color, const char *text)
{
	if (!g_font) return -1;
	return vita2d_pvf_draw_text(g_font, x, y + 20, color, 1.f, text);
}

int font_width(const char *text)
{
	if (!g_font) return -1;
	return vita2d_pvf_text_width(g_font, 1.f, text);
}