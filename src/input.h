#ifndef _VITA_INPUT_H_
#define _VITA_INPUT_H_

#include <psp2/ctrl.h>
#include <psp2/touch.h>

typedef struct
{
    SceCtrlData pad;
    SceTouchData touch;

    uint64_t delta;   // microseconds from previous frame
    uint32_t pressed; // button pressed in last frame
    uint32_t down;    // button is currently down
    uint32_t active; // button is pressed in last frame, or held down for a long
                     // time (10 frames)
} vita_input;

extern unsigned int SCE_CTRL_ENTER;
extern unsigned int SCE_CTRL_CANCEL;

void input_init(vita_input *input);
int input_pick(vita_input *input);

#endif // _VITA_INPUT_H_
