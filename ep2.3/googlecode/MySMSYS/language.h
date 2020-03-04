
extern const char my_ipc_name[];
extern const char ELFNAME[];
extern const char ELFNAME_D[];
extern const char COPY_RIGHT[];

extern const char FLDR_IN[];
extern const char FLDR_OUT[];
extern const char FLDR_DRAFT[];
extern const char FLDR_UNK[];
extern const char FLDR_ARCHIVE[];

extern const char PERCENT_T[];
extern const char PERCENT_S[];
extern const char num_fetion[];
extern const char STR_LINES[];
extern const char STR_UNK_TXTT[];
extern const char STR_UNK_TYPE[];

#ifdef LANG_CN
#define LGP_CFG_MAIN_FOLDER "MySMSYS�ļ���"
#define LGP_CFG_SYSTEM_FOLDER "ϵͳ�ļ���"
#define LGP_CFG_ENA_NOTIFY "��������Ϣ����"
#define LGP_CFG_VIBRA_POWER "��ǿ��"
#define LGP_CFG_NOTIFY_SET "����Ϣ��������"
#define LGP_CFG_NOTIFY_TIME "����ʱ�䳤��(��)"
#define LGP_CFG_ENA_AUTO_SAF "�鿴ʱת��Ϊ�ļ�"
#define LGP_CFG_ENA_AUTO_SAF_RT "��ʱת��Ϊ�ļ�"
#define LGP_CFG_ENA_NEWSMS_ICON "������ʾ�¶���ͼ��"
//#define LGP_CFG_ICONNEW_PATH "ͼ��"
#define LGP_CFG_ICONNEW_POS "ͼ������"
#define LGP_CFG_EXPORT_CHARSET "�����ı��ַ�����"
#define LGP_CFG_ENA_SAVE_SENT "�����ѷ��Ͷ���"
#define LGP_CFG_ENA_SOUND "ʹ����������"
#define LGP_CFG_SOUND_VOL "����"
#define LGP_CFG_SOUND_PATH "�����ļ�·��"
#define LGP_CFG_ENA_AUTO_DEL_RP "�Զ�ɾ����Ϣ����"
#define LGP_CFG_ENA_EXIT_SAVE_DRAFT "����ݸ���˳�"
#define LGP_CFG_TEXT_FONT "�ı�����"
#define LGP_EDIT_FONT_DEFAULT "ʹ��Ĭ��" 
#define LGP_EDIT_FONT_SMALL "С������"
#define LGP_EDIT_FONT_SMALL_BOLD "С����Ӵ�"
#define LGP_EDIT_FONT_MEDIUM "�к�����"
#define LGP_EDIT_FONT_MEDIUM_BOLD "������Ӵ�"
#define LGP_EDIT_FONT_LARGE "�������"
#define LGP_EDIT_FONT_LARGE_BOLD "������Ӵ�"
#ifdef ELKA
#define LGP_CFG_ENA_SLI "ʹ��SLI"
#endif
#define LGP_CFG_KEYHOOK_SETTING "���������������"
#define LGP_CFG_1ST_KEY "����1"
#define LGP_CFG_2ND_KEY "����2"
#define LGP_CFG_ICONBAR_SETTING "����Ϣͼ��������"
#define LGP_CFG_ENA_IB "ʹ��ͼ����"
#define LGP_CFG_ICON_IB "�¶���ͼ��ID"
#define LGP_CFG_ENA_AUTO_SAVE_DRAFT "�Զ�����ݸ�"
#define LGP_CFG_PROFILE1_SET "�龰ģʽ1"
#define LGP_CFG_PROFILE2_SET "�龰ģʽ2"
#define LGP_CFG_PROFILE3_SET "�龰ģʽ3"
#define LGP_CFG_PROFILE4_SET "�龰ģʽ4"
#define LGP_CFG_PROFILE5_SET "�龰ģʽ5"
#define LGP_CFG_PROFILE6_SET "�龰ģʽ6"
#define LGP_CFG_PROFILE7_SET "�龰ģʽ7"
#define LGP_CFG_PROFILE8_SET "�龰ģʽ8"
#define LGP_CFG_ENA_SNED_ON_BG "��̨���Ͷ���"
#define LGP_CFG_ICONSNED_IB "���ŷ���ͼ��ID"
#define LGP_CFG_DEFAULT_SENT_NUM "Ĭ�Ϸ��ͺ���"
#define LGP_CFG_FILTERSET "��������"
#define LGP_CFG_FILTEROP "����ѡ��"
#define LGP_FILTER_ALL "ȫ��"
#define LGP_FILTER_NEW "δ������"
#define LGP_FILTER_DAT "����SMS.dat"
#define LGP_FILTER_FILE "�����ļ�"
#define LGP_FILTER_NUM "����"
#define LGP_FILTER_STR "�ַ���"
#define LGP_CFG_STRORNUM "�ַ��������"
#define LGP_CFG_DEFAULT_TAB "Ĭ�ϱ�ǩ"
#define LGP_CFG_MAINMENU "���˵�"
#define LGP_CFG_INBOX "�ռ���"
#define LGP_CFG_SENT "�ѷ���"
#define LGP_CFG_FILTER "ɸѡ��ʾ"
#define LGP_CFG_DRAFT "�ݸ�"
#endif

