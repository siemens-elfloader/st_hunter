#include <cfg_items.h>
#include "inc/lng.h"
#include "inc/mc.h"


__CFG_BEGIN(0)

__root const CFG_HDR cfghdr0= { CFG_LEVEL, psz_sm_common, 1, 0 };

__root const CFG_HDR cfghdr1= { CFG_CHECKBOX, psz_savestate, 0, 2 };
    __root const int CONFIG_SAVE_PATH = 1;

__root const CFG_HDR cfghdr2= { CFG_CHECKBOX, psz_backexit, 0, 2 };
    __root const int CONFIG_BACK_EXIT = 1;

__root const CFG_HDR cfghdr3= { CFG_CHECKBOX, psz_free_png, 0, 2 };
    __root const int CONFIG_FREE_PNG_CACHE_ON_CLOSE = 0;

__root const CFG_HDR cfghdr4= { CFG_CHECKBOX, psz_show_ext, 0, 2 };
    __root const int CONFIG_SHOW_EXT = 1;

__root const CFG_HDR cfghdr5= { CFG_LEVEL, "", 0, 0 };


__root const CFG_HDR cfghdr6= { CFG_LEVEL, psz_gui, 1, 0 };

__root const CFG_HDR cfghdr7= { CFG_LEVEL, psz_font_setup, 1, 0 };

__root const CFG_HDR cfghdr8= { CFG_LEVEL, psz_font, 1, 0 };

__root const CFG_HDR cfghdr9= { CFG_UINT, psz_font_head, 0, 12 };
        __root const int FONT_HEAD = FONT_SMALL;

__root const CFG_HDR cfghdr10= { CFG_UINT, psz_font_soft, 0, 12 };
        __root const int FONT_SOFT = FONT_SMALL;

__root const CFG_HDR cfghdr11= { CFG_UINT, psz_font_list, 0, 12 };
        __root const int FONT_SPISOK = FONT_SMALL;

__root const CFG_HDR cfghdr12= { CFG_UINT, psz_font_menunv, 0, 12 };
        __root const int FONT_MENUNV = FONT_SMALL;

__root const CFG_HDR cfghdr13= { CFG_UINT, psz_font_menuv, 0, 12 };
        __root const int FONT_MENUV = FONT_SMALL;

__root const CFG_HDR cfghdr14= { CFG_UINT, psz_font_prop, 0, 12 };
        __root const int FONT_PROP = FONT_SMALL;

__root const CFG_HDR cfghdr15= { CFG_UINT, psz_font_msg, 0, 12 };
        __root const int FONT_MSG = FONT_SMALL;

__root const CFG_HDR cfghdr16= { CFG_LEVEL, "", 0, 0 };

__root const CFG_HDR cfghdr17= { CFG_UINT, psz_scspeed, 4, 200};
      __root const int CONFIG_SCROLL_TEXT_SPEED = 4;

__root const CFG_HDR cfghdr18= { CFG_UINT, psz_scwait, 0, 200 };
      __root const int CONFIG_SCROLL_TEXT_WAIT = 20;

__root const CFG_HDR cfghdr19= { CFG_UINT, psz_scrl_step, 0, 200 };
      __root const int CONFIG_SCROLL_STEP = 1;

__root const CFG_HDR cfghdr20= { CFG_LEVEL, "", 0, 0 };

__root const CFG_HDR cfghdr21= { CFG_LEVEL, psz_menu_opt, 1, 0 };

__root const CFG_HDR cfghdr22= { CFG_CHECKBOX, psz_savepoz, 0, 2 };
      __root const int cursave = 1;

__root const CFG_HDR cfghdr23= { CFG_LEVEL, "", 0, 0 };

__root const CFG_HDR cfghdr24= { CFG_LEVEL, psz_softhead, 1, 0 };

__root const CFG_HDR cfghdr25= { CFG_CHECKBOX, psz_soft_on, 0, 2 };
      __root const int CONFIG_SOFT_BUTTON = 1;

__root const CFG_HDR cfghdr26= { CFG_CHECKBOX, psz_time_on, 0, 2 };
      __root const int Clock = 1;

