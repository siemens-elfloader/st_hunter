#include "..\inc\swilib.h"
#include "add_swilib.h"

//BCFG Config
extern const unsigned int bcfg_csm_act;
extern const int bcfg_markcsm_ena;

extern const int bcfg_0x136_ena;
extern const int bcfg_0x137_ena;
extern const int bcfg_0x138_ena;
extern const int bcfg_0x139_ena;
extern const int bcfg_0x13A_ena;

extern const int bcfg_0x152_ena;
extern const int bcfg_0x167_ena;
extern const int bcfg_0x217_ena;

extern const int bcfg_0x04A_ena;
extern const int bcfg_0x099_ena;

extern const unsigned int bcfg_fss_fstype;
extern const unsigned int bcfg_fss_type;
extern const int bcfg_fss_key;
extern const int bcfg_fss_msg;

extern const int bcfg_isnsg;


extern const unsigned int bcfg_ka_type;
extern const unsigned int bcfg_type;
extern const int bcfg_key;
extern const int bcfg_msg;

extern const int bcfg_ls_ena;
extern const int bcfg_rs_ena;
extern const int bcfg_lb_ena;
extern const int bcfg_rb_ena;
extern const int bcfg_ub_ena;
extern const int bcfg_db_ena;



//Replaced Functions
//Create CSM
extern int   CreateCSM_mod(CSM_DESC_NSG* csm_desc,void *a2,int a3);

//Craete GUI
extern int   CreateGUI_mod(void *gui);
extern int   CreateGUI_ID_mod(void *gui, int id);
extern int   CreateGUI_30or2_mod(void *gui);
extern int   CreateGUIWithDummyCSM_mod(void *gui, int flag);
extern int   CreateGUIWithDummyCSM_30or2_mod(void *gui, int flag);

//Craete UI
extern int   CreateInputTextDialog_mod(INPUTDIA_DESC *input_desc, HEADER_DESC *header_desc,void *editq,int do_mfree,void *user_pointer);
extern int   CreateMenu_mod(int is_small,int zero1,const MENU_DESC *menu, HEADER_DESC *hdr,int start_item,int n_items,void *user_pointer,int *to_remove);
extern int   CreateMultiLinesMenu_mod(int dummy,int dummy2,const ML_MENU_DESC *menu, HEADER_DESC *hdr,int to_item,int n);

//PlayMusic Functions
extern short PlayFile_mod(int flags, WSHDR *foldername, WSHDR *filename, int CepId, int Msg, void *sfo, void *sfo2);
extern int   PlayMelodyInMem_mod(char Unk_0x11, void * MelAddr, int MelSize, int CepId, int Msg, int Unk_0);

//File System Functions
extern int   FindFirstFile_mod(DIR_ENTRY_NSG *DIRENTRY, char *mask, unsigned int *ErrorNumber);
extern int   FindNextFile_mod(DIR_ENTRY_NSG *DIRENTRY, unsigned int *ErrorNumber);
extern int   FindClose_mod(DIR_ENTRY_NSG *DIRENTRY, unsigned int *ErrorNumber);


#define ELF_NAME    "NSED"   // Имя эльфа
#define ELF_AUTHOR  "Dimadze"// Автор эльфа, то есть я
#define ELF_VERSION "0.5"    // Версия эльфа

//------------------- NSD SIEMENS ELF ------------
const int minus11=-11;
int keyup = 1, key_hook = 0;
unsigned short maincsm_name_body[140];

GBS_MSG_NSG gbs_msg_nsg_msg, gbs_msg_nsg_key;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  WSHDR *ws2;
  int i1;
}MAIN_GUI;

int nse_keyhook(int submsg, int msg);
extern void InitConfig();
extern int  onKey_bx(GUI *gui, GUI_MSG *msg, void *addr);
extern int  onMessage_bx(CSM_RAM *data_asm, GBS_MSG *msg_asm, void *addr_asm);


