#include "..\inc\swilib.h"
#include "../inc/cfg_items.h"
#include "conf_loader.h"
#include "local_ipc.h"
/*
const char ipc_my_name[]=IPC_TEXTINFO_NAME;
const IPC_REQ gipc={
  ipc_my_name,
  ipc_my_name,
  NULL
};
*/
//IPC
//IPC constants for XTask interface
#define IPC_XTASK_NAME "XTask"
#define IPC_XTASK_IDLE 1
#define IPC_XTASK_SHOW_CSM 2
#define IPC_XTASK_LOCK_SHOW 3
#define IPC_XTASK_UNLOCK_SHOW 4
const char ipc_my_name[32]=IPC_TEXTINFO_NAME;
const char ipc_xtask_name[]=IPC_XTASK_NAME;
IPC_REQ gipc;

#define TMR_SECOND 216
GBSTMR mytmr;
extern const char name_1[256];
extern const char name_2[256];
extern const char name_3[256];
extern const char name_4[256];
extern const char name_5[256];

extern const int ACTIVE_KEY_1;
extern const int ACTIVE_KEY_2;
extern const int ACTIVE_KEY_3;
extern const int ACTIVE_KEY_4;
extern const int ACTIVE_KEY_5;

extern const int ACTIVE_KEY_STYLE;
extern const int ACTIVE_KEY_STYLE_2;
extern const int ACTIVE_KEY_STYLE_3;
extern const int ACTIVE_KEY_STYLE_4;
extern const int ACTIVE_KEY_STYLE_5;

extern const int ENA_LOCK;

extern const int IDLE_1;
extern const int IDLE_2;
extern const int IDLE_3;
extern const int IDLE_4;
extern const int IDLE_5;

extern void InitConfig();
int my_csm_id;
unsigned int *ErrorNumber;

const char percent_t[]="%t";
int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
void (*old_icsm_onClose)(CSM_RAM*);
extern void kill_data(void *p, void (*func_p)(void *));

#pragma segment="ELFBEGIN"
void ElfKiller(void)
{
  kill_data(__segment_begin("ELFBEGIN"),(void (*)(void *))mfree_adr());
}

void TimerProc(void)
{
  
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
}

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;   

void Hide()
{
  gipc.name_to=ipc_xtask_name;
  gipc.name_from=ipc_my_name;
  gipc.data=0;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_IDLE,&gipc);
}

void HideAndLock()
{
  gipc.name_to=ipc_xtask_name;
  gipc.name_from=ipc_my_name;
  gipc.data=0;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_IDLE,&gipc);
  if(IsUnlocked())
   {
     KbdLock();
   }
}
#pragma inline=forced
int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

int strncmpNoCase(const char *s1,const char *s2,unsigned int n)
{
	int i;
	int c;
	while(!(i=(c=toupper(*s1++))-toupper(*s2++))&&(--n)) if (!c) break;
	return(i);
}


#define FUNC_PT_START	0xA085DEA4
#define FUNC_PT_END	0xA0865BA3
#define FUNC_PT_PSIZE	0x80
#define FUNC_PT_OFFSET	0x10
#define MAX_FUNC	0xFA

#ifdef NEWSGOLD
unsigned int GetFunctionPointByName(char *name)
{
	char *p=(char *)FUNC_PT_START;
	int len=strlen(name);
	if(len==0)
		return 0;
	while((char *)FUNC_PT_END-p>0)
	{
		if(!strncmpNoCase(p, name, len))
			return (*(unsigned int *)(p+FUNC_PT_OFFSET));
		p+=FUNC_PT_PSIZE;
	}
	return 0;
}
#endif


void run_address(unsigned int address) 
{
  void (*runaddr)(void);
  runaddr=(void (*)())address;
  if (runaddr) 
  {
    runaddr();
  }
}

void run_shortcut(char *shortcut) 
{
#ifdef NEWSGOLD
   run_address(GetFunctionPointByName(shortcut));
#else
   run_address((unsigned int)GetFunctionPointer(shortcut));
#endif
}

int chr2num(char chr)
{
	if(chr>='0'&&chr<='9')
		return (chr-('0'-0));
	if(chr>='A'&&chr<='Z')
		//return (chr-0x37);
		return (chr-('A'-0xA));
	if(chr>='a'&&chr<='z')
		//return (chr-0x57);
		return (chr-('a'-0xA));
	return 0;
}

