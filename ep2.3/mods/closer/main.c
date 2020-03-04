#include "..\inc\swilib.h"
#include "..\inc\xtask_ipc.h"
#include "..\inc\cfg_items.h"
#include "conf_loader.h"

unsigned int csmid=0;
void *csmadr=NULL;
extern const char CSMADR[9];//A068A8D4
extern unsigned long  strtoul (const char *nptr,char **endptr,int base);
extern const char *successed_config_filename;
extern int InitConfig();
int FindIdByCSM()
{
 csmadr=(void *)strtoul(CSMADR,NULL,16);
 CSM_RAM *p=CSM_root()->csm_q->csm.first;
 while(p)
  {
    if (p->constr==csmadr) break;
    p=p->next;
  }
 if (p)
  {
    csmid=p->id;
    CloseCSM(csmid);
//    return 1;
  }
  return 0;
}

/*
extern const char UNDER_IDLE_CONSTR[];
CSM_RAM *under_idle;
CSM_RAM *GetUnderIdleCSM(void)
{
  CSM_RAM *u;
  CSM_DESC *UnderIdleDesc;
  if (strlen((char *)UNDER_IDLE_CONSTR)==8)
  {
    UnderIdleDesc=(CSM_DESC *)strtoul((char *)UNDER_IDLE_CONSTR,0,0x10);
  }
  else
  {
    UnderIdleDesc=((CSM_RAM *)(FindCSMbyID(CSM_root()->idle_id))->prev)->constr;
    sprintf((char *)UNDER_IDLE_CONSTR,"%08X",UnderIdleDesc);
    SaveConfigData(successed_config_filename);
  }
  LockSched();
  u=CSM_root()->csm_q->csm.first;
  while(u && u->constr!=UnderIdleDesc) u=u->next;
  UnlockSched();
  return u;
}

static int do_CSMtoTop(CSMQ *csm_q, void *_cmd)
{
//  static const char my_color[]={0x00,0x00,0x00,0x32};
  MMICMD *cmd=_cmd;
  int *gui;
  CSM_RAM *wcsm;
  CSM_RAM *top_csm=FindCSM(csm_q,cmd->parent_id);
  CSM_RAM *bot_csm=under_idle;
  if (!bot_csm) return 1;
#ifdef NEWSGOLD
  CSM_RAM *work_csm=FindCSM(csm_q,cmd->id);
#else
  CSM_RAM *work_csm=FindCSM(csm_q,(int)(cmd->cmd_csm));
#endif
  if (!work_csm) return 1;
  if (work_csm==bot_csm) return 1;
  if (work_csm==top_csm) return 1;
  if (work_csm==(top_csm?top_csm->prev:csm_q->csm.last)) return 1;
  if (!top_csm) UnfocusGUI();
//
  do
  {
    if (top_csm)
    {
      wcsm=(CSM_RAM *)top_csm->prev; //ѕолучаем перемещаемый CSM
    }
    else
    {
      wcsm=csm_q->csm.last;
    }
    ((CSM_RAM *)(wcsm->prev))->next=top_csm; //CSM перед перемещаемым теперь указывает на верхний CSM
    if (top_csm)
    {
      top_csm->prev=wcsm->prev; //prev верхнего CSM указывает на CSM перед перемещаемым
    }
    else
    {
      csm_q->csm.last=wcsm->prev;
    }
    //“еперь вставл€ем в нужное место перемещаемый CSM
    ((CSM_RAM *)(wcsm->next=bot_csm->next))->prev=wcsm;
    bot_csm->next=wcsm;
    wcsm->prev=bot_csm;
  }
  while((top_csm?top_csm->prev:csm_q->csm.last)!=work_csm);
//  
  if (!top_csm)
  {
    if ((gui=((CSM_RAM *)(csm_q->csm.last))->gui_ll.last))
    {
      FocusGUI(gui[3]);
    }
    //—ообщение об уничтожении несуществующего CSM, необходимо дл€ правильной работы IdleCSM
    GBS_SendMessage(MMI_CEPID,MSG_CSM_DESTROYED,0,30002,0);
  }
  return 1;
}

#pragma optimize=no_inline
__thumb static void LLaddToEnd(LLQ *ll, void *data)
{
  LLIST *d=data;
  d->next=NULL;
  d->prev=ll->last;
  if (ll->last)
  {
    ((LLIST *)ll->last)->next=d;
  }
  else
  {
    ll->first=d;
  }
  ll->last=d;
}

//Move CSM with "id" on top
//Top is under CSM with "top_id" - may be -1
void CSMtoTop(int id, int top_id)
{
  CSMQ *csm_q=CSM_root()->csm_q;
  MMICMD *cmd;
  if (!FindCSMbyID(id)) return;
  if (id==top_id) return; //Ќечего
  cmd=malloc(sizeof(MMICMD));
  cmd->csm_q=csm_q;

  cmd->flag1=5;
#ifdef NEWSGOLD
  cmd->prio=0;
  cmd->id=id;
#else
  cmd->cmd_csm=(CSM_RAM*)id;
#endif
  cmd->parent_id=top_id;
  cmd->proc=do_CSMtoTop;
  LLaddToEnd(&csm_q->cmd,cmd);
}
*/
void main(void)
{
  InitConfig();
  if(strlen(CSMADR)!=0)//UNDER_IDLE_CONSTR
  {
    FindIdByCSM();
//    under_idle=GetUnderIdleCSM(); //»щем idle_dialog
  //  CSMtoTop(CSM_root()->idle_id,-1);
  }
  else
  {
    WSHDR *ws=AllocWS(150);
    str_2ws(ws,successed_config_filename,128);
    ExecuteFile(ws,0,0);
    FreeWS(ws);    
    ShowMSG(1,(int)"Input CSM Adress!!!");
  }
}
