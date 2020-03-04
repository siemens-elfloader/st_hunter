#ifndef _STRCONST_H_
#define _STRCONST_H_

#define CP_RIGHT "My SMS System C++\nv3.1 Beta\n(c)BingK\n(r)titron,Rst7\nmod by JustCrazy"

extern const char IPC_NAME[];
extern const char FLDR_IN[];
//extern const char FLDR_OUT[];
extern const char FLDR_SENT[];
extern const char FLDR_DRAFT[];
extern const char FLDR_UNK[];
extern const char FLDR_ARCHIVE[];

extern const char PERCENT_T[];
extern const char PERCENT_S[];
extern const char num_fetion[];
extern const char STR_LINES[];
extern const char STR_UNK_TXTT[];
extern const char STR_UNK_TYPE[];

extern const char STR_UNK_NUM_UTF8[];
extern const char STR_NUMBER_UTF8[];
extern const char STR_TEXT_UTF8[];
extern const char STR_TO_UTF8[];
extern const char STR_FROM_UTF8[];
extern const char STR_TIME_UTF8[];
extern const char STR_UNK_UTF8[];
extern const char DEFAUT_HLP_STR[];

#ifdef LANG_CN
#define LGP_CFG_MAIN_FOLDER "MySMSYSÎÄ¼þ¼Ð"
#define LGP_CFG_SYSTEM_FOLDER "ÏµÍ³ÎÄ¼þ¼Ð"
#define LGP_CFG_ENA_NOTIFY "µ¯³öÐÂÐÅÏ¢´°¿Ú"
#define LGP_CFG_VIBRA_POWER "Õñ¶¯Ç¿¶È"
#define LGP_CFG_NOTIFY_SET "ÐÂÐÅÏ¢ÌáÐÑÉèÖÃ"
#define LGP_CFG_NOTIFY_TIME "ÌáÐÑÊ±¼ä³¤¶È(Ãë)"
#define LGP_CFG_ENA_AUTO_SAF "²é¿´Ê±×ª»»ÎªÎÄ¼þ"
#define LGP_CFG_ENA_AUTO_SAF_RT "¼´Ê±×ª»»ÎªÎÄ¼þ"
#define LGP_CFG_ENA_NEWSMS_ICON "´ý»úÏÔÊ¾ÐÂ¶ÌÐÅÍ¼±ê"
#define LGP_CFG_ICONNEW_POS "Í¼±ê×ø±ê"
#define LGP_CFG_EXPORT_CHARSET "µ¼³öÎÄ±¾×Ö·û±àÂë"
#define LGP_CFG_ENA_SAVE_SENT "±£´æÒÑ·¢ËÍ¶ÌÐÅ"
#define LGP_CFG_ENA_SOUND "Ê¹ÓÃÉùÒôÌáÐÑ"
#define LGP_CFG_SOUND_VOL "ÒôÁ¿"
#define LGP_CFG_SOUND_PATH "ÉùÒôÎÄ¼þÂ·¾¶"
#define LGP_CFG_ENA_AUTO_DEL_RP "×Ô¶¯É¾³ýÐÅÏ¢±¨¸æ"
#define LGP_CFG_ENA_EXIT_SAVE_DRAFT "±£´æ²Ý¸åºóÍË³ö"
#define LGP_CFG_TEXT_FONT "ÎÄ±¾×ÖÌå"
#define LGP_EDIT_FONT_DEFAULT "Ê¹ÓÃÄ¬ÈÏ" 
#define LGP_EDIT_FONT_SMALL "Ð¡ºÅ×ÖÌå"
#define LGP_EDIT_FONT_SMALL_BOLD "Ð¡×ÖÌå¼Ó´Ö"
#define LGP_EDIT_FONT_MEDIUM "ÖÐºÅ×ÖÌå"
#define LGP_EDIT_FONT_MEDIUM_BOLD "ÖÐ×ÖÌå¼Ó´Ö"
#define LGP_EDIT_FONT_LARGE "´óºÅ×ÖÌå"
#define LGP_EDIT_FONT_LARGE_BOLD "´ó×ÖÌå¼Ó´Ö"
#ifdef ELKA
#define LGP_CFG_ENA_SLI "Ê¹ÓÃSLI"
#endif
#define LGP_CFG_KEYHOOK_SETTING "°´¼ü×éºÏÆô¶¯ÉèÖÃ"
#define LGP_CFG_1ST_KEY "°´¼ü1"
#define LGP_CFG_2ND_KEY "°´¼ü2"
#define LGP_CFG_ICONBAR_SETTING "ÐÂÐÅÏ¢Í¼±êÀ¸ÉèÖÃ"
#define LGP_CFG_ENA_IB "Ê¹ÓÃÍ¼±êÀ¸"
#define LGP_CFG_ICON_IB "ÐÂ¶ÌÐÅÍ¼±êID"
#define LGP_CFG_ENA_AUTO_SAVE_DRAFT "×Ô¶¯±£´æ²Ý¸å"
#define LGP_CFG_PROFILE1_SET "Çé¾°Ä£Ê½1"
#define LGP_CFG_PROFILE2_SET "Çé¾°Ä£Ê½2"
#define LGP_CFG_PROFILE3_SET "Çé¾°Ä£Ê½3"
#define LGP_CFG_PROFILE4_SET "Çé¾°Ä£Ê½4"
#define LGP_CFG_PROFILE5_SET "Çé¾°Ä£Ê½5"
#define LGP_CFG_PROFILE6_SET "Çé¾°Ä£Ê½6"
#define LGP_CFG_PROFILE7_SET "Çé¾°Ä£Ê½7"
#define LGP_CFG_PROFILE8_SET "Çé¾°Ä£Ê½8"