int start(const char s[128])
{
  if((s)&&strlen(s))
   {
    if(!strncmp(s,"CMD_HIDE_AND_LOCK",strlen("CMD_HIDE_AND_LOCK")))
    {
      HideAndLock();
      return 1;    
    }
     
    if(!strncmp(s,"CMD_HIDE",strlen("CMD_HIDE")))
    {
      Hide();
      return 1;    
    }
    
    if((s[2]=='\\')&&((s[(strlen(s))-3]=='.')||//Проверяем строку на наличие символов '\\' и '.'
       (s[(strlen(s))-4]=='.')||(s[(strlen(s))-5]=='.')))
       /* Если в строке есть символы '\\' и '.', то запускаем как обычный файл*/
        {
          WSHDR *ws;
          ws=AllocWS(256);
          str_2ws(ws,s,256);
          ExecuteFile(ws,0,0);
          FreeWS(ws);
          return(1);
         }
    if ((s[2]!='\\')&&(s[(strlen(s))-3]!='.')&&
       (s[(strlen(s))-4]!='.')&&(s[(strlen(s))-5]!='.')&&
       (s[0]!='a')&&(s[0]!='A')&&(s[1]!='0'))
      /* Если в строке нет символов '\\' и '.' и первые два символа не А0,
         то запускаем шорткат */
        {
          /*typedef void (*voidfunc)(); 
          voidfunc pp=(voidfunc)GetFunctionPointer((char*)s); 
          if(pp!=0) SUBPROC((void*)pp);*/
          char ShortCut[32];
	char *p=(char*)s;
	int i=0;
	int j=0;
	for(;j<strlen(s);j++)
	{
          if(p[j]>='A')
	  {
	    ShortCut[i]=p[j];
	    i++;
	  }
        }
	ShortCut[i]=0;
	run_shortcut(ShortCut);
  /*      int (*p) (void);
         p=(int(*)(void))GetFunctionPointer((char*)s);
         if(p!=NULL)
          (*p)();
         else
           ShowMSG(1,(int)"NULL pointer function!");
        return 1;
*/
  /*        unsigned int* addr = (unsigned int*)GetFunctionPointer((char*)s); 
          if (addr)
          {
            typedef void (*voidfunc)(); 
            #ifdef NEWSGOLD 
              voidfunc pp=(voidfunc)*(addr+4);
            #else 
              voidfunc pp=(voidfunc)addr; 
            #endif 
              SUBPROC((void*)pp);
          }*/
        }
    if ((s[2]!='\\')&&(s[(strlen(s))-3]!='.')&&
       (s[(strlen(s))-4]!='.')&&(s[(strlen(s))-5]!='.')&&
       ((s[0]=='a')||(s[0]=='A'))&&(s[1]=='0'))
      /* Если в строке нет символов '\\' и '.' и первые два символа А0,
         то запускаем энтрипоинт */
        {/*
          int entry;
          sscanf(s,"%08X",&entry);
          SUBPROC((void*)entry);*/
           int i;
	unsigned int addr=0;
	char *p=(char*)s;
	for(i=0;i<strlen(s);i++)
	{
           addr=chr2num(p[i])+addr*0x10;
	}
	run_address(addr);        
        }
    }
    
  return 1;
}

int mode;

int IsIdle(void)
{ 
  void *icsm=FindCSMbyID(CSM_root()->idle_id); 
  if (IsGuiOnTop(((int*)icsm)[DISPLACE_OF_IDLEGUI_ID/4 ])) 
    return(1); 
  else
  return(0); 
} 

//----------------Keyhook----------------//

