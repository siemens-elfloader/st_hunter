
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


int CreateCSM_mod(CSM_DESC_NSG *csm_desc, void *a2, int a3)
{
 if (csm_desc->zero1 == 0 && csm_desc->zero2 == 0 && csm_desc->zero3 == 0 && csm_desc->zero4 == 0)
  {
   char *csm_desc_sg = (char *)csm_desc;
   memcpy((char *)csm_desc+8,(char *)csm_desc+24,(sizeof(CSM_DESC_NSG)-24)+sizeof(WSHDR));
   for (int i = 0; i < 24; i++) csm_desc_sg[sizeof(CSM_DESC_NSG) + sizeof(WSHDR) - 24 + i] = 0;
   
   CSM_DESC *csm_desc_temp  = (CSM_DESC *)csm_desc_sg;
   csm_desc_temp->datasize = 0x2D;
   
   if (bcfg_csm_act == 0) 
    {
      ShowMSG(1, (int)"NSE:\nNSG CSM Detect!");
      zeromem(csm_desc, sizeof(CSM_DESC_NSG));
      return 0;
    }
   if (bcfg_markcsm_ena == 1) 
    {
      WSHDR *ws_addname = AllocWS(32);
      wsprintf(ws_addname, " (NSG)");
      WSHDR *ws_csmname = (WSHDR *)((char *)csm_desc_temp + sizeof(CSM_DESC));
      wstrcat (ws_csmname, ws_addname);
      FreeWS(ws_addname);
    }
   
   
   return CreateCSM_orig((CSM_DESC *)csm_desc, a2,a3);
  }
 
 return CreateCSM_orig((CSM_DESC *)csm_desc, a2,a3);
}


//Create GUI
int CreateGUI_mod(void *gui)
{
 GUI_NSG *main_gui_addr = (GUI_NSG *)gui;
 
   main_gui_addr->canvas->x = 0;
   main_gui_addr->canvas->y = 0;
   main_gui_addr->canvas->x2 = ScreenW()-1;
   main_gui_addr->canvas->y2 = ScreenH()-1;
   main_gui_addr->unk4       = 0xDD;
 
 return CreateGUI_orig(gui);
}


int CreateGUI_ID_mod(void *gui, int id)
{
 GUI_NSG *main_gui_addr = (GUI_NSG *)gui;
 
   main_gui_addr->canvas->x = 0;
   main_gui_addr->canvas->y = 0;
   main_gui_addr->canvas->x2 = ScreenW()-1;
   main_gui_addr->canvas->y2 = ScreenH()-1;
   main_gui_addr->unk4       = 0xDD;
 
 return CreateGUI_ID_orig(gui, id);
}


int CreateGUI_30or2_mod(void *gui)
{
 GUI_NSG *main_gui_addr = (GUI_NSG *)gui;
 
   main_gui_addr->canvas->x = 0;
   main_gui_addr->canvas->y = 0;
   main_gui_addr->canvas->x2 = ScreenW()-1;
   main_gui_addr->canvas->y2 = ScreenH()-1;
   main_gui_addr->unk4       = 0xDD;
 
 return CreateGUI_30or2_orig(gui);
}


int CreateGUIWithDummyCSM_mod(void *gui, int flag)
{
 GUI_NSG *main_gui_addr = (GUI_NSG *)gui;
 
   main_gui_addr->canvas->x = 0;
   main_gui_addr->canvas->y = 0;
   main_gui_addr->canvas->x2 = ScreenW()-1;
   main_gui_addr->canvas->y2 = ScreenH()-1;
   main_gui_addr->unk4       = 0xDD;
 
 return CreateGUIWithDummyCSM_orig(gui, flag);
}


int CreateGUIWithDummyCSM_30or2_mod(void *gui, int flag)
{
 GUI_NSG *main_gui_addr = (GUI_NSG *)gui;
 
   main_gui_addr->canvas->x = 0;
   main_gui_addr->canvas->y = 0;
   main_gui_addr->canvas->x2 = ScreenW()-1;
   main_gui_addr->canvas->y2 = ScreenH()-1;
   main_gui_addr->unk4       = 0xDD;
 
 return CreateGUIWithDummyCSM_30or2_orig(gui, flag);
}




//Create UI
int CreateInputTextDialog_mod(INPUTDIA_DESC *input_desc, HEADER_DESC *header_desc,void *editq,int do_mfree,void *user_pointer)
{
 input_desc->rc.x  = 0;
 input_desc->rc.y  = HeaderH() - 1;
 input_desc->rc.x2 = ScreenW() - 1;
 input_desc->rc.y2 = ScreenH() - SoftkeyH() - 1;
 
 header_desc->rc.x  = 0;
 header_desc->rc.y  = 0;
 header_desc->rc.x2 = ScreenW() - 1;
 header_desc->rc.y2 = HeaderH() - 1;
 
 return CreateInputTextDialog_orig(input_desc, header_desc, editq, do_mfree, user_pointer);
}