int nse_keyhook(int submsg, int msg)
{
   if (((bcfg_type==1 && msg==KEY_DOWN) || (bcfg_type==2 && msg==LONG_PRESS)) && submsg==bcfg_key && keyup == 1)
    {
     int *lib = (int *)(*((int *)(int*)LIB_TOP()));
     if  (NSE_keyAnswerType() == 0)
      {
       if (bcfg_msg == 1) ShowMSG(1,(int)"NSE KA: SMART!");  else PLAYTONE(1);
       lib[0xD01] = 1;
       keyup = 0;
       return (2);
      }
    if  (NSE_keyAnswerType() == 1)
      {
       if (bcfg_msg == 1) ShowMSG(1,(int)"NSE KA: NSG!");  else PLAYTONE(1);
       lib[0xD01] = 2;
       keyup = 0;
       return (2);
      } 
    if  (NSE_keyAnswerType() == 2)
      {
       if (bcfg_msg == 1) ShowMSG(1,(int)"NSE KA: SGOLD!");  else PLAYTONE(1);
       lib[0xD01] = 0;
       keyup = 0;
       return (2);
      } 
    }
   
    if (((bcfg_fss_type==1 && msg==KEY_DOWN) || (bcfg_fss_type==2 && msg==LONG_PRESS)) && submsg==bcfg_fss_key && keyup == 1)
    {
     int *lib = (int *)(*((int *)(int*)LIB_TOP()));
     if (NSE_fileSystemType() == 0)
      {
       if (bcfg_fss_msg == 1) ShowMSG(1,(int)"NSE FS:\nSMART!"); else PLAYTONE(1);
       lib[0xD02] = 1;
       keyup = 0;
       return (2);
      }
     if (NSE_fileSystemType() == 1)
      {
       if (bcfg_fss_msg == 1) ShowMSG(1,(int)"NSE FS:\nNSG!"); else PLAYTONE(1);
       lib[0xD02] = 2;
       keyup = 0;
       return (2);
      }
     if (NSE_fileSystemType() == 2)
      {
       if (bcfg_fss_msg == 1) ShowMSG(1,(int)"NSE FS:\nSGOLD!"); else PLAYTONE(1);
       lib[0xD02] = 0;
       keyup = 0;
       return (2);
      } 
    }
   
   if (msg == KEY_UP) keyup = 1;

 return(0);
}

int nse_onkey(GUI *gui, GUI_MSG *guimsg, void *addr)
{
 if (NSE_keyAnswerType() == KA_SMART)
  {
   CSM_RAM *csm = CSM_root()->csm_q->csm.last;
   CSM_DESC *desc = csm->constr;
   
   if (desc->datasize == NSGCSM)
    {
     if (gui->unk4 != ELFGUILABEL && ((guimsg->gbsmsg->submess == UP_BUTTON && bcfg_ub_ena == 1)     ||
                                      (guimsg->gbsmsg->submess == DOWN_BUTTON && bcfg_db_ena == 1)   ||
                                      (guimsg->gbsmsg->submess == LEFT_BUTTON && bcfg_lb_ena == 1)   ||
                                      (guimsg->gbsmsg->submess == RIGHT_BUTTON  && bcfg_rb_ena == 1) ||
                                      (guimsg->gbsmsg->submess == LEFT_SOFT  && bcfg_ls_ena == 1)    ||
                                      (guimsg->gbsmsg->submess == RIGHT_SOFT  && bcfg_rs_ena == 1))
                                      ) return onKey_bx(gui, guimsg, addr);
        
     gbs_msg_nsg_key.pid_from = guimsg->gbsmsg->pid_from;
     gbs_msg_nsg_key.msg      = guimsg->gbsmsg->msg;
     gbs_msg_nsg_key.submess  = guimsg->gbsmsg->submess;
     gbs_msg_nsg_key.data0    = guimsg->gbsmsg->data0;
     gbs_msg_nsg_key.data1    = guimsg->gbsmsg->data1;
   
     guimsg->gbsmsg = (GBS_MSG *)&gbs_msg_nsg_key;
     
     return onKey_bx(gui, guimsg, addr);
    }
       
  }
  
 if (NSE_keyAnswerType() == KA_NSG)
  {
   gbs_msg_nsg_key.pid_from = guimsg->gbsmsg->pid_from;
   gbs_msg_nsg_key.msg      = guimsg->gbsmsg->msg;
   gbs_msg_nsg_key.submess  = guimsg->gbsmsg->submess;
   gbs_msg_nsg_key.data0    = guimsg->gbsmsg->data0;
   gbs_msg_nsg_key.data1    = guimsg->gbsmsg->data1;
   
   guimsg->gbsmsg = (GBS_MSG *)&gbs_msg_nsg_key;
  }
 
  return onKey_bx(gui, guimsg, addr);
}

int nse_onmessage(CSM_RAM *data, GBS_MSG *msg, void *addr, int type_csm)
{
 if (type_csm == NSGCSM)
  {
   gbs_msg_nsg_msg.pid_from = msg->pid_from;
   gbs_msg_nsg_msg.msg      = msg->msg;
   
   if (msg->msg == MSG_GUI_DESTROYED)      gbs_msg_nsg_msg.msg = MSG_GUI_DESTROYED_NSG;
   if (msg->msg == MSG_RECONFIGURE_REQ)    gbs_msg_nsg_msg.msg = MSG_RECONFIGURE_REQ_NSG;
   if (msg->msg == MSG_IPC)                gbs_msg_nsg_msg.msg = MSG_IPC_NSG;
   
   gbs_msg_nsg_msg.submess  = msg->submess;
   gbs_msg_nsg_msg.data0    = msg->data0;
   gbs_msg_nsg_msg.data1    = msg->data1;
   
   msg = (GBS_MSG *)&gbs_msg_nsg_msg;
  }
 
 return onMessage_bx(data, msg, addr);
}



