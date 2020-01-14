#include "vita.h"

#include <psp2/kernel/processmgr.h>
#include <psp2/apputil.h>
#include <psp2/appmgr.h>
#include <psp2/common_dialog.h>
#include <psp2/display.h>
#include <psp2/power.h>
#include <psp2/shellutil.h>
#include <psp2/rtc.h>
#include <vita2d.h>
#include <stdio.h>

enum
{
	COLOR_WHITE = RGBA8(255, 255, 255, 255),
	COLOR_BLACK = RGBA8(0, 0, 0, 255),
	COLOR_AQUA = RGBA8(168, 244, 253, 255),
	COLOR_BLUEGREY = RGBA8(92, 108, 124, 255),
	COLOR_RED = RGBA8(255, 0, 0, 255),
	COLOR_YELLOW = RGBA8(240, 255, 0, 255)
};

static vita2d_texture *g_bg;
static vita2d_texture *g_battery;
static vita2d_texture *g_charge;
float g_battery_x;
char g_title[128];

extern unsigned char _binary_assets_img_bg_png_start;
extern unsigned char _binary_assets_img_battery_png_start;
extern unsigned char _binary_assets_img_charge_png_start;

void vita_init()
{
	// Set CPU to 444mhz
	scePowerSetArmClockFrequency(444);
	// Init SceShellUtil events
	sceShellUtilInitEvents(0);
	// Prevent automatic CMA connection
	sceShellUtilLock(SCE_SHELL_UTIL_LOCK_TYPE_USB_CONNECTION);
	// Init SceAppUtil
	SceAppUtilInitParam init_param;
	SceAppUtilBootParam boot_param;
	memset(&init_param, 0, sizeof(SceAppUtilInitParam));
	memset(&boot_param, 0, sizeof(SceAppUtilBootParam));
	sceAppUtilInit(&init_param, &boot_param);
	// Init SceCommonDialog
	SceCommonDialogConfigParam config;
	sceCommonDialogConfigParamInit(&config);
	sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_LANG, (int *)&config.language);
	sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_ENTER_BUTTON, (int *)&config.enterButtonAssign);
	sceCommonDialogSetConfigParam(&config);
	// Get title
	memset(g_title, 0, sizeof(g_title));
	sceAppMgrAppParamGetString(sceKernelGetProcessId(), 9, g_title, sizeof(g_title));
	// Init Vita2dLib
	vita2d_init();
	font_init();
	// load image asset
	g_bg = vita2d_load_PNG_buffer(&_binary_assets_img_bg_png_start);
	g_battery = vita2d_load_PNG_buffer(&_binary_assets_img_battery_png_start);
	g_charge = vita2d_load_PNG_buffer(&_binary_assets_img_charge_png_start);
	g_battery_x = 949 - vita2d_texture_get_width(g_battery);
}

void vita_shutdown()
{
	vita2d_free_texture(g_bg);
	vita2d_free_texture(g_battery);
	vita2d_free_texture(g_charge);
	// Destory Vita2dLib
	font_destroy();
	vita2d_fini();
	// Shutdown AppUtil
	sceAppUtilShutdown();
	sceKernelExitProcess(0);
}

void vita_draw_status()
{
	// Background
	vita2d_draw_rectangle(0, 0, VITA_WIDTH, STATUSBAR_HEIGHT, COLOR_BLACK);
	font_draw(0, 0, COLOR_WHITE, g_title);
	// Battery
	if (sceKernelGetModel() == SCE_KERNEL_MODEL_VITA)
	{
		float percent = scePowerGetBatteryLifePercent();
		float width = percent * 0.29f;
		unsigned int battery_color = scePowerIsLowBattery() ?
			RGBA8(255, 48, 48, 255) : RGBA8(91, 223, 38, 255);
		vita2d_draw_rectangle((947.0f - width), 4, width, 16, battery_color);
	}
	vita2d_draw_texture(scePowerIsBatteryCharging() ? g_charge : g_battery, g_battery_x, 2);
	// DateTime
	SceDateTime time;
	char datetime_string[40];
	sceRtcGetCurrentClockLocalTime(&time);
	snprintf(datetime_string, sizeof(datetime_string), "%04d-%02d-%02d %02d:%02d:%02d",
		time.year, time.month, time.day, time.hour, time.minute, time.second);
	font_draw(700, 0, COLOR_WHITE, datetime_string);
}

void vita_start_draw()
{
	vita2d_start_drawing();
	// draw background
	vita2d_draw_texture(g_bg, 0, 0);
	vita_draw_status();
}

void vita_end_draw()
{
	vita2d_end_drawing();
	vita2d_common_dialog_update();
	vita2d_swap_buffers();
	sceDisplayWaitVblankStart();
}