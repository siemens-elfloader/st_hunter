#include "..\inc\cfg_items.h"
#include "..\inc\swilib.h"
#include "lang.h"

__root CFG_HDR cfghdr1={CFG_COORDINATES, cfgPOS, 0, 0};
__root unsigned int pos_x=131;
__root unsigned int pos_y=0x02;

__root CFG_HDR cfghdr2={CFG_UINT, cfgFONT, 0, 16};
__root unsigned int font=FONT_SMALL;

__root CFG_HDR cfghdr3={CFG_UINT, cfgATTR, 0, 512};
__root unsigned int txt_attr=TEXT_ALIGNMIDDLE+TEXT_OUTLINE;

__root CFG_HDR cfghdr4={CFG_COLOR, cfgWD_COLOR, 0, 0};
__root char word_color[4]={0xFF, 0xFF, 0xFF, 100};

__root CFG_HDR cfghdr5={CFG_COLOR, cfgWS_FRM_COLOR, 0, 0};
__root char word_frm_color[4]={0, 0, 0, 100};

__root CFG_HDR cfghdr6={CFG_COLOR, cfgRND_FRM_COLOR, 0, 0};
__root char rnd_frm_color[4]={0xFF, 0xFF, 0xFF, 100};

__root CFG_HDR cfghdr7={CFG_COLOR, cfgFRM_COLOR, 0, 0};
__root char frm_color[4]={0, 0, 0, 100};


