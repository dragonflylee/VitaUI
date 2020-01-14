#include "input.h"
#include "debug.h"

#include <psp2/apputil.h>
#include <psp2/system_param.h>
#include <psp2/kernel/processmgr.h>
#include <string.h>

#define ANALOG_CENTER 128
#define ANALOG_THRESHOLD 64

unsigned int SCE_CTRL_ENTER;
unsigned int SCE_CTRL_CANCEL;

static SceUInt64 g_time;
static uint32_t g_frame;

void input_init(vita_input *input)
{
    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, SCE_TOUCH_SAMPLING_STATE_START);
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

    int enter_button = SCE_SYSTEM_PARAM_ENTER_BUTTON_CROSS;
    sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_ENTER_BUTTON, &enter_button);
    // Japanese region
    if (enter_button == SCE_SYSTEM_PARAM_ENTER_BUTTON_CIRCLE)
    {
        SCE_CTRL_ENTER = SCE_CTRL_CIRCLE;
        SCE_CTRL_CANCEL = SCE_CTRL_CROSS;
    }
    else
    {
        SCE_CTRL_ENTER = SCE_CTRL_CROSS;
        SCE_CTRL_CANCEL = SCE_CTRL_CIRCLE;
    }
    g_time = sceKernelGetProcessTimeWide();
    memset(input, 0, sizeof(vita_input));
}

int input_pick(vita_input *input)
{
    int ret;
    if ((ret = sceCtrlPeekBufferPositive(0, &input->pad, 1)) < 0)
    {
        LOG("sceCtrlPeekBufferPositive 0x%.8X", ret);
        return ret;
    }
    if ((ret = sceTouchPeek(SCE_TOUCH_PORT_FRONT, &input->touch, 1)) < 0)
    {
        LOG("sceTouchPeek 0x%.8X", ret);
        return ret;
    }

    uint32_t previous = input->down;
    input->down = input->pad.buttons;
    input->pressed = input->down & ~previous;
    // handle Analog Sticks
    if (!(input->down & SCE_CTRL_INTERCEPTED))
    {
        if (input->pad.lx < ANALOG_CENTER - ANALOG_THRESHOLD)
            input->down |= SCE_CTRL_LEFT;
        if (input->pad.lx > ANALOG_CENTER + ANALOG_THRESHOLD)
            input->down |= SCE_CTRL_RIGHT;
        if (input->pad.ly < ANALOG_CENTER - ANALOG_THRESHOLD)
            input->down |= SCE_CTRL_UP;
        if (input->pad.ly > ANALOG_CENTER + ANALOG_THRESHOLD)
            input->down |= SCE_CTRL_DOWN;
    }
    // handle Repeat Press
    input->active = input->pressed;
    if (input->down == previous)
    {
        if (g_frame >= 10)
        {
            input->active = input->down;
        }
        g_frame++;
    }
    else
    {
        g_frame = 0;
    }
    uint64_t time = sceKernelGetProcessTimeWide();
    input->delta = time - g_time;
    g_time = time;
    return 1;
}