__root const CFG_HDR cfghdr27= { CFG_UINT, psz_soft_smes, 0, 240 };
      __root const int SMECHENIE = 1;

__root const CFG_HDR cfghdr28= { CFG_CHECKBOX, psz_put_in_head, 0, 2 };
      __root const int CONFIG_PUT_IN_HEAD = 0;

__root const CFG_HDR cfghdr29= { CFG_LEVEL, "", 0, 0 };

__root const CFG_HDR cfghdr30={CFG_STR_UTF8, psz_grafik_patch,0,127};
    __root const char graf_folder[128]="";

__root const CFG_HDR cfghdr31= { CFG_CHECKBOX, psz_pp_cur, 0, 2 };
    __root const int CONFIG_PP_CUR = 1;
/////////////////////////////////////////////////
__root const CFG_HDR cfghdr32= { CFG_CHECKBOX, psz_load_graf, 0, 2 };
    __root const int CONFIG_LOAD_GRAF = 1;
/////////////////////////////////////////////////
__root const CFG_HDR cfghdr33= { CFG_LEVEL, "", 0, 0 };


__root const CFG_HDR cfghdr34= { CFG_LEVEL, psz_sm_disks, 1, 0 };

__root const CFG_HDR cfghdr35= { CFG_CHECKBOX, psz_showhidden, 0, 2 };
    __root const int CONFIG_SHOW_HIDDEN = 0;

__root const CFG_HDR cfghdr36= { CFG_CHECKBOX, psz_showsystem, 0, 2 };
    __root const int CONFIG_SHOW_SYSTEM = 0;

__root const CFG_HDR cfghdr37= { CFG_CHECKBOX, psz_showhiddrv, 0, 2 };
    __root const int CONFIG_SHOW_SYSDRV = 0;

__root const CFG_HDR cfghdr38= { CFG_CHECKBOX, psz_showmmc, 0, 2 };
    __root const int CONFIG_SHOW_MMC = 1;

__root const CFG_HDR cfghdr39= { CFG_LEVEL, "", 0, 0 };


__root const CFG_HDR cfghdr40= { CFG_LEVEL, psz_sm_configs, 1, 0 };

__root const CFG_HDR cfghdr41= { CFG_CHECKBOX, psz_loadicons, 0, 2 };
    __root const int CONFIG_LOAD_ICONS = 1;

__root const CFG_HDR cfghdr42= { CFG_CHECKBOX, psz_loadkeys, 0, 2 };
    __root const int CONFIG_LOAD_KEYS = 1;

__root const CFG_HDR cfghdr43= { CFG_CHECKBOX, psz_loadmui, 0, 2 };
    __root const int CONFIG_LOAD_MUI = 0;

__root const CFG_HDR cfghdr44= { CFG_CHECKBOX, psz_loadcs, 0, 2 };
    __root const int CONFIG_LOAD_CS = 1;

__root const CFG_HDR cfghdr45= { CFG_STR_UTF8, psz_configpath, 0, 63 };
    __root const char CONFIG_MCCONFIG_PATH[64] = "";

__root const CFG_HDR cfghdr46= { CFG_LEVEL, "", 0, 0 };


__root const CFG_HDR cfghdr47= { CFG_LEVEL, psz_sm_confirms, 1, 0 };

__root const CFG_HDR cfghdr48= { CFG_CHECKBOX, psz_confirmexit, 0, 2 };
    __root const int CONFIG_CONFIRM_EXIT = 0;

__root const CFG_HDR cfghdr49= { CFG_CHECKBOX, psz_confirmdel, 0, 2 };
    __root const int CONFIG_CONFIRM_DELETE = 1;

__root const CFG_HDR cfghdr50= { CFG_CHECKBOX, psz_confirmdelr, 0, 2 };
    __root const int CONFIG_CONFIRM_DELETERO = 1;

__root const CFG_HDR cfghdr51= { CFG_CHECKBOX, psz_confirmcopy, 0, 2 };
    __root const int CONFIG_CONFIRM_COPY = 1;

