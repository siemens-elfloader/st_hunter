#include "..\inc\swilib.h"
#include "..\readimg.h"
#include "..\externals.h"
#include "..\conf_loader.h"
#include "../inc/naticq_ipc.h"
#include "check_ipc.h"
#include "smiles_loader.h"
#include "../inc/xtask_ipc.h"
#include "../strings.h"
#include "../inc/pnglist.h"
#include "../smiles.h"

const int minus11=-11;

const char ipc_my_name[] = IPC_CHECKER_NAME;
char ipc_send_name[32] = IPC_NATICQ_NAME;


typedef struct
{
  CSM_RAM csm;
}MAIN_CSM;

CSM_RAM *under_idle;
IPC_REQ gipc;

extern void kill_data(void *p, void (*func_p)(void *));

#pragma inline=forced

int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}


int maincsm_id;
void CheckDoubleRun(void)
{
  int csm_id;
  if ((csm_id=(int)(gipc.data))!=-1)
  {
    //gipc.name_to=IPC_XTASK_NAME;
   // gipc.name_from=ipc_my_name;
 //   gipc.data=(void *)csm_id;
   // GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_SHOW_CSM,&gipc);
    LockSched();
    CloseCSM(maincsm_id);
    ShowMSG(1,(int)"Already started!");
    UnlockSched();
  }
  else
  {
    extern void InitXStatusesImg(void);
    extern void ProcessNextImg(void);
    extern void LoadImages();
    LoadImages();
    InitXStatusesImg();
    ProcessNextImg();
    //InitSmiles(); Это вызовется из InitXStatusesImg
  }
}

extern void ProcessNextSmile(void);
extern void ProcessNextXStatImg(void);

int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
// MAIN_CSM *csm=(MAIN_CSM*)data;
  {
    //IPC
    if (msg->msg==MSG_IPC)
    {
      IPC_REQ *ipc;
      if ((ipc=(IPC_REQ*)msg->data0))
      {
	if (strcmp_nocase(ipc->name_to,ipc_my_name)==0)
	{
/*          if(!strcmp(ipc->name_to,ipc->name_from))
          {
            strcpy(ipc_send_name,ipc->name_from);
            LockSched();
            ShowMSG(1,(int)ipc_send_name);
            UnlockSched();
          }*/
	  switch (msg->submess)
	  {
           case IPC_SEND_ICONS:
             {
         //      extern const unsigned int UIN;
           //   sprintf((char *)ipc_send_name+6,"%d",UIN);
                           strcpy(ipc_send_name,ipc->name_from);
           /* LockSched();
            ShowMSG(1,(int)ipc_send_name);
            UnlockSched();*/
               extern ICONS *iconstop;
                char s[3];
              extern int CountIcon();
              sprintf(s,"count = %d",CountIcon());
               ShowMSG(1,(int)s);
               gipc.name_to = ipc_send_name;
               gipc.name_from = ipc_my_name;
               gipc.data = 0;(void*)iconstop;
               GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_RECV_ICON,&gipc);
               
/*
               LockSched();
               ShowMSG(1,(int)"!IPC_SEND_ICONS>>");
               UnlockSched();*/
             
           /*    extern ICONS *xiconstop;
               gipc.name_to = IPC_NATICQ_NAME;
               gipc.name_from = ipc_my_name;
               gipc.data = (void*)xiconstop;
               GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_RECV_XICON,&gipc);
               
          
             
                 extern DYNPNGICONLIST *SmilesImgList;
             gipc.name_to = IPC_NATICQ_NAME;
             gipc.name_from = ipc_my_name;
             gipc.data = (void*)SmilesImgList;
             GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_RECV_SMILE,&gipc);       
        
         
                     extern S_SMILES *s_top;
             gipc.name_to = IPC_NATICQ_NAME;
             gipc.name_from = ipc_my_name;
             gipc.data = (void*)s_top;
             GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_RECV_S_TOP,&gipc);   */ 
            /*     LockSched();
              ShowMSG(1,(int)"!IPC_SEND_S_TOP");
             UnlockSched();             */             
             }
            break;             
           case IPC_SEND_XICON:
             {
               extern ICONS *xiconstop;
               gipc.name_to = ipc_send_name;
               gipc.name_from = ipc_my_name;
               gipc.data = (void*)xiconstop;
               GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_RECV_XICON,&gipc);
              /* 
             LockSched();
              ShowMSG(1,(int)"!IPC_SEND_XICON");
             UnlockSched();*/
             }
             break;             
          case IPC_SEND_SMILE:
            {
             extern DYNPNGICONLIST *SmilesImgList;
             gipc.name_to = ipc_send_name;
             gipc.name_from = ipc_my_name;
             gipc.data = (void*)SmilesImgList;
             GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_RECV_SMILE,&gipc);       
        /*
             LockSched();
              ShowMSG(1,(int)"!IPC_SEND_SMILE");
             UnlockSched();*/
            }
          break;
         case IPC_SEND_S_TOP:
            {
             extern S_SMILES *s_top;
             gipc.name_to = ipc_send_name;
             gipc.name_from = ipc_my_name;
             gipc.data = (void*)s_top;
             GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_RECV_S_TOP,&gipc);    
             /*    LockSched();
              ShowMSG(1,(int)"!IPC_SEND_S_TOP");
             UnlockSched();*/
            }
            break;
         case IPC_CHECK_DOUBLERUN:
	    //Если приняли свое собственное сообщение, значит запускаем чекер
	    if (ipc->name_from==ipc_my_name)
            {
                SUBPROC((void *)CheckDoubleRun);
            }
            else ipc->data=(void *)maincsm_id;
	    break;
	  case IPC_SMILE_PROCESSED:
	    //Только собственные смайлы ;)
	    if (ipc->name_from==ipc_my_name)
            {
              SUBPROC((void *)ProcessNextSmile);
            }
	    break;
	  case IPC_XSTATUSIMG_PROCESSED:
	    //Только собственные иксстатусы ;)
	    if (ipc->name_from==ipc_my_name)
            {
               SUBPROC((void *)ProcessNextXStatImg);
            }
	    break;
          }
        }
      }
    }
  }
  
  /*
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      InitConfig();
    }
  }*/
  return (1);  
}

static void maincsm_oncreate(CSM_RAM *data)
{
  gipc.name_to=ipc_my_name;
  gipc.name_from=ipc_my_name;
  gipc.data=(void *)-1;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_CHECK_DOUBLERUN,&gipc);
}

static void Killer(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

static void maincsm_onclose(CSM_RAM *csm)
{
  extern void FreeIconList();
  FreeIconList();
  extern void FreeXStatusesImg(void);
  FreeXStatusesImg();
  extern void FreeSmiles(void);
  FreeSmiles();
  
  SUBPROC((void *)Killer);
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
#ifdef NEWSGOLD
  0,
  0,
  0,
  0,
#endif
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

static void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"CheckNaticqPic");
}

int main()
{
  InitConfig();
  CSM_RAM *save_cmpc;
  char dummy[sizeof(MAIN_CSM)];
  UpdateCSMname();  
  LockSched();
  save_cmpc=CSM_root()->csm_q->current_msg_processing_csm;
  CSM_root()->csm_q->current_msg_processing_csm=CSM_root()->csm_q->csm.first;
  maincsm_id = CreateCSM(&MAINCSM.maincsm,dummy,0);
  CSM_root()->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();

  return 0;
}