#ifdef MEGADIAL
#define LGP_CFG_MDSETTING "MegaDialÉèÖÃ"
#define LGP_CFG_MDBGCOL "ÁÐ±í±³¾°É«"
#define LGP_CFG_MDMNBDCOL "ÁÐ±íÍâ¿òÉ«"
#define LGP_CFG_MDNSLCOL "Î´Ñ¡ÖÐÌõÄ¿ÑÕÉ«"
#define LGP_CFG_MDSLCOL "Ñ¡ÖÐÌõÄ¿ÑÕÉ«"
#define LGP_CFG_MDSLBGCOL "Ñ¡ÖÐÌõÄ¿±³¾°É«"
#define LGP_CFG_MDSLBDCOL "Ñ¡ÖÐÌõÄ¿Íâ¿òÉ«"
#define LGP_CFG_MDSHMKCOL "ËÑË÷¹Ø¼ü×ÖÑÕÉ«"
#define LGP_CFG_ENA_MD "Ê¹ÓÃMegaDial"
#define LGP_CFG_MDCSCOL "ÇøºÅÐãÑÕÉ«"
#endif

#define LGP_CFG_ENA_SNED_ON_BG "ºóÌ¨·¢ËÍ¶ÌÐÅ"
#define LGP_CFG_ICONSNED_IB "¶ÌÐÅ·¢ËÍÍ¼±êID"
#define LGP_CFG_DEFAULT_SENT_NUM "Ä¬ÈÏ·¢ËÍºÅÂë"

#define LGP_CFG_FILTERSET "É¸Ñ¡ÏÔÊ¾ÉèÖÃ"
#define LGP_CFG_FILTEROP "É¸Ñ¡ÏÔÊ¾Ñ¡Ïî"
#define LGP_FILTER_ALL "È«²¿"
#define LGP_FILTER_NEW "Î´¶Á¶ÌÐÅ"
#define LGP_FILTER_DAT "À´×ÔSMS.dat"
#define LGP_FILTER_FILE "À´×ÔÎÄ¼þ"
#define LGP_FILTER_NUM "ºÅÂë"
#define LGP_FILTER_STR "×Ö·û´®"
#define LGP_CFG_STRORNUM "×Ö·û´®»òºÅÂë"
#define LGP_CFG_DEFAULT_TAB "Ä¬ÈÏ±êÇ©"
#define LGP_CFG_MAINMENU "Ö÷²Ëµ¥"
#define LGP_CFG_INBOX "ÊÕ¼þÏä"
#define LGP_CFG_SENT "ÒÑ·¢ËÍ"
#define LGP_CFG_FILTER "É¸Ñ¡ÏÔÊ¾"
#define LGP_CFG_DRAFT "²Ý¸å"
#define LGP_CFG_ENA_VIBRA "Ê¹ÓÃÕñ¶¯"
#define LGP_CFG_VIBRA_RHYTHM "Õñ¶¯½Ú×à"
#endif