__root const CFG_HDR cfghdr52= { CFG_CHECKBOX, psz_confirmmove, 0, 2 };
    __root const int CONFIG_CONFIRM_MOVE = 1;

__root const CFG_HDR cfghdr53= { CFG_CHECKBOX, psz_confirmrepl, 0, 2 };
    __root const int CONFIG_CONFIRM_REPLACE = 1;

__root const CFG_HDR cfghdr54= { CFG_LEVEL, "", 0, 0 };


__root const CFG_HDR cfghdr55= { CFG_LEVEL, psz_sm_autoexit, 1, 0 };

__root const CFG_HDR cfghdr56= { CFG_UINT, psz_autoexit_after, 0, 2000 };
    __root const int CONFIG_AUTO_EXIT_AFTER_MIN = 10;

__root const CFG_HDR cfghdr57= { CFG_CHECKBOX, psz_autoexit_on_background, 0, 2 };
    __root const int CONFIG_ENABLE_BACKGROUND_EXIT = 1;

__root const CFG_HDR cfghdr58= { CFG_LEVEL, "", 0, 0 };


__root const CFG_HDR cfghdr59= { CFG_LEVEL, psz_arc, 1, 0 };

__root const CFG_HDR cfghdr60= { CFG_LEVEL, psz_7z, 1, 0 };

__root const CFG_HDR cfghdr61= { CFG_CHECKBOX, psz_enable_7z, 0, 2 };
      __root const int CONFIG_7Z_ENABLE = 1;

__root const CFG_HDR cfghdr62= { CFG_CBOX, psz_detect_by, 0, 2 };
      __root const int CONFIG_7Z_DETECT_BY = 0;
__root const CFG_CBOX_ITEM cfgcbox63[] = { psz_content, psz_extension };

__root const CFG_HDR cfghdr64= { CFG_LEVEL, "", 0, 0 };

__root const CFG_HDR cfghdr65= { CFG_LEVEL, psz_sm_zip, 1, 0 };

__root const CFG_HDR cfghdr66= { CFG_CHECKBOX, psz_zip_enable, 0, 2 };
    __root const int CONFIG_ZIP_ENABLE = 1;

__root const CFG_HDR cfghdr67= { CFG_CBOX, psz_detect_by, 0, 2 };
      __root const int CONFIG_ZIP_DETECT_BY = 0;
__root const CFG_CBOX_ITEM cfgcbox68[] = { psz_content, psz_extension };

__root const CFG_HDR cfghdr69= { CFG_CHECKBOX, psz_jar_zip, 0, 2 };
    __root const int CONFIG_JAR_ENABLE = 0;

__root const CFG_HDR cfghdr70= { CFG_CHECKBOX, psz_sdt_zip, 0, 2 };
    __root const int CONFIG_SDT_ENABLE = 0;

__root const CFG_HDR cfghdr71= { CFG_LEVEL, "", 0, 0 };

__root const CFG_HDR cfghdr72= { CFG_STR_UTF8, psz_temppath, 0, 63 };
    __root const char CONFIG_TEMP_PATH[64] = DEFAULT_DISK":\\Temp";

__root const CFG_HDR cfghdr73= { CFG_CHECKBOX, psz_cleartemp, 0, 2 };
    __root const int CONFIG_DELETE_TEMP_FILES_ON_EXIT = 1;

__root const CFG_HDR cfghdr74= { CFG_LEVEL, "", 0, 0 };

__root const CFG_HDR cfghdr75= { CFG_LEVEL, psz_opt_open, 1, 0 };

__root const CFG_HDR cfghdr76= { CFG_CHECKBOX, psz_music_on_elfpleer, 0, 2 };
    __root const int CONFIG_MEDIA_ENABLE = 0;

__root const CFG_HDR cfghdr77= { CFG_STR_UTF8, psz_elfview, 0, 127};
    __root const char CONFIG_VIEW_PUT[128] = "";

__root const CFG_HDR cfghdr78= { CFG_LEVEL, "", 0, 0 };


__CFG_END(0)
