

//ELKA
#define DISPLACE_OF_EDGUI_EL 0x90
#define DISPLACE_OF_USSDREQGUI_ID_EL 0x44
#define DISPLACE_OF_EDGUI_ID_EL 0x4C
#define DISPLACE_OF_IDLEGUI_ID_EL 0x2C
#define DISPLACE_OF_IDLECANVAS_EL 0x20

//NSG
#define DISPLACE_OF_EDGUI_NSG 0x88
#define DISPLACE_OF_USSDREQGUI_ID_NSG 0x44
#define DISPLACE_OF_EDGUI_ID_NSG 0x4C
#define DISPLACE_OF_IDLEGUI_ID_NSG 0x2C
#define DISPLACE_OF_IDLECANVAS_NSG 0x18


#define DISPLACE_OF_INCOMMINGGUI_NSG 0x34
#define DISPLACE_OF_OUTGOINGGUI_NSG 0x30
#define MSG_INCOMMING_CALL_NSG 0x6000
#define MSG_STATE_OF_CALL_NSG 0x6009
#define MSG_END_CALL_NSG 0x600B
#define MSG_HELPER_TRANSLATOR_NSG 0xDEAE
#define MSG_RECONFIGURE_REQ_NSG 0xDEAF
#define MSG_IPC_NSG 0xDEB0
#define MSG_SMS_RX_NSG 0x5803
#define MSG_USSD_RX_NSG 0x60E2
#define MSG_AUTOUSSD_RX_NSG 0x60E5

#define MSG_PLAYFILE_REPORT_NSG 0x70BC
#define MSG_CSM_DESTROYED_NSG 0x6400
#define MSG_CSM_DESTROY_NOTIFY_NSG 0x6402
#define MSG_GUI_DESTROYED_NSG 0x640E
#define MSG_IDLETMR_EXPIRED_NSG 0x6401
#define MMI_CEPID_NSG 0x4209
#define LGP_NULL_NSG 0x7FFFFFFF
#define SET_SOFT_KEY_N_NSG 0
#define LGP_DOIT_PIC_NSG 0x7FFFC0FB


//Play File
#define MSG_PLAYFILE_REPORT_NSG   0x70BC
#define MSG_PLAYFILE_REPORT_SGX75 0x174
#define MSG_PLAYFILE_REPORT_SGOLD 0x167


//Key Answers
#define KA_SGOLD 0
#define KA_SMART 1
#define KA_NSG   2

//Sizes MAINCSM use as CSM ID
#define SGOLDCSM     0x28
#define NSGCSM       0x2D //New ID (Default: 0x2C)
#define ELFGUILABEL  0xDD //GUI LABEL for ELF without UI


#define PLAYTONE(A) PlaySound(1,0, 0, A, 0)


typedef struct
{
  RECT *canvas;
  void *methods;
  void *definition;
  char state;
  char unk2;
  char unk3;
  char unk4;
  int color1; //Параметр GeneralFunc пишется сюда?????
  int color2;
  LLQ item_ll;
  int unk5;
  char unk6;
  char unk7;
  char unk8;
  char unk9;
  int unk10;
  int flag30; //Используется при создании (бывает |=0x02)
}GUI_NSG;

typedef struct{
  int pid_from;
  int msg;
  int submess;
  void *data0;
  void *data1;
} GBS_MSG_NSG;

typedef struct
{
  char zero;
  char unk1;
  short keys;
  GBS_MSG_NSG *gbsmsg;
}GUI_MSG_NSG;

typedef struct{
  int (*onMessage)(CSM_RAM*,GBS_MSG_NSG*);
  void (*onCreate)(CSM_RAM*);
  int zero1;
  int zero2;
  int zero3;
  int zero4;
  void (*onClose)(CSM_RAM*);
  int datasize;
  int statesize;
  const int *states;
} CSM_DESC_NSG;


typedef struct{
  unsigned short repeat_num;  //0 - non limited
  unsigned short unk;
  int time_between_play;
  int play_first; // 0 - play all
  int volume;
  int unk5;
  int unk6;
  int unk7;
  int unk8;
  int unk9;
}PLAYFILE_OPT_NSG;

typedef struct{
  unsigned short repeat_num;  //0 - non limited
  unsigned short unk;
  int time_between_play;
  int play_first; // 0 - play all
  int volume;
  int unk4;     //0x80000000
  int unk5;     //1
  int unk6;     //0
  int unk7;
}PLAYFILE_OPT_SGX75;