#ifdef LANG_EN
#define LGP_CFG_MAIN_FOLDER "MySMSYS Folder"
#define LGP_CFG_SYSTEM_FOLDER "System Folder"
#define LGP_CFG_ENA_NOTIFY "Popup New SMS"
#define LGP_CFG_ENA_PRE "Popup preview"
#define LGP_CFG_ENA_SCREEN "Draw On ScreenSaver"
/*#ifdef ELKA
#define LGP_CFG_ENA_SLI "Enable SLI"
#endif*/
#define LGP_CFG_VIBRA_POWER "Vibra Power"
#define LGP_CFG_NOTIFY_SET "Notify Setup"
#define LGP_CFG_NOTIFY_TIME "Notify Time (s)"
#define LGP_CFG_ENA_AUTO_SAF "Auto Save As File At View"
#define LGP_CFG_ENA_AUTO_SAF_RT "Auto Save As File Real Time"
#define LGP_CFG_ENA_NEWSMS_ICON "Show NewSMS Icon Idle"
#define LGP_CFG_ICONNEW_POS "Icon Position"
#define LGP_CFG_EXPORT_CHARSET "Export Text Charset"
#define LGP_CFG_ENA_SAVE_SENT "Save Sent SMS"
#define LGP_CFG_ENA_SOUND "Enable Sound Notify"
#define LGP_CFG_SOUND_VOL "Volume"
#define LGP_CFG_SOUND_PATH "Sound File Path"
#define LGP_CFG_ENA_AUTO_DEL_RP "Auto Delete Message Report"
#define LGP_CFG_ENA_EXIT_SAVE_DRAFT "Exit After Save Draft"
#define LGP_CFG_TEXT_FONT "Text Font"
#define LGP_EDIT_FONT_DEFAULT "Default" 
#define LGP_EDIT_FONT_SMALL "Small"
#define LGP_EDIT_FONT_SMALL_BOLD "Small+Bold"
#define LGP_EDIT_FONT_MEDIUM "Medium"
#define LGP_EDIT_FONT_MEDIUM_BOLD "Medium+Bold"
#define LGP_EDIT_FONT_LARGE "Large"
#define LGP_EDIT_FONT_LARGE_BOLD "Large+Bold"
#define LGP_CFG_KEYHOOK_SETTING "KeyHook Settings"
#define LGP_CFG_1ST_KEY "1st Key"
#define LGP_CFG_2ND_KEY "2nd Key"
#define LGP_CFG_ICONBAR_SETTING "Iconbar Settings"
#define LGP_CFG_ENA_IB "Enable Iconbar"
#define LGP_CFG_ICON_IB "New SMS Picture ID"
#define LGP_CFG_ENA_AUTO_SAVE_DRAFT "Auto Save Draft"
#define LGP_CFG_PROFILE1_SET "Profile1"
#define LGP_CFG_PROFILE2_SET "Profile2"
#define LGP_CFG_PROFILE3_SET "Profile3"
#define LGP_CFG_PROFILE4_SET "Profile4"
#define LGP_CFG_PROFILE5_SET "Profile5"
#define LGP_CFG_PROFILE6_SET "Profile6"
#define LGP_CFG_PROFILE7_SET "Profile7"
#define LGP_CFG_PROFILE8_SET "Profile8"

#ifdef MEGADIAL
#define LGP_CFG_MDSETTING "MegaDial Settings"
#define LGP_CFG_MDBGCOL "Color Menu background"
#define LGP_CFG_MDMNBDCOL "Color Menu border"
#define LGP_CFG_MDNSLCOL "Color Not selected"
#define LGP_CFG_MDSLCOL "Color Selected"
#define LGP_CFG_MDSLBGCOL "Color Selected background"
#define LGP_CFG_MDSLBDCOL "Color Selected border"
#define LGP_CFG_MDSHMKCOL "Color Search mark"
#define LGP_CFG_ENA_MD "Enable MegaDial"
#endif

#define LGP_CFG_ENA_SNED_ON_BG "Send SMS on Background"
#define LGP_CFG_ICONSNED_IB "Sending Picture ID"
#define LGP_CFG_DEFAULT_SENT_NUM "Default Sent Number"
#define LGP_CFG_FILTERSET "Filter Settings"
#define LGP_CFG_FILTEROP "Filter Options"
#define LGP_FILTER_ALL "All"
#define LGP_FILTER_NEW "New SMS"
#define LGP_FILTER_DAT "From SMS.dat"
#define LGP_FILTER_FILE "From Files"
#define LGP_FILTER_NUM "Number"
#define LGP_FILTER_STR "Strings"
#define LGP_CFG_STRORNUM "Str Or Num"
#define LGP_CFG_DEFAULT_TAB "Default Tab"
#define LGP_CFG_MAINMENU "MainMenu"
#define LGP_CFG_INBOX "Inbox"
#define LGP_CFG_SENT "Sent"
#define LGP_CFG_FILTER "Filter"
#define LGP_CFG_DRAFT "Draft"
#define LGP_CFG_ENA_VIBRA "Enable Vibra"
#define LGP_CFG_VIBRA_RHYTHM "Vibra Rhythm"
#endif

//#endif