int mode_enter;
int my_keyhook(int submsg, int msg)
{
 //---------------------------- 1 ---------------------------//

  if (submsg==ACTIVE_KEY_1 && strlen(name_1))
    {
      switch(msg)
      {
        case KEY_DOWN:
          if (!ACTIVE_KEY_STYLE)
          {
            if(IDLE_1)
            {
              if(IsIdle())
              {
                 if (IsUnlocked()||ENA_LOCK)
                  {
                    start(name_1);
                  }
              }
              else
              {
                GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_1);break;
              }
            }
            else
             if (IsUnlocked()||ENA_LOCK)
             {
              start(name_1);
             }
            return(2);
          }
          else
          {
            if (mode_enter==2)
            {
              GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_1);
              return (0);
            }
            mode_enter=0;
            return (2);
          }          
        case KEY_UP:
          if (ACTIVE_KEY_STYLE)
          {
            if (mode_enter==0)
            {
              mode_enter=2;
              GBS_SendMessage(MMI_CEPID,KEY_DOWN,ACTIVE_KEY_1);
              return (2);
            }
            if (mode_enter==2)
            {
              mode_enter=0;
              return (0);
            }
            mode_enter=0;
            return (2);
          }          
        case LONG_PRESS:
          if (ACTIVE_KEY_STYLE)
          {
            mode_enter=1;
            if(IDLE_1)
             {
              if(IsIdle())
              {
                if (IsUnlocked()||ENA_LOCK)
                {
                  start(name_1);
                }
              }
              else  GBS_SendMessage(MMI_CEPID,msg,ACTIVE_KEY_1);
            }
            else
             if (IsUnlocked()||ENA_LOCK)
             {
              start(name_1);
             }
          }
         break;
      }
    }
  
 //---------------------------- 2 ---------------------------//

  if (submsg==ACTIVE_KEY_2  && strlen(name_2))
    {
      switch(msg)
      {
        case KEY_DOWN:
          if (!ACTIVE_KEY_STYLE_2)
          {
           if(IDLE_2)
            {
              if(IsIdle())
              {
               if (IsUnlocked()||ENA_LOCK)
                {
                  start(name_2);
                }
              }
              else
              {
                GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_2);break;
              }
            }
            else 
             if (IsUnlocked()||ENA_LOCK)
              {
                start(name_2);
              }
            return(2);
          }
          else
          {
            if (mode_enter==2)
            {
              GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_2);
              return (0);
            }
            mode_enter=0;
            return (2);
          }
          
        case KEY_UP:
          if (ACTIVE_KEY_STYLE_2)
          {
            if (mode_enter==0)
            {
              mode_enter=2;
              GBS_SendMessage(MMI_CEPID,KEY_DOWN,ACTIVE_KEY_2);
              return (2);
            }
            if (mode_enter==2)
            {
              mode_enter=0;
              return (0);
            }
            mode_enter=0;
            return (2);
          }
          
        case LONG_PRESS:
          if (ACTIVE_KEY_STYLE_2)
          {
            mode_enter=1;
            if(IDLE_1)
             {
               if(IsIdle())
               {
                if (IsUnlocked()||ENA_LOCK)
                 {
                   start(name_2);
                 }
               }
                else
              {
                GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_2);
              }
             }
            else 
              if (IsUnlocked()||ENA_LOCK)
              {
                start(name_2);
              }
          }
         break;
      }
    }
  
  //---------------------------- 3 ---------------------------//

  if (submsg==ACTIVE_KEY_3 && strlen(name_3))
    {
      switch(msg)
      {
        case KEY_DOWN:
          if (!ACTIVE_KEY_STYLE_3)
          {
          if(IDLE_3)
            {
              if(IsIdle())
              {
               if (IsUnlocked()||ENA_LOCK)
                {
                 start(name_3);
                }
              }
              else
              {
                GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_3);break;
              }
            }
            else if (IsUnlocked()||ENA_LOCK)
                {
                  start(name_3);
                }
            return(2);
          }
          else
          {
            if (mode_enter==2)
            {
              GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_3);
              return (0);
            }
            mode_enter=0;
            return (2);
          }
          
        case KEY_UP:
          if (ACTIVE_KEY_STYLE_3)
          {
            if (mode_enter==0)
            {
              mode_enter=2;
              GBS_SendMessage(MMI_CEPID,KEY_DOWN,ACTIVE_KEY_3);
              return (2);
            }
            if (mode_enter==2)
            {
              mode_enter=0;
              return (0);
            }
            mode_enter=0;
            return (2);
          }
          
        case LONG_PRESS:
          if (ACTIVE_KEY_STYLE_3)
          {
            mode_enter=1;
            if(IDLE_1)
            {
              if(IsIdle())
              {
               if (IsUnlocked()||ENA_LOCK)
                {
                  start(name_3);
                }
              }
              else
              {
                GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_3);
              }
            }
            else
              if (IsUnlocked()||ENA_LOCK)
                {
                  start(name_3);
                }
          }
         break;
      }
    }
  
  //---------------------------- 4 ---------------------------//

  if (submsg==ACTIVE_KEY_4  && strlen(name_4))
    {
      switch(msg)
      {
        case KEY_DOWN:
          if (!ACTIVE_KEY_STYLE_4)
          {
           if(IDLE_4)
            {
              if(IsIdle())
              {
                if (IsUnlocked()||ENA_LOCK)
                {
                  start(name_4);
                }
              }
              else
              {
                GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_4);break;
              }
            }
            else if (IsUnlocked()||ENA_LOCK)
                {
                  start(name_4);
                }
            return(2);
          }
          else
          {
            if (mode_enter==2)
            {
              GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_4);
              return (0);
            }
            mode_enter=0;
            return (2);
          }
          
        case KEY_UP:
          if (ACTIVE_KEY_STYLE_4)
          {
            if (mode_enter==0)
            {
              mode_enter=2;
              GBS_SendMessage(MMI_CEPID,KEY_DOWN,ACTIVE_KEY_4);
              return (2);
            }
            if (mode_enter==2)
            {
              mode_enter=0;
              return (0);
            }
            mode_enter=0;
            return (2);
          }
          
        case LONG_PRESS:
          if (ACTIVE_KEY_STYLE_4)
          {
            mode_enter=1;
            if(IDLE_4)
            {
              if(IsIdle())
              {
                if (IsUnlocked()||ENA_LOCK)
                {
                  start(name_4);
                }
              }
              else
              {
                GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_4);
              }
            }
            else if (IsUnlocked()||ENA_LOCK)
                {
                  start(name_4);
                }
          }
         break;
      }
    }
  
 //---------------------------- 5 ---------------------------//

  if (submsg==ACTIVE_KEY_5  && strlen(name_5))
    {
      switch(msg)
      {
        case KEY_DOWN:
          if (!ACTIVE_KEY_STYLE_5)
          {
            if(IDLE_5)
            {
              if(IsIdle())
              {
                if (IsUnlocked()||ENA_LOCK)
                {
                  start(name_5);
                }
              }
              else
              {
                GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_5);break;
              }
            }
            else if (IsUnlocked()||ENA_LOCK)
                {
                  start(name_5);
                }
            return(2);
          }
          else
          {
            if (mode_enter==2)
            {
              GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_5);
              return (0);
            }
            mode_enter=0;
            return (2);
          }
          
        case KEY_UP:
          if (ACTIVE_KEY_STYLE_5)
          {
            if (mode_enter==0)
            {
              mode_enter=2;
              GBS_SendMessage(MMI_CEPID,KEY_DOWN,ACTIVE_KEY_5);
              return (2);
            }
            if (mode_enter==2)
            {
              mode_enter=0;
              return (0);
            }
            mode_enter=0;
            return (2);
          }
          
        case LONG_PRESS:
          if (ACTIVE_KEY_STYLE_5)
          {
            mode_enter=1;
         if(IDLE_1)
            {
              if(IsIdle())
              {
                if (IsUnlocked()||ENA_LOCK)
                {
                  start(name_5);
                }
              }
              else
              {
                GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_5);
              }
            }
            else if (IsUnlocked()||ENA_LOCK)
                {
                  start(name_5);
                }
          }
         break;
      }
    }
  return(0);
}
// ----------------------------------------------------------------------------
#define idlegui_id(icsm) (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])


