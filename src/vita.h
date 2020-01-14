#ifndef _VITA_H_
#define _VITA_H_

#define VITA_WIDTH  960
#define VITA_HEIGHT 544
#define STATUSBAR_HEIGHT 30

#ifdef __cplusplus
extern "C" {
#endif

void vita_init();
void vita_shutdown();
void vita_start_draw();
void vita_end_draw();
// font 
void font_init();
void font_destroy();
int font_draw(int x, int y, unsigned int color, const char* text);
int font_width(const char* text);

#ifdef __cplusplus
}
#endif

#endif // _VITA_H_