#ifdef LANG_EN
#define LGP_CFG_MAIN_FOLDER "MySMSYS Folder"
#define LGP_CFG_SYSTEM_FOLDER "System Folder"
#define LGP_CFG_ENA_NOTIFY "Popup New SMS"
#define LGP_CFG_VIBRA_POWER "Vibra Power"
#define LGP_CFG_NOTIFY_SET "Notify Setup"
#define LGP_CFG_NOTIFY_TIME "Notify Time (s)"
#define LGP_CFG_ENA_AUTO_SAF "Auto Save As File At View"
#define LGP_CFG_ENA_AUTO_SAF_RT "Auto Save As File Real Time"
#define LGP_CFG_ENA_NEWSMS_ICON "Show NewSMS Icon Idle"
//#define LGP_CFG_ICONNEW_PATH "Icon"
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
#endif

typedef struct{
  const char *LGP_OK;
  const char *LGP_BACK;
  const char *LGP_ADRBK;
  const char *LGP_ERR;
  const char *LGP_SELECT;
  const char *LGP_QUIT;
  const char *LGP_NEW;
  const char *LGP_IN_R;
  const char *LGP_IN_N;
  const char *LGP_IN_A;
  const char *LGP_OUT;
  const char *LGP_DRAFT;
  const char *LGP_ALL;
  const char *LGP_OPTIONS;
  const char *LGP_ERR_0NUM;
  const char *LGP_EXIT;
  const char *LGP_CANCEL;
  const char *LGP_SEND;
  const char *LGP_DEL;
  const char *LGP_SAVE_AS_FILE;
  const char *LGP_SAVE_AS_DRAFT;
  const char *LGP_INSERT_ADRNUM;
  const char *LGP_INSERT_BLANKNUM;
  const char *LGP_CHAR_COUNT;
  const char *LGP_VIEW;
  const char *LGP_EDIT;
  const char *LGP_REPLY;
  const char *LGP_NEW_MSG;
  const char *LGP_FROM;
  const char *LGP_CODESHOW;
  const char *LGP_FETION;
  const char *LGP_SAVEALL;
  const char *LGP_CONFIG_UPDATE;
  const char *LGP_CONFIG;
  const char *LGP_SAVE_ALL_ONE;
  const char *LGP_PLS_INPUT_PATH;
  const char *LGP_OTHERS;
  const char *LGP_FILE_FAILED;
  const char *LGP_FILE_EXIST;
  const char *LGP_EXPORT_TXT;
  const char *LGP_MOVE_ARCHIVE;
  const char *LGP_MOVE_ALL_MSS;
  const char *LGP_OPEN_ARCHIVE;
  const char *LGP_EXPORT_N;
  const char *LGP_MOVE_MSSARCHIVER_N;
  const char *LGP_DEL_ALL_MSS;
  const char *LGP_ABOUT;
  const char *LGP_COV_DAT_TXT;
  const char *LGP_SAVE_ALL_AS_FILE;
  const char *LGP_SAF_N;
  const char *LGP_DEL_ALL;
  const char *LGP_DEL_ALL_MSG;
  const char *LGP_MOVE_ALL;
  const char *LGP_DEL_ALL_MSG_MSS;
  const char *LGP_SAVE;
  const char *LGP_MSG_REPORT;
  const char *LGP_MSG_REVEICED; //���ʱ��
  const char *LGP_UNK_RP_STATUS;
  const char *LGP_TEMPLATE;
  const char *LGP_CALL_ADDRESSBOOK;
  const char *LGP_STATISTICS;
  const char *LGP_IN_SMSDAT;
  const char *LGP_IS_MSSFILE;
  const char *LGP_DAT_FREE;
  const char *LGP_DISK_FREE;
  const char *LGP_MSS_MAINMENU;
  const char *LGP_TRANSLIT;
  const char *LGP_TIME;
  const char *LGP_UNK_NUM;
  const char *LGP_NUMBER;
  const char *LGP_TEXT;
  const char *LGP_TO;
  const char *LGP_UNK;
  const char *LGP_FILTER;
}LGPACK;

extern LGPACK lgp;
/*
extern const char STR_FROM[];
extern const char STR_UNK_NUM[];
extern const char STR_NUMBER[];
extern const char STR_TEXT[];
extern const char STR_TO[];
extern const char STR_TIME[];
extern const char STR_UNK[];
*/
extern const char STR_UNK_NUM_UTF8[];
extern const char STR_NUMBER_UTF8[];
extern const char STR_TEXT_UTF8[];
extern const char STR_TO_UTF8[];
extern const char STR_FROM_UTF8[];
extern const char STR_TIME_UTF8[];
extern const char STR_UNK_UTF8[];

//-------------
void LoadLangPack(void);
void FreeLangPack(void);