typedef struct{
  unsigned short repeat_num;  //0 - non limited
  unsigned short unk;
  int time_between_play;
  int play_first; // 0 - play all
  int volume;
  int unk5;
  int unk6;
}PLAYFILE_OPT_SGOLD;


#pragma pack(1)
typedef struct{
  char unk0[40];
  unsigned int file_size;
  short file_attr;
  unsigned int create_date_time;
  char unk1[2];
  char folder_name[128];
  char unk2;
  char file_name[128];
  char unk3[11];
  short wsfolder_name[128+1];
  short wsfile_name[128+1];
}DIR_ENTRY_NSG;
#pragma pack()



//Service Functions

#pragma swi_number=0x8D00
__swi __arm int   NSE_isLaunched();     // 1 = Launch, 0xFFFFFFFF = Not (-1);

#pragma swi_number=0x8D01
__swi __arm int   NSE_keyAnswerType();  // 0, 0xFFFFFFFF - SGold, 1 - Smart, 2 - NewSGold;

#pragma swi_number=0x8D02
__swi __arm int   NSE_fileSystemType(); // 0, 0xFFFFFFFF - SGold, 1 - Smart, 2 - NewSGold;

#pragma swi_number=0x8D03
__swi __arm void *NSE_getOnKeyAddress();

#pragma swi_number=0x8D04
__swi __arm void *NSE_getOnMessageAddress();


//Original Functions

//Create CSM
#pragma swi_number=0x0D10
__swi __arm int CreateCSM_orig(const CSM_DESC*,void *,int);

//Create GUI
#pragma swi_number=0x0D11
__swi __arm int CreateGUI_orig(void *gui);

#pragma swi_number=0x0D12
__swi __arm int CreateGUI_ID_orig(void *gui, int id);

#pragma swi_number=0x0D13
__swi __arm int CreateGUI_30or2_orig(void *gui);

#pragma swi_number=0x0D14
__swi __arm int CreateGUIWithDummyCSM_orig(void *gui, int flag);

#pragma swi_number=0x0D15
__swi __arm int CreateGUIWithDummyCSM_30or2_orig(void *gui, int flag);


//Create UI
#pragma swi_number=0x0D20
__swi __arm int CreateInputTextDialog_orig(const INPUTDIA_DESC *input_desc, const HEADER_DESC *header_desc,void *editq,int do_mfree,void *user_pointer);

#pragma swi_number=0x0D21
__swi __arm int CreateMenu_orig(int is_small,int zero1,const MENU_DESC *menu, const HEADER_DESC *hdr,int start_item,int n_items,void *user_pointer,int *to_remove);

#pragma swi_number=0x0D22
__swi __arm int CreateMultiLinesMenu_orig(int dummy,int dummy2,const ML_MENU_DESC *,const HEADER_DESC *,int to_item,int n);


//Picture Work Functions
#pragma swi_number=0x0D30
__swi __arm IMGHDR *CreateIMGHDRFromPngFile_orig(const char *fname, int type);


//PlayMusic Functions

#pragma swi_number=0x0D40
#ifdef X75
__swi __arm short PlayFile_orig(int flags, WSHDR *foldername, WSHDR *filename, int zero, int CepId, int Msg, PLAYFILE_OPT_SGX75 *sfo);
#else
__swi __arm short PlayFile_orig(int flags, WSHDR *foldername, WSHDR *filename, int CepId, int Msg, PLAYFILE_OPT_SGOLD *sfo);
#endif

#pragma swi_number=0x0D41
__swi __arm int PlayMelodyInMem_orig(char Unk_0x11, void * MelAddr, int MelSize, int CepId, int Msg, int Unk_0);


//File System Functions

#pragma swi_number=0x0D50
__swi __arm int FindFirstFile_orig(DIR_ENTRY *DIRENTRY, char *mask, unsigned int *ErrorNumber);

#pragma swi_number=0x0D51
__swi __arm int FindNextFile_orig(DIR_ENTRY *DIRENTRY, unsigned int *ErrorNumber);

#pragma swi_number=0x0D52
__swi __arm int FindClose_orig(DIR_ENTRY *DIRENTRY, unsigned int *ErrorNumber);