#pragma inline
int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}

int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
  void *icsm;
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
     InitConfig();
      ShowMSG(1,(int)"ElfStart config updated!");
    }
  }
  if (msg->msg==MSG_IPC)
  {
    IPC_REQ *ipc;
    if ((ipc=(IPC_REQ*)msg->data0))
    {
      if (strcmp_nocase(ipc->name_to,ipc_my_name)==0)
      {
        switch (msg->submess)
        {
        case IPC_UPDATE_STAT:
          GBS_StartTimerProc(&mytmr, 10*TMR_SECOND/10, TimerProc);
        }
      }
    }
  }
  icsm=FindCSMbyID(CSM_root()->idle_id);
  if (icsm)
  {
    if (IsGuiOnTop(idlegui_id(icsm))) //Если IdleGui на самом верху
    {
      GUI *igui=GetTopGUI();
      if (igui) //И он существует
      {
      }
    }
  }
  return(1);

}
static void maincsm_oncreate(CSM_RAM *data)
{
 
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
}

void maincsm_onclose(CSM_RAM *data)
{ 
  RemoveKeybMsgHook((void *)my_keyhook);
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
  }
static unsigned short maincsm_name_body[140];
const int minus11=-11;

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
  sizeof(CSM_RAM),
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"ElfStart");
}
// ----------------------------------------------------------------------------


CSM_DESC icsmd;
int main()
{
  CSMROOT *csmr;
  CSM_RAM *save_cmpc;
  CSM_RAM main_csm;
  InitConfig();
  UpdateCSMname();
  LockSched();
  AddKeybMsgHook((void *)my_keyhook);
  csmr=CSM_root();
  save_cmpc=csmr->csm_q->current_msg_processing_csm;
  csmr->csm_q->current_msg_processing_csm=csmr->csm_q->csm.first;
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  csmr->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();
//  ShowMSG(1,(int)"Elfstart launch");
  return (0);
}