int CreateMenu_mod(int is_small,int zero1,const MENU_DESC *menu, HEADER_DESC *hdr,int start_item,int n_items,void *user_pointer,int *to_remove)
{
 hdr->rc.x  = 0;
 hdr->rc.y  = 0;
 hdr->rc.x2 = ScreenW() - 1;
 hdr->rc.y2 = HeaderH() - 1;
 
 return CreateMenu_orig(is_small,zero1,menu,hdr,start_item,n_items,user_pointer,to_remove);
}


int CreateMultiLinesMenu_mod(int dummy,int dummy2,const ML_MENU_DESC *menu, HEADER_DESC *hdr,int to_item,int n)
{
 hdr->rc.x = 0;
 hdr->rc.y = 0;
 hdr->rc.x2 = ScreenW()-1;
 hdr->rc.y2 = HeaderH()-1;
 
 return CreateMultiLinesMenu_orig(dummy,dummy2,menu,hdr,to_item,n); 
}

//PlayMusic Functions

#ifdef X75
short PlayFile_mod(int flags, WSHDR *foldername, WSHDR *filename, int CepId, int Msg, void *sfo, void *sfo2)
{
 if (CepId != 0)
  {
   if (flags == 0x10)
    {
     PLAYFILE_OPT_SGX75 sfo_sg_x75;
     PLAYFILE_OPT_NSG *sfo_nsg = (PLAYFILE_OPT_NSG *)sfo;
   
     sfo_sg_x75.unk4=0x80000000;
     sfo_sg_x75.unk5=1;
   
     sfo_sg_x75.repeat_num        = sfo_nsg->repeat_num;
     sfo_sg_x75.unk               = sfo_nsg->unk;
     sfo_sg_x75.time_between_play = sfo_nsg->time_between_play;
     sfo_sg_x75.play_first        = sfo_nsg->play_first;
     sfo_sg_x75.volume            = sfo_nsg->volume;
   
     return PlayFile_orig(flags, foldername, filename, 0, CepId, MSG_PLAYFILE_REPORT_SGX75, (PLAYFILE_OPT_SGX75 *)&sfo_sg_x75);
   } else
      {
       PLAYFILE_OPT_SGX75 sfo_sg_x75;
       PLAYFILE_OPT_SGOLD *sfo_sg = (PLAYFILE_OPT_SGOLD *)sfo;
   
       sfo_sg_x75.unk4=0x80000000;
       sfo_sg_x75.unk5=1;
   
       sfo_sg_x75.repeat_num        = sfo_sg->repeat_num;
       sfo_sg_x75.unk               = sfo_sg->unk;
       sfo_sg_x75.time_between_play = sfo_sg->time_between_play;
       sfo_sg_x75.play_first        = sfo_sg->play_first;
       sfo_sg_x75.volume            = sfo_sg->volume;
   
       return PlayFile_orig(flags, foldername, filename, 0, CepId, MSG_PLAYFILE_REPORT_SGX75, (PLAYFILE_OPT_SGX75 *)&sfo_sg_x75);
      }
  }
 return PlayFile_orig(flags, foldername, filename, CepId, Msg, (int)sfo,(PLAYFILE_OPT_SGX75 *)sfo2);
}

int PlayMelodyInMem_mod(char Unk_0x11, void * MelAddr, int MelSize, int CepId, int Msg, int Unk_0)
{
 return PlayMelodyInMem_orig(Unk_0x11, MelAddr, MelSize, CepId, MSG_PLAYFILE_REPORT_SGX75, Unk_0);
}

#else
short PlayFile_mod(int flags, WSHDR *foldername, WSHDR *filename, int CepId, int Msg, void *sfo, void *sfo2)
{
  if (flags == 0x10)
    {
     PLAYFILE_OPT_SGOLD sfo_sg;
     PLAYFILE_OPT_NSG *sfo_nsg = (PLAYFILE_OPT_NSG *)sfo;
   
     sfo_sg.unk5=1;
   
     sfo_sg.repeat_num        = sfo_nsg->repeat_num;
     sfo_sg.unk               = sfo_nsg->unk;
     sfo_sg.time_between_play = sfo_nsg->time_between_play;
     sfo_sg.play_first        = sfo_nsg->play_first;
     sfo_sg.volume            = sfo_nsg->volume;
   
     return PlayFile_orig(flags, foldername, filename, CepId, MSG_PLAYFILE_REPORT_SGOLD, (PLAYFILE_OPT_SGOLD *)&sfo_sg);
   }
  if (CepId == 0)
   {
     PLAYFILE_OPT_SGOLD sfo_sg;
     PLAYFILE_OPT_SGX75 *sfo_sg_x75 = (PLAYFILE_OPT_SGX75 *)sfo2;
   
     sfo_sg.unk5=1;
   
     sfo_sg.repeat_num        = sfo_sg_x75->repeat_num;
     sfo_sg.unk               = sfo_sg_x75->unk;
     sfo_sg.time_between_play = sfo_sg_x75->time_between_play;
     sfo_sg.play_first        = sfo_sg_x75->play_first;
     sfo_sg.volume            = sfo_sg_x75->volume;
   
     return PlayFile_orig(flags, foldername, filename, Msg, MSG_PLAYFILE_REPORT_SGOLD, (PLAYFILE_OPT_SGOLD *)&sfo_sg);
   }
  
  return PlayFile_orig(flags, foldername, filename, CepId, Msg, (PLAYFILE_OPT_SGOLD *)sfo);
}