void CreateSecondaryRamLib()
{
       //Secondary RamSwiLib Init
       int *lib = (int *)(*((int *)(int*)LIB_TOP()));
       
       //Service Lib
       lib[0xD00] = 1;                   // NSE launh mode (0 - don't work, 1 - work)
       lib[0xD01] = bcfg_ka_type;        // NSE KeyAnswer Type (0 - SGold, 1 - Smart, 2 - NewSGold)
       lib[0xD02] = bcfg_fss_fstype;     // NSE File System Flag  (0 - SGold, 1 - Smart, 2 - NewSGold)
       
       lib[0xD03] = (int)&nse_onkey;     // NSE Universal OnKey Func Address
       lib[0xD04] = (int)&nse_onmessage; // NSE Universal OnMessage Func Address
       
       //User Lib
       lib[0xD10] = lib[0x107]; // int CreateCSM(const CSM_DESC*,void *, int)
       
       lib[0xD11] = lib[0x137]; // int CreateGUI(void *gui)
       lib[0xD12] = lib[0x136]; // int CreateGUI_ID(void *gui, int id)
       lib[0xD13] = lib[0x138]; // int CreateGUI_30or2(void *gui)
       lib[0xD14] = lib[0x139]; // int CreateGUIWithDummyCSM(void *gui,int flag)

       lib[0xD15] = lib[0x13A]; // int CreateGUIWithDummyCSM_30or2(void *gui,int flag)
       
       lib[0xD20] = lib[0x167]; // int CreateInputTextDialog(const INPUTDIA_DESC *input_desc, const HEADER_DESC *header_desc,void *editq,int do_mfree,void *user_pointer)
       lib[0xD21] = lib[0x152]; // int CreateMenu(int is_small,int zero1,const MENU_DESC *menu, const HEADER_DESC *hdr,int start_item,int n_items,void *user_pointer,int *to_remove)
       lib[0xD22] = lib[0x217]; // int CreateMultiLinesMenu(int dummy,int dummy2,const ML_MENU_DESC *,const HEADER_DESC *,int to_item,int n)

       lib[0xD40] = lib[0x04A]; // short PlayFile(int flags, WSHDR *foldername, WSHDR *filename, int zero, int CepId, int Msg, const PLAYFILE_OPT *sfo)
       lib[0xD41] = lib[0x099]; // int   PlayMelodyInMem(char Unk_0x11, void * MelAddr, int MelSize, int CepId, int Msg, int Unk_0)
   
       lib[0xD50] = lib[0x06B]; // int FindFirstFile(DIR_ENTRY *DIRENTRY, char *mask, unsigned int *ErrorNumber)
       lib[0xD51] = lib[0x06C]; // int FindNextFile(DIR_ENTRY *DIRENTRY, unsigned int *ErrorNumber)
       lib[0xD52] = lib[0x06D]; // int FindClose(DIR_ENTRY *DIRENTRY, unsigned int *ErrorNumber)
       
       
       //New RamSwiLib
       lib[0x107] = (int)&CreateCSM_mod;
       
       if (bcfg_0x137_ena == 1)
       lib[0x137] = (int)&CreateGUI_mod;
       if (bcfg_0x136_ena == 1)
       lib[0x136] = (int)&CreateGUI_ID_mod;
       if (bcfg_0x138_ena == 1)
       lib[0x138] = (int)&CreateGUI_30or2_mod;
       if (bcfg_0x139_ena == 1)
       lib[0x139] = (int)&CreateGUIWithDummyCSM_mod;
       if (bcfg_0x13A_ena == 1)
       lib[0x13A] = (int)&CreateGUIWithDummyCSM_30or2_mod;
       
       if (bcfg_0x167_ena == 1)
       lib[0x167] = (int)&CreateInputTextDialog_mod;
       if (bcfg_0x152_ena == 1)
       lib[0x152] = (int)&CreateMenu_mod;
       if (bcfg_0x217_ena == 1)
       lib[0x217] = (int)&CreateMultiLinesMenu_mod;
       
       if (bcfg_0x04A_ena == 1)
       lib[0x04A] = (int)&PlayFile_mod;
       if (bcfg_0x099_ena == 1)
       lib[0x099] = (int)&PlayMelodyInMem_mod;
       
       lib[0x06B] = (int)&FindFirstFile_mod;
       lib[0x06C] = (int)&FindNextFile_mod;
       lib[0x06D] = (int)&FindClose_mod;
       
       lib[0x1B5] =  bcfg_isnsg;
}



