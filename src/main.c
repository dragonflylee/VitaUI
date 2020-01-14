#include "vita.h"
#include "debug.h"
#include "input.h"

int main(int argc, char *argv[])
{
	log_init("ux0:/data/log");
	vita_init();
	vita_input input;
	input_init(&input);
	while (input_pick(&input) > 0)
	{
		vita_start_draw();

		if (input.down & SCE_CTRL_CROSS)
			LOG("press \xe2\x95\xb3");
		if (input.down & SCE_CTRL_CIRCLE)
			LOG("press \xe2\x97\x8b");
		if (input.down & SCE_CTRL_TRIANGLE)
			LOG("press \xe2\x96\xb3");
		if (input.down & SCE_CTRL_SQUARE)
			LOG("press \xe2\x96\xa1");
		

		vita_end_draw();
	}
	log_flush();
	vita_shutdown();
	return 0;
}