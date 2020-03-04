//JKeyFinder - Утилита для поиска кода клавиш от другого телефона для Java - приложений.
//Смотрите как работает в readme.txt
//(c)Dimadze

//Если не компилится (Нет слова "Linking"), Используйте "Clean"

#include "..\..\inc\swilib.h"
#include "..\..\inc\xtask_ipc.h"

#define ELF_NAME "JKeyFinder"
#define ELF_AUTHOR "Dimadze"

int action_1=0;
int action_2=0;
int action_3=0;
int action_4=0;
int action_5=0;
int action_6=0;

int action2=0;

int key_n;
int key_f1=0;
int key_f2=0;
char message[128];
char message2[128];

char elfhello[64];

int my_keyhook(int submsg, int msg);

int IsIDLE(void)
{
  CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
  if (IsGuiOnTop(((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])) return(1);
                                                           else return(0);
}

const int minus11=-11;

typedef struct
{
  CSM_RAM csm;
}MAIN_CSM;

CSM_RAM *under_idle;

const char ipc_my_name[32]=ELF_NAME; 
const char ipc_xtask_name[]=IPC_XTASK_NAME; 
IPC_REQ gipc;

void Idle(void)
{
   gipc.name_to=ipc_xtask_name; 
   gipc.name_from=ipc_my_name; 
   gipc.data=0; 
   GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_IDLE,&gipc);
}

extern void kill_data(void *p, void (*func_p)(void *));


