#include <swilib2.h>
#include <conf_loader.h>
#include <cfg_items.h>

__CFG_BEGIN(0)

	__root const CFG_HDR cfghdr0 = {CFG_CHECKBOX, "Stop playing when you exit", 0, 2};
	__root const unsigned int cfg_stop_playing = 0;
	
	__root const CFG_HDR cfghdr5={CFG_UINT, "Rewind speed", 1, 10};
	__root const unsigned int cfg_rewind_speed = 5;
	
__CFG_END(0)