#ifdef LANG_RU
#define LGP_CFG_MAIN_FOLDER "Ïàïêà MySMSYS"
#define LGP_CFG_SYSTEM_FOLDER "Ïàïêà System"
#define LGP_CFG_ENA_NOTIFY "Âñïëûâàþù New SMS"
#define LGP_CFG_ENA_PRE "Ïðåäïðîñìîòð âñïëûâ."
#define LGP_CFG_ENA_SCREEN "Ðèñîâàòü íà çàñòàâêå"
#define LGP_CFG_VIBRA_POWER "Vibra Power"
#define LGP_CFG_NOTIFY_SET "Notify Setup"
#define LGP_CFG_NOTIFY_TIME "Notify Time (s)"
#define LGP_CFG_ENA_AUTO_SAF "Auto Save As File At View"
#define LGP_CFG_ENA_AUTO_SAF_RT "Auto Save As File Real Time"
#define LGP_CFG_ENA_NEWSMS_ICON "Show NewSMS Icon Idle"
#define LGP_CFG_ICONNEW_POS "Icon Position"
#define LGP_CFG_EXPORT_CHARSET "Export Text Charset"
#define LGP_CFG_ENA_SAVE_SENT "Save Sent SMS"
#define LGP_CFG_ENA_SOUND "Enable Sound Notify"
#define LGP_CFG_SOUND_VOL "Ãðîìêîñòü"
#define LGP_CFG_SOUND_PATH "Sound File Path"
#define LGP_CFG_ENA_AUTO_DEL_RP "Auto Delete Message Report"
#define LGP_CFG_ENA_EXIT_SAVE_DRAFT "Exit After Save Draft"
#define LGP_CFG_TEXT_FONT "Text Font"
#define LGP_EDIT_FONT_DEFAULT "Default" 
#define LGP_EDIT_FONT_SMALL "Small"
#define LGP_EDIT_FONT_SMALL_BOLD "Small+Bold"
#define LGP_EDIT_FONT_MEDIUM "Medium"
#define LGP_EDIT_FONT_MEDIUM_BOLD "Medium+Bold"
#define LGP_EDIT_FONT_LARGE "Large"
#define LGP_EDIT_FONT_LARGE_BOLD "Large+Bold"
#ifdef ELKA
#define LGP_CFG_ENA_SLI "Enable SLI"
#endif
#define LGP_CFG_KEYHOOK_SETTING "KeyHook Settings"
#define LGP_CFG_1ST_KEY "1st Key"
#define LGP_CFG_2ND_KEY "2nd Key"
#define LGP_CFG_ICONBAR_SETTING "Iconbar Settings"
#define LGP_CFG_ENA_IB "Enable Iconbar"
#define LGP_CFG_ICON_IB "New SMS Picture ID"
#define LGP_CFG_ENA_AUTO_SAVE_DRAFT "Auto Save Draft"
#define LGP_CFG_PROFILE1_SET "Profile1"
#define LGP_CFG_PROFILE2_SET "Profile2"
#define LGP_CFG_PROFILE3_SET "Profile3"
#define LGP_CFG_PROFILE4_SET "Profile4"
#define LGP_CFG_PROFILE5_SET "Profile5"
#define LGP_CFG_PROFILE6_SET "Profile6"
#define LGP_CFG_PROFILE7_SET "Profile7"
#define LGP_CFG_PROFILE8_SET "Profile8"

#ifdef MEGADIAL
#define LGP_CFG_MDSETTING "MegaDial Settings"
#define LGP_CFG_MDBGCOL "Color Menu background"
#define LGP_CFG_MDMNBDCOL "Color Menu border"
#define LGP_CFG_MDNSLCOL "Color Not selected"
#define LGP_CFG_MDSLCOL "Color Selected"
#define LGP_CFG_MDSLBGCOL "Color Selected background"
#define LGP_CFG_MDSLBDCOL "Color Selected border"
#define LGP_CFG_MDSHMKCOL "Color Search mark"
#define LGP_CFG_ENA_MD "Enable MegaDial"
#endif

#define LGP_CFG_ENA_SNED_ON_BG "Send SMS on Background"
#define LGP_CFG_ICONSNED_IB "Sending Picture ID"
#define LGP_CFG_DEFAULT_SENT_NUM "Default Sent Number"
#define LGP_CFG_FILTERSET "Filter Settings"
#define LGP_CFG_FILTEROP "Filter Options"
#define LGP_FILTER_ALL "All"
#define LGP_FILTER_NEW "New SMS"
#define LGP_FILTER_DAT "From SMS.dat"
#define LGP_FILTER_FILE "From Files"
#define LGP_FILTER_NUM "Number"
#define LGP_FILTER_STR "Strings"
#define LGP_CFG_STRORNUM "Str Or Num"
#define LGP_CFG_DEFAULT_TAB "Default Tab"
#define LGP_CFG_MAINMENU "MainMenu"
#define LGP_CFG_INBOX "Inbox"
#define LGP_CFG_SENT "Sent"
#define LGP_CFG_FILTER "Filter"
#define LGP_CFG_DRAFT "Draft"
#define LGP_CFG_ENA_VIBRA "Enable Vibra"
#define LGP_CFG_VIBRA_RHYTHM "Vibra Rhythm"
#endif

#endif