int PlayMelodyInMem_mod(char Unk_0x11, void * MelAddr, int MelSize, int CepId, int Msg, int Unk_0)
{
 return PlayMelodyInMem_orig(Unk_0x11, MelAddr, MelSize, CepId, MSG_PLAYFILE_REPORT_SGOLD, Unk_0);
}

#endif


//File System Functions
DIR_ENTRY dir; //Common DIR_ENTRY Struct

int FindFirstFile_mod(DIR_ENTRY_NSG *DIRENTRY, char *mask, unsigned int *ErrorNumber)
{
  if (NSE_fileSystemType() > 0)
  {
    CSM_RAM *csm = CSM_root()->csm_q->csm.last;
    CSM_DESC *desc = csm->constr;
    
    if (desc->datasize != NSGCSM && NSE_fileSystemType() != 2) return FindFirstFile_orig((DIR_ENTRY *)DIRENTRY, mask, ErrorNumber);
    
    dir.file_attr        = DIRENTRY->file_attr;
    dir.file_size        = DIRENTRY->file_size;
    dir.create_date_time = DIRENTRY->create_date_time;
     
    int ret = FindFirstFile_orig((DIR_ENTRY *)&dir, mask, ErrorNumber);
    
    
    DIRENTRY->file_attr        = dir.file_attr;
    DIRENTRY->file_size        = dir.file_size;
    DIRENTRY->create_date_time = dir.create_date_time;
    
    strcpy(DIRENTRY->folder_name, dir.folder_name);
    strcpy(DIRENTRY->file_name, dir.file_name);
     
    return ret;
   }
  
  return FindFirstFile_orig((DIR_ENTRY *)DIRENTRY, mask, ErrorNumber);
}

int FindNextFile_mod(DIR_ENTRY_NSG *DIRENTRY, unsigned int *ErrorNumber)
{
  if (NSE_fileSystemType() > 0)
  {
    
    CSM_RAM *csm = CSM_root()->csm_q->csm.last;
    CSM_DESC *desc = csm->constr;
    
    if (desc->datasize != NSGCSM && NSE_fileSystemType() != 2) return FindNextFile_orig((DIR_ENTRY *)DIRENTRY, ErrorNumber);
    
    dir.file_attr        = DIRENTRY->file_attr;
    dir.file_size        = DIRENTRY->file_size;
    dir.create_date_time = DIRENTRY->create_date_time;
    
    strcpy(dir.folder_name, DIRENTRY->folder_name);
    strcpy(dir.file_name, DIRENTRY->file_name);
     
    int ret = FindNextFile_orig((DIR_ENTRY *)&dir, ErrorNumber);
    
    DIRENTRY->file_attr        = dir.file_attr;
    DIRENTRY->file_size        = dir.file_size;
    DIRENTRY->create_date_time = dir.create_date_time;
    
    strcpy(DIRENTRY->folder_name, dir.folder_name);
    strcpy(DIRENTRY->file_name, dir.file_name);
    
    return ret;
   }
  
  return FindNextFile_orig((DIR_ENTRY *)DIRENTRY, ErrorNumber);
}



int FindClose_mod(DIR_ENTRY_NSG *DIRENTRY, unsigned int *ErrorNumber)
{
 if (NSE_fileSystemType() > 0)
   {
    CSM_RAM *csm = CSM_root()->csm_q->csm.last;
    CSM_DESC *desc = csm->constr;
    
    if (desc->datasize != NSGCSM && NSE_fileSystemType() != 2) return FindClose_orig((DIR_ENTRY *)DIRENTRY, ErrorNumber);
     
    dir.file_attr        = DIRENTRY->file_attr;
    dir.file_size        = DIRENTRY->file_size;
    dir.create_date_time = DIRENTRY->create_date_time;
     
    int ret = FindClose_orig((DIR_ENTRY *)&dir, ErrorNumber);
    
    DIRENTRY->file_attr        = dir.file_attr;
    DIRENTRY->file_size        = dir.file_size;
    DIRENTRY->create_date_time = dir.create_date_time;
    
    strcpy(DIRENTRY->folder_name, dir.folder_name);
    strcpy(DIRENTRY->file_name, dir.file_name);
    
    return ret;
   }
  
  return FindClose_orig((DIR_ENTRY *)DIRENTRY, ErrorNumber);
}

