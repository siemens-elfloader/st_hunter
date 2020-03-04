#include "..\inc\swilib.h"
#include "../inc/cfg_items.h"
#include "conf_loader.h"

#define UPDATE_TIME (1*262)
#define ELF_ID 0x1EF

char name[256];
CSM_DESC icsmd;
extern const int ACTIVE_KEY_STYLE;
extern void InitConfig();

int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);

GBSTMR mytmr;

const char per_t[]="%t";
// ----------------------------------------------------------------------------
int start()
{
      WSHDR *ws;
      ws=AllocWS(256);
      str_2ws(ws,name,128);
      ExecuteFile(ws,0,0);
      FreeWS(ws);
      return(1);
    
  return(0);
}

//====================================
//        Вызов эльфа
//====================================
/*
int my_keyhook(int submsg, int msg)
{
  int scrH=ScreenH();
  int scrW=ScreenW();
  if(IsUnlocked() && submsg==BTN)
  {
    if(mode==1)
    {
      if(msg==KEY_UP)
      {
        GBS_SendMessage(MMI_CEPID,KEY_DOWN,BTN);
        return(0);
      }
      if(msg==KEY_DOWN)
      {
        mode=0;
        GBS_SendMessage(MMI_CEPID,KEY_UP,BTN);
        return(0);
      }
    }

    if(msg==KEY_UP)
    { if(mode==2)
      {
        mode=0;
        return(2);
      }
    }
    else if(msg==(LONG_P?LONG_PRESS:KEY_DOWN) )//длинное нажатие или нет
    { //Вызов закладок
      start();
    }

   
   if(!IsUnlocked()) return(2);

    if(LONG_P)
    {
      if(msg==LONG_PRESS)
      {
        mode=2;
        return(2);
      }
      if(msg==KEY_DOWN)
      {
        mode=1;
        return(2);
      }
    }
    else  return(2);
  } 
  return(0);

}*/

int my_keyhook(int submsg, int msg)
{ if (ACTIVE_KEY_STYLE-2<0){
    if (submsg!=ACTIVE_KEY) return(0);
    else{
       if (my_csm_id)
        {
         if (((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=my_csm_id)
         {
           CloseCSM(my_csm_id);
         }
         if (msg==KEY_UP)
         {
           GBS_SendMessage(MMI_CEPID,KEY_DOWN,ENTER_BUTTON);
         }
         return(2);
       }
     switch(msg){
     case KEY_DOWN:
       mode=0;
       break;
     case KEY_UP:
       if ((mode==1)&&(ACTIVE_KEY_STYLE==1))
         {
           if (IsUnlocked()||ENA_LOCK){
           do_gui(0,0);
           return(2);
         }
         break;
         }
       if ((mode==0)&&(ACTIVE_KEY_STYLE==0))
       {
         if (IsUnlocked()||ENA_LOCK){
         do_gui(0,0);
         return(2);
         }
         }
         break;
     case LONG_PRESS:
         mode=1;
         if (ACTIVE_KEY_STYLE==1) return(2); // Что бы длинное нажатие работало при активном режиме - на короткое нажатие кнопки
     }
  }
  }
  return(0);
}


// ----------------------------------------------------------------------------

int main(void)
{ InitConfig();
  LockSched();
  CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
  memcpy(&icsmd,icsm->constr,sizeof(icsmd));
  old_icsm_onClose=icsmd.onClose;
  old_icsm_onMessage=icsmd.onMessage;
  icsmd.onClose=MyIDLECSM_onClose;
  icsmd.onMessage=MyIDLECSM_onMessage;
  icsm->constr=&icsmd;
  AddKeybMsgHook((void *)my_keyhook);
  UnlockSched();
  return 0;
}