void RepairRAMSWILib()
{
 LockSched();
 int *lib = (int *)(*((int *)(int*)LIB_TOP()));

 lib[0x107] = lib[0xD10];
       
 if (bcfg_0x137_ena == 1) lib[0x137] = lib[0xD11];
 if (bcfg_0x136_ena == 1) lib[0x136] = lib[0xD12];
 if (bcfg_0x138_ena == 1) lib[0x138] = lib[0xD13];
 if (bcfg_0x139_ena == 1) lib[0x139] = lib[0xD14];
 if (bcfg_0x13A_ena == 1) lib[0x13A] = lib[0xD15];
       
 if (bcfg_0x167_ena == 1) lib[0x167] = lib[0xD20];
 if (bcfg_0x152_ena == 1) lib[0x152] = lib[0xD21];
 if (bcfg_0x217_ena == 1) lib[0x217] = lib[0xD22];
       
 if (bcfg_0x04A_ena == 1) lib[0x04A] = lib[0xD40];
 if (bcfg_0x099_ena == 1) lib[0x099] = lib[0xD41];
 

 lib[0x06B] = lib[0xD50];
 lib[0x06C] = lib[0xD51];
 lib[0x06D] = lib[0xD52];
       
 lib[0x1B5] = 0;
 
 for (int i = 0xD00; i<0xE00; i++) lib[i] = 0xFFFFFFFF;
 UnlockSched();
}





static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg == MSG_GUI_DESTROYED) && ((int)msg->data0 == csm->gui_id)) csm->csm.state=-3;
  if ( msg->msg == MSG_RECONFIGURE_REQ )
   {
    RepairRAMSWILib(); 
    InitConfig();
    CreateSecondaryRamLib();
    
    if (bcfg_type >  0 && key_hook == 0) {AddKeybMsgHook((void *)nse_keyhook);    key_hook = 1;}
    if (bcfg_type == 0 && key_hook == 1) {RemoveKeybMsgHook((void *)nse_keyhook); key_hook = 0;}
   }
  return(1);
}


static void maincsm_oncreate(CSM_RAM *data){}


//Function for unload this elf from RAM
static void ELFUnload(void)
{
  extern void *ELF_BEGIN;
  extern void kill_data(void *p, void (*func_p)(void *));
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}


static void maincsm_onclose(CSM_RAM *csm)
{
  RemoveKeybMsgHook((void *)nse_keyhook);
  RepairRAMSWILib();
  SUBPROC((void *)ELFUnload);
}


static unsigned short maincsm_name_body[140];

static const struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
}MAINCSM =
{
  {
  maincsm_onmessage,
  maincsm_oncreate,
  maincsm_onclose,
  sizeof(MAIN_CSM),
  1,
  &minus11
  },
  {
    maincsm_name_body,
    NAMECSM_MAGIC1,
    NAMECSM_MAGIC2,
    0x0,
    139
  }
};

//Function for update CSM name
static void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),ELF_NAME" v"ELF_VERSION);
}
//----------------------------------------------------


//Main Function
int main(char *exename, char *fname)
{
 if (isnewSGold() == 0)
  {
   if ((int *)(*((int *)(int*)LIB_TOP())))
    {
     int *lib = (int *)(*((int *)(int*)LIB_TOP()));
     
     if (lib[0xD00] == 0xFFFFFFFF)
      {
       //Getting BCFG Settings
       InitConfig();
      
       //NSD Init
       CSM_RAM *save_cmpc;
       char dummy[sizeof(MAIN_CSM)];
       UpdateCSMname();
  
       LockSched();
       
       save_cmpc=CSM_root()->csm_q->current_msg_processing_csm;
       CSM_root()->csm_q->current_msg_processing_csm=CSM_root()->csm_q->csm.first;
       CreateCSM(&MAINCSM.maincsm,dummy,0);
       CSM_root()->csm_q->current_msg_processing_csm=save_cmpc;

       //Secondary RamSwiLib Init
       CreateSecondaryRamLib();
       
       if (bcfg_type>0) {AddKeybMsgHook((void *)nse_keyhook); key_hook = 1;}
   
       UnlockSched();
      } 
       else
         {
          ShowMSG(1,(int)"NSE Launch Error!");
          SUBPROC((void *)ELFUnload);
         }
    }
     else 
       {
        ShowMSG(1,(int)"RAM SWI\nnot load!");
        SUBPROC((void *)ELFUnload);
       }
  }
   else 
     {
      ShowMSG(1,(int)"This ELF\nfor SGold!");
      SUBPROC((void *)ELFUnload);
     }
  
  return 0;
}
