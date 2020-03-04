#include "..\inc\swilib.h"
#include "..\inc\cfg_items.h"

__root const CFG_HDR cfghdr_01_begin={CFG_LEVEL,"Functions",1,0};


__root const CFG_HDR cfghdr_01_00_begin={CFG_LEVEL,"Create CSM",1,0};


__root const CFG_HDR cfghdr_01_00__1={CFG_CBOX,"Action",0,2};
__root const unsigned int bcfg_csm_act = 1;
__root const CFG_CBOX_ITEM cfghdr_01_00__1c[2]={"Kill NSG CSM","Prepair NSG CSM"};

__root const CFG_HDR cfghdr_01_00__2={CFG_CHECKBOX,"Mark NSG CSM Name",0,0};
__root const int bcfg_markcsm_ena = 1;

__root const CFG_HDR cfghdr_01_00_end={CFG_LEVEL,"",0,0};




__root const CFG_HDR cfghdr_01_01_begin={CFG_LEVEL,"Create GUI",1,0};


__root const CFG_HDR cfghdr_01_01__1={CFG_CHECKBOX,"SWI 0x136",0,0};
__root const int bcfg_0x136_ena = 1;

__root const CFG_HDR cfghdr_01_01__2={CFG_CHECKBOX,"SWI 0x137",0,0};
__root const int bcfg_0x137_ena = 1;

__root const CFG_HDR cfghdr_01_01__3={CFG_CHECKBOX,"SWI 0x138",0,0};
__root const int bcfg_0x138_ena = 1;

__root const CFG_HDR cfghdr_01_01__4={CFG_CHECKBOX,"SWI 0x139",0,0};
__root const int bcfg_0x139_ena = 1;

__root const CFG_HDR cfghdr_01_01__5={CFG_CHECKBOX,"SWI 0x13A",0,0};
__root const int bcfg_0x13A_ena = 1;


__root const CFG_HDR cfghdr_01_01_end={CFG_LEVEL,"",0,0};




__root const CFG_HDR cfghdr_01_02_begin={CFG_LEVEL,"Create UI",1,0};


__root const CFG_HDR cfghdr_01_02__1={CFG_CHECKBOX,"SWI 0x152",0,0};
__root const int bcfg_0x152_ena = 1;

__root const CFG_HDR cfghdr_01_02__2={CFG_CHECKBOX,"SWI 0x167",0,0};
__root const int bcfg_0x167_ena = 1;

__root const CFG_HDR cfghdr_01_02__3={CFG_CHECKBOX,"SWI 0x217",0,0};
__root const int bcfg_0x217_ena = 1;


__root const CFG_HDR cfghdr_01_02_end={CFG_LEVEL,"",0,0};




__root const CFG_HDR cfghdr_01_03_begin={CFG_LEVEL,"PlayMusic Func",1,0};


__root const CFG_HDR cfghdr_01_03__1={CFG_CHECKBOX,"SWI 0x04A",0,0};
__root const int bcfg_0x04A_ena = 1;

__root const CFG_HDR cfghdr_01_03__2={CFG_CHECKBOX,"SWI 0x099",0,0};
__root const int bcfg_0x099_ena = 1;


__root const CFG_HDR cfghdr_01_03_end={CFG_LEVEL,"",0,0};




__root const CFG_HDR cfghdr_01_04_begin={CFG_LEVEL,"FSystem Func",1,0};

__root const CFG_HDR cfghdr_01_04__1={CFG_CBOX,"FS Type when start elf",0,2};
__root const unsigned int bcfg_fss_fstype = 0;
__root const CFG_CBOX_ITEM cfghdr_01_04__01c[2]={"SGold","Smart"};

__root const CFG_HDR cfghdr_01_04__2={CFG_CBOX,"Mode",0,3};
__root const unsigned int bcfg_fss_type = 0;
__root const CFG_CBOX_ITEM cfghdr_01_04__2c[3]={"Not KeyHook","Short","Long"};

__root const CFG_HDR cfghdr_01_04__3={CFG_KEYCODE,"Key",0,99};
__root const int bcfg_fss_key=GREEN_BUTTON;

__root const CFG_HDR cfghdr_01_04__4={CFG_CHECKBOX,"Message Info",0,0};
__root const int bcfg_fss_msg = 1;


__root const CFG_HDR cfghdr_01_04_end={CFG_LEVEL,"",0,0};




__root const CFG_HDR cfghdr_01_05_begin={CFG_LEVEL,"Misc Func",1,0};


__root const CFG_HDR cfghdr_01_05__1 = {CFG_CBOX, "isnewSGold()", 0, 3};
__root const int bcfg_isnsg = 0;
__root const CFG_CBOX_ITEM cfghdr_01_05__1c[3] = {"SGold", "NewSGold","ELKA"};


__root const CFG_HDR cfghdr_01_05_end={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_01_end={CFG_LEVEL,"",0,0};




__root const CFG_HDR cfghdr_02_begin={CFG_LEVEL,"Key Answer",1,0};


__root const CFG_HDR cfghdr_02__01={CFG_CBOX,"Key Answer when start elf",0,2};
__root const unsigned int bcfg_ka_type = 0;
__root const CFG_CBOX_ITEM cfghdr_02__01c[2]={"SGold","Smart"};

__root const CFG_HDR cfghdr_02__02={CFG_CBOX,"Type key press",0,3};
__root const unsigned int bcfg_type = 0;
__root const CFG_CBOX_ITEM cfghdr_02__02c[3]={"Not keyhook","Short","Long"};

__root const CFG_HDR cfghdr_02__03={CFG_KEYCODE,"Key",0,99};
__root const int bcfg_key=GREEN_BUTTON;

__root const CFG_HDR cfghdr_02__04={CFG_CHECKBOX,"Message Info",0,0};
__root const int bcfg_msg = 1;



__root const CFG_HDR cfghdr_02_01_begin={CFG_LEVEL,"Set SMART Mode for UI",1,0};


__root const CFG_HDR cfghdr_02_01__1={CFG_CHECKBOX,"Left Soft",0,0};
__root const int bcfg_ls_ena = 0;

__root const CFG_HDR cfghdr_02_01__2={CFG_CHECKBOX,"Right Soft",0,0};
__root const int bcfg_rs_ena = 0;

__root const CFG_HDR cfghdr_02_01__3={CFG_CHECKBOX,"Up Button",0,0};
__root const int bcfg_ub_ena = 1;

__root const CFG_HDR cfghdr_02_01__4={CFG_CHECKBOX,"Down Button",0,0};
__root const int bcfg_db_ena = 1;

__root const CFG_HDR cfghdr_02_01__5={CFG_CHECKBOX,"Left Button",0,0};
__root const int bcfg_lb_ena = 1;

__root const CFG_HDR cfghdr_02_01__6={CFG_CHECKBOX,"Right Button",0,0};
__root const int bcfg_rb_ena = 1;


__root const CFG_HDR cfghdr_02_01_end={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_02_end={CFG_LEVEL,"",0,0};