int my_keyhook(int submsg, int msg)
{
void *icsm=FindCSMbyID(CSM_root()->idle_id);
 
    if ((submsg=='*') && (msg==LONG_PRESS) && action2==0 && action_1==0 && action_2==0 && action_3==0 && action_4==0 && action_5==0) // 1
    {   
        ShowMSG(1,(int)"Давай\nнажимай!");
        action_1=1;
	return(2); 
    }
                                        
    if (msg==KEY_DOWN && action2==0 && action_1==1 && action_2==0 && action_3==0 && action_4==0 && action_5==0)
    {  
        ShowMSG(1,(int)"Вводи!");
        key_n=submsg;
        action_1=0;
        action_2=1;
	return(2); 
    }
    
    if (msg==KEY_DOWN && action2==0 && action_1==0 && action_2==1 && action_3==0 && action_4==0 && action_5==0)
    {   
      if (submsg=='0') {key_f1=0;action_3=1;action_2=0;return(2);}
      if (submsg=='1') {key_f1=1*10;action_3=1;action_2=0;return(2);}
      if (submsg=='2') {key_f1=2*10;action_3=1;action_2=0;return(2);}
      if (submsg=='3') {key_f1=3*10;action_3=1;action_2=0;return(2);}
      if (submsg=='4') {key_f1=4*10;action_3=1;action_2=0;return(2);}
      if (submsg=='5') {key_f1=5*10;action_3=1;action_2=0;return(2);}
      if (submsg=='6') {key_f1=6*10;action_3=1;action_2=0;return(2);}
      if (submsg=='7') {key_f1=7*10;action_3=1;action_2=0;return(2);}
      if (submsg=='8') {key_f1=8*10;action_3=1;action_2=0;return(2);}
      if (submsg=='9') {key_f1=9*10;action_3=1;action_2=0;return(2);}
      
    }
    if (msg==KEY_DOWN && action2==0 && action_1==0 && action_2==0 && action_3==1 && action_4==0 && action_5==0)
    {   
      if (submsg=='0') {key_f2=key_f1;action_4=1;action_3=0;return(2);}
      if (submsg=='1') {key_f2=key_f1+1;action_4=1;action_3=0;return(2);}
      if (submsg=='2') {key_f2=key_f1+2;action_4=1;action_3=0;return(2);}
      if (submsg=='3') {key_f2=key_f1+3;action_4=1;action_3=0;return(2);}
      if (submsg=='4') {key_f2=key_f1+4;action_4=1;action_3=0;return(2);}
      if (submsg=='5') {key_f2=key_f1+5;action_4=1;action_3=0;return(2);}
      if (submsg=='6') {key_f2=key_f1+6;action_4=1;action_3=0;return(2);}
      if (submsg=='7') {key_f2=key_f1+7;action_4=1;action_3=0;return(2);}
      if (submsg=='8') {key_f2=key_f1+8;action_4=1;action_3=0;return(2);}
      if (submsg=='9') {key_f2=key_f1+9;action_4=1;action_3=0;return(2);}
    }
    
    if ((submsg=='#') && (msg==LONG_PRESS) && action2==0 && action_1==0 && action_2==0 && action_3==0 && action_4==1 && action_5==0)
    {   
      if (key_n!=key_f2){
        sprintf(message,"%s: %02d\n%s: %02d","Реал. кн",key_n,"Эмул. кн",key_f2);
        ShowMSG(1,(int)message);
        action_6=1;
        action_5=1;
        action_4=0;    
      } else 
      {
        ShowMSG(1,(int)"Ошибка!\nДавай\nснова!");
        action_1=1;
        action_4=0; 
        key_n=0;
        key_f1=0;
        key_f2=0;
      }
	return(2); 
    }
    
   if ((submsg=='#') && (msg==LONG_PRESS) && action2==0 && action_6==1)
    {   
        ShowMSG(1,(int)"Вводи!");
        action_6=0;
        action_5=0;
        action_2=1;
        key_f1=0;
        key_f2=0;
	return(2); 
    }
    
    if ((submsg=='*') && (msg==LONG_PRESS) && action2==0 && action_1==0 && action_2==0 && action_3==0 && action_4==0 && action_5==1)
    {   
        ShowMSG(1,(int)"Сброс!");
        action_5=0;
        action_6=0;
        key_n=0;
        key_f1=0;
        key_f2=0;
	return(2); 
    }
    
    if ((submsg==key_n) && action2==0 && action_1==0 && action_2==0 && action_3==0 && action_4==0 && action_5==1)
    {   
	GBS_SendMessage(MMI_CEPID,msg,key_f2);
	return(2); 
    }
    
    if ((submsg=='#') && (msg==LONG_PRESS) && action2==0 && action_1==0 && action_2==0 && action_3==0 && action_4==0 && action_5==0)
    {   
        ShowMSG(1,(int)"4 - --\n5 - Жми!\n6 - ++");
        action2=1;
	return(2); 
    }
    
    if (msg==KEY_DOWN && action2==1)
    {   
      sprintf(message2,"%s: %02d","Текущ. код",key_f2);
      if (submsg=='6') {if (key_f2<99) key_f2++;return(2);}
      if (submsg=='4') {if (key_f2>0) key_f2--;return(2);}
      if (submsg=='5') {GBS_SendMessage(MMI_CEPID,msg,key_f2);return(2);}
      if (submsg=='0') {ShowMSG(1,(int)message2);return(2);}  
      if (submsg=='#') {ShowMSG(1,(int)"Сброс!");key_f2=0;action2=0;return(2);}  
    }            
      
return(0);

}

#pragma inline=forced

int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}

int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
  
 void *icsm=FindCSMbyID(CSM_root()->idle_id);

  return (1);  
}

static void maincsm_oncreate(CSM_RAM *data)
{

}

static void Killer(void)
{
  extern void *ELF_BEGIN;
  RemoveKeybMsgHook((void *)my_keyhook);
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

static void maincsm_onclose(CSM_RAM *csm)
{
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),ELF_NAME);
}


int main()
{
  CSM_RAM *save_cmpc;
  char dummy[sizeof(MAIN_CSM)];
  UpdateCSMname();
  
  LockSched();
  save_cmpc=CSM_root()->csm_q->current_msg_processing_csm;
  CSM_root()->csm_q->current_msg_processing_csm=CSM_root()->csm_q->csm.first;
  CreateCSM(&MAINCSM.maincsm,dummy,0);
  CSM_root()->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();
  AddKeybMsgHook((void *)my_keyhook);
  sprintf(elfhello,"%s\n(c)%s",ELF_NAME,ELF_AUTHOR);
  ShowMSG(1,(int)elfhello);
  
  return 0;
}
