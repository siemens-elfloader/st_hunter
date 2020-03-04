#include "..\inc\swilib.h"
#include "..\inc\cfg_items.h"
#include "main.h"
#include "rect_patcher.h"

const int minus11=-11;

const char _percent_u[]="%u";
const char _percent_d[]="%d";
const char _percent_t[]="%t";

int csm_id = 0;


unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;

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

unsigned short maincsm_name_body[140];

int my_csm_id=0;
int cursize;
int isAbout;

#define _VERSION_ "v0.1.2"

typedef struct
{
  void *next;
  char text[128];
}LogLine;


char *buf;
char *buf2;
int size = 0;
int size2 = 0;
char _s1[]="%s:";

LogLine *toplog;

char name1[128] = "";
char name2[128] = "";
char dirpath[128];

char exedir[128];
char logdir[128];
char backupdir[128];
char curbcfg[128];

//****************************************************************************//

void ErrorMsg(const char *msg)
{
  LockSched();
  ShowMSG(1,(int)msg);
  UnlockSched();
}


void menup1(GUI *data)
{
  SelectFile(0);      
}

void menup2(GUI *data)
{
  SelectFile(1);  
}


const HEADER_DESC menuhdr={0,0,131,YDISP,NULL,(int)"ImportBCFG",LGP_NULL};
const int menusoftkeys[]={0,1,2};
static MENUITEM_DESC menuitems[2]=
{
  {NULL,(int)"Не выбрано",LGP_NULL,0,NULL/*menusoftkeys*/,MENU_FLAG3,MENU_FLAG2},
  {NULL,(int)"Не выбрано",LGP_NULL,0,NULL/*menusoftkeys*/,MENU_FLAG3,MENU_FLAG2},
};

const MENUPROCS_DESC menuprocs[2]={
  menup1,
  menup2
};

const SOFTKEY_DESC menu_sk[]=
{
  {0x0018,0x0000,(int)"Start!"},
  {0x0001,0x0000,(int)"Exit"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menu_skt=
{
  (SOFTKEY_DESC *)menu_sk,0
};


static int mmenu_keyhook(void *data, GUI_MSG *msg)
{
  int k=msg->gbsmsg->submess;
  int m=msg->gbsmsg->msg;
  if(m==KEY_DOWN)
  {
    if( k == LEFT_SOFT)
    {
      if(name1[0] == '-' || name2[0] == '-') { ErrorMsg("Select files!"); return 0; }     
     
       CreateLogScreen(data); 
       Parser(); 
     
    }
    if( k == RIGHT_SOFT) return 1;  
  }     
  return(0);
}


const MENU_DESC tmenu=
{
  8,mmenu_keyhook,NULL,NULL,
  menusoftkeys,
  &menu_skt,
  0x10,
  NULL,
  menuitems,
  menuprocs,
  2
};

int TestMenu(void)
{
  patch_header(&menuhdr);
  return CreateMenu(0,0,&tmenu,&menuhdr,0,2,0,0);
}


//****************************************************************************//


int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}


void AddToLog(char *text)
{
  LogLine* tmp = malloc(sizeof(LogLine));
  strncpy(tmp->text,text,128);  
  tmp->next=0;
  
  if(!toplog)
  {
    toplog = tmp;
  }
  else
  {
    LogLine* tlog = toplog;
    while(tlog->next) tlog = tlog->next;
    tlog->next = tmp;  
  }
}

void FreeLog()
{
  LogLine* tmp = toplog;
  toplog = 0;
  while(tmp)
  {
    LogLine* tline = tmp;
    tmp = tmp->next;
    mfree(tline);
  }
}


void SaveFile()
{
   if(!strlen(curbcfg) && !get_file_size(curbcfg)) return;
   unsigned int ul;
   unlink(curbcfg,&ul);
   char s[128];
   int f=fopen(curbcfg,A_ReadWrite+A_BIN+A_Create,P_READ+P_WRITE,&ul);
   if (f!=-1)
    {
      sprintf(s,"File %s save succeful",curbcfg);
      AddToLog(s);
      fwrite(f,buf,size,&ul);
    }
   else
   {
     sprintf(s,"File %s not save",curbcfg);
     AddToLog(s);
     ShowMSG(2,(int)"not save file");
   }
 fclose(f,&ul);
}

void SaveBackup()
{
   if(!strlen(name1) && !get_file_size(name1)) return;
   unsigned int ul;
   char *s=malloc(128);
   char *name = strrchr(name1,'\\')+1;     
   int i;
   for(i=strlen(name); i>0 && name[i]!='.';i--) name[i]=0;
   name[i++]=0;
   sprintf(s,"%s\\%s_backup.bcfg",backupdir,name);
   unlink(s,&ul);
   int f=fopen(s,A_ReadWrite+A_BIN+A_Create,P_READ+P_WRITE,&ul);
   if (f!=-1)
    {
      char s2[128];
      sprintf(s2,"Backup file %s save succeful",s);
      AddToLog(s2);
      fwrite(f,buf,size,&ul);
    //  ShowMSG(1,(int)"save");
    }
   else
     ShowMSG(2,(int)"not save backup file");
  fclose(f,&ul);
  mfree(s);
}


void SaveLog()
{
 if((!strlen(name1) && !get_file_size(name1)) || !toplog) return;
 LogLine* log = toplog;
 unsigned int ul;
 char *s=malloc(128);
 char *name = strrchr(name1,'\\')+1;     
 sprintf(s,"%s\\%s_bcfg.log",logdir,name);
 unlink(s,&ul);
 int f=fopen(s,A_ReadWrite+A_TXT+A_Create+A_Append,P_READ+P_WRITE,&ul);
 if (f!=-1)
  {
    while(log)  
    {
      sprintf(s,"%s\n",log->text);
      fwrite(f,s,strlen(s),&ul);
      log=log->next;
    }
  }
 else ShowMSG(1,(int)"Not save log file");
 fclose(f,&ul);
 mfree(s);
 FreeLog();
}


void OpenFile(char *fname,int flag)
{
   if(flag == 0)//первый файл
   {
     buf = malloc(get_file_size(fname));
     strcpy(name1,fname);
     strcpy(curbcfg,fname);
     menuitems[0].lgp_id_small=(int)name1;
   }
   else//второй файл
   {
     buf2 = malloc(get_file_size(fname));
     strcpy(name2,fname);
     menuitems[1].lgp_id_small=(int)name2;
   }
   
   char *mybuf = (!flag) ? (buf) : (buf2);
   int bytes;
   unsigned int ul;
   int f=fopen(fname,A_ReadOnly+A_BIN,P_READ,&ul);
   if (f!=-1)
   {
     char s[128];
     bytes = fread(f,mybuf,get_file_size(fname),&ul);
     sprintf(s,"%s file %s open %db", flag?"Source":"Dest",fname,bytes);
     AddToLog(s);
   }
   else
   {
     char s[128];
     sprintf(s,"Error open file %d %s", flag,fname);
     AddToLog(s);
   }
    
   if(!flag)
   {
     size = bytes;
     SaveBackup();
   }
   else
     size2 = bytes;
 fclose(f,&ul); 
}

void PrintAbout()
{
  WSHDR *ws=AllocWS(128);
  wsprintf(ws,"ImporterBCFG %s (c)kluchnik\n%s at %s",_VERSION_,__DATE__,__TIME__);
  DrawRoundedFrame(0,YDISP,ScreenW()-1,ScreenH()-1,0,0,0,"\xFF\xFF\xFF\x64","\x00\x00\x00\x64");
  DrawString(ws, 0,(ScreenH()/2)-(GetFontYSIZE(FONT_SMALL)*2),ScreenW()-1, ScreenH()-1, FONT_SMALL, TEXT_ALIGNMIDDLE,"\xFF\xFF\xFF\x64" ,0);  
  FreeWS(ws);
}

static void OnRedraw(MAIN_GUI *data)
{  
  DrawRoundedFrame(0,YDISP,ScreenW()-1,ScreenH()-1,0,0,0,"\xFF\xFF\xFF\x64","\x00\x00\x00\x64");
  if(!isAbout)
  {
    wsprintf(data->ws1,size?"load dest file %s...%db":"not load dest file %s...%db",name1,size);
    DrawString(data->ws1, 2, YDISP+2, ScreenW()-1, ScreenH()-1, FONT_SMALL, 0,"\xFF\xFF\xFF\x64" ,0);
    
    wsprintf(data->ws1, (size2 ? ("load source file %s...%db\n\n\nParsing...%db/%db\n%s") : ("not load source file %s...%db\nPlease press enter button and select file")) ,
                           name2, size2, cursize,size, (cursize==size ? "Parse success" : ""));
    DrawString(data->ws1, 2, YDISP+2+GetFontYSIZE(FONT_SMALL)*3, ScreenW()-1, ScreenH()-1, FONT_SMALL, 0,"\xFF\xFF\xFF\x64" ,0);
  }
  else
  {
    PrintAbout();  
  }
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  unsigned int err;
  sprintf(logdir,"%slogs",dirpath);
  if(!isdir(logdir,&err)) mkdir(logdir,&err);  
  
  sprintf(backupdir,"%sbackup",dirpath);
  if(!isdir(backupdir,&err)) mkdir(backupdir,&err);  
  
  data->ws1 = AllocWS(256);
  data->gui.state=1;
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  wsprintf(data->ws1,"Process saving...Please wait...");
  DrawString(data->ws1, 2, ScreenH()-2-GetFontYSIZE(FONT_SMALL)*3, ScreenW()-1, ScreenH()-1, FONT_SMALL, 0,"\xFF\xFF\xFF\x64" ,0);
  SaveFile();
  SaveLog();
  FreeWS(data->ws1);
  data->gui.state=0;
}

static void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  data->gui.state=2;
  DisableIDLETMR();
}

static void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

static int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  DirectRedrawGUI(); 
  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    if(msg->gbsmsg->submess == '*') isAbout=1;
    else
    {
      isAbout=0;
      switch(msg->gbsmsg->submess)
      {
         case RIGHT_SOFT: case RED_BUTTON: CloseCSM(csm_id); return (1);
      }
    }
  }
  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

int method8(void){return(0);}
int method9(void){return(0);}

const void * const gui_methods[11]={
  (void *)OnRedraw,
  (void *)onCreate,
  (void *)onClose,
  (void *)onFocus,
  (void *)onUnfocus,
  (void *)OnKey,
  0,
  (void *)kill_data,
  (void *)method8,
  (void *)method9,
  0
};


const RECT Canvas={0,0,0,0};

void CreateLogScreen(void *data)
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  CreateGUI(main_gui);  
}

static void maincsm_oncreate(CSM_RAM *data)
{
  name1[0] = '-';
  name2[0] = '-';
  MAIN_CSM *csm = (MAIN_CSM*)data;
  csm->gui_id = TestMenu();
//  CreateLogScreen(data);
}


void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  extern void kill_data(void *p, void (*func_p)(void *));
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}


static void maincsm_onclose(CSM_RAM *csm)
{
  FreeLog();
  mfree(buf);
  mfree(buf2);

  SUBPROC((void *)ElfKiller);
}


static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }
  return(1);
}


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

void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"ImporterBCFG");
}

int main(char *exename, char *fname)
{
  char *name;
  name = exename;
  for(int i=strlen(name); i>0 && name[i]!='\\';i--) name[i]=0;
  strcpy(dirpath,name);

  MAIN_CSM main_csm;
  LockSched();
  UpdateCSMname();
  csm_id = CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  UnlockSched();
}


void utf82win(char*d,const char *s)
{
  for (; *s; s+=2)
  {
    unsigned char ub = *s, lb = *(s+1);
    if (ub == 208)
      if (lb != 0x81)
        {*d = lb + 48; d++;}
      else
        {*d = '?'; d++;}

    if (ub == 209)
      if (lb != 0x91)
        {*d = lb + 112; d++;}
      else
        {*d = '¬'; d++;}

    if ((ub != 208) && (ub != 209) && (lb != 91) && (lb != 81))
    {
      *d = ub;
      d++;
      s--;
    }
  }
  *d = 0;
}


void Compare(void *p,CFG_HDR *hp,int type)
{
// return;
 char *p2 = buf2;
 int n = size2;
 CFG_HDR *hp2;
 unsigned int i;
 char *s=malloc(128);
 while(n>=sizeof(CFG_HDR))
  {
    hp2=(CFG_HDR*)p2;
    n-=sizeof(CFG_HDR);
    p2+=sizeof(CFG_HDR);
    switch(hp2->type)
    {
    case CFG_UINT:
      n-=sizeof(unsigned int);
      if (n<0)
      {
      L_ERRCONSTR3:
        AddToLog("Unexpected EOF!!!");
        return;
      }
      if(type==CFG_UINT)
      {
        if( *((unsigned int *)p) != *((unsigned int *)p2) && !strcmp(hp->name,hp2->name))
        {
            sprintf(s,"[Unsigned int] %s: %d -> %d",hp->name,*((unsigned int *)p),*((unsigned int *)p2));
            AddToLog(s);
            
            *((unsigned int *)p) = *((unsigned int *)p2);
            if(*((unsigned int *)p) > hp->max) *((unsigned int *)p) = hp->max;
        }
       }
      p2+=sizeof(unsigned int);
      break;
   case CFG_INT:
      n-=sizeof(int);
      if (n<0) goto L_ERRCONSTR3;
      //{
       // int k1,k2;
       // unsigned int min,max;
       // min=(k1=hp->min)>=0?k1:(-k1)*10;
       // max=(k2=hp->max)>=0?k2:(-k2)*10;
      //}
       if(type==CFG_INT)
       {
        if( *((int*)p) != *((int*)p2) && !strcmp(hp->name,hp2->name) )
        {
           sprintf(s,"[INT] %s: %d -> %d",hp->name,*((int *)p),*((int *)p2));
           AddToLog(s);
           *((int*)p) = *((int*)p2);
           if( *((int*)p) > hp->max ) *((int*)p) = hp->max;
           if( *((int*)p) < hp->min ) *((int*)p) = hp->min;
         }
        }
      p2+=sizeof(int);
      break;
    case CFG_STR_UTF8:
      n-=(hp2->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR3;
      if(type==CFG_STR_UTF8)
      {
        if(strcmp((char*)p,(char*)p2)!=0 && !strcmp(hp->name,hp2->name))
         {
           sprintf(s,"[UTF8_STR] %s: %s -> %s",hp2->name,(char*)p,(char*)p2);
           AddToLog(s);
           memcpy(p,p2,(hp2->max+1+3)&(~3));
         }
      }
      p2+=(hp2->max+1+3)&(~3);
      break;
    case CFG_STR_WIN1251:
      n-=(hp2->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR3;
      if(type==CFG_STR_WIN1251)
      {
        if(strcmp((char*)p,(char*)p2)!=0 && !strcmp(hp->name,hp2->name))
         {
           sprintf(s,"[WIN1251_STR] %s: %s -> %s",hp->name,(char*)p,(char*)p2);
           AddToLog(s);
           memcpy(p,p2,(hp2->max+1+3)&(~3));
        }
      }
      p2+=(hp2->max+1+3)&(~3);
      break;

    case CFG_UTF8_STRING_PASS:
    case CFG_UTF8_STRING:
      n-=(hp2->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR3;

      if(type==CFG_UTF8_STRING)
      {
        if(strcmp((char*)p,(char*)p2)!=0 && !strcmp(hp->name,hp2->name))
        {
          char *s2 = malloc(256);
          sprintf(s2,"[UTF8_STRING] %s: %s -> %s",hp->name,(char*)p,(char*)p);
          utf82win(s,s2);
          AddToLog(s);
          memcpy(p,p2,(hp2->max+1+3)&(~3));
        }
      }
      p2+=(hp2->max+1+3)&(~3);
      break;

    case CFG_CBOX:
      n-=hp2->max*sizeof(CFG_CBOX_ITEM)+4;
      if (n<0) goto L_ERRCONSTR3;
      if(type==CFG_CBOX)
      {
        i=*((int *)p2);//текущий элемент комбобокс
        if (i>=hp2->max)
        {
         AddToLog("Bad index in combobox!!!");
         goto L_ERRCONSTR3;
//          *((int *)p2) = 0;
        }
        if(*((int *)p2)  != *((int *)p)  && !strcmp(hp->name,hp2->name))
        {
          sprintf(s,"[Combo Box] %s: %s [%d item] -> %s [%d item]",hp->name,
                                     (CFG_CBOX_ITEM*)((char*)p + *((int *)p) *sizeof(CFG_CBOX_ITEM)+4),  *((int *)p),
                                     (CFG_CBOX_ITEM*)((char*)p + *((int *)p2)*sizeof(CFG_CBOX_ITEM)+4), *((int *)p2));
          *((int *)p) = *((int *)p2) ;
//          if(i > hp2->max) *((int *)p) = hp2->max;
             
          AddToLog(s);
        }
      }
      p2+=hp2->max*sizeof(CFG_CBOX_ITEM)+4;
      break;
    case CFG_STR_PASS:
      n-=(hp2->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR3;
      if(type==CFG_STR_PASS)
      {
        if(strcmp((char*)p,(char*)p2)!=0 && !strcmp(hp->name,hp2->name))
         {
           memcpy(p,p2,(hp2->max+1+3)&(~3));
           sprintf(s,"[Password str] %s: %s",hp->name,(char*)p);
           AddToLog(s);
         }
      }
      p2+=(hp2->max+1+3)&(~3);
      break;
    case CFG_COORDINATES:
      n-=8;
      if (n<0) goto L_ERRCONSTR3;
      if(type==CFG_COORDINATES)
        {
          if( ( (*(int*)p != *(int*)p2) || (*(int*)p+1 != *(int*)p2+1) ) && !strcmp(hp->name,hp2->name))
          {
            sprintf(s,"[COORDINATES] %s: x: %d -> %d",hp->name,*((int *)p),*((int *)p2));
            AddToLog(s);
            sprintf(s,"[COORDINATES] %s: y: %d -> %d",hp->name,*((int *)p+1),*((int *)p2+1));
            AddToLog(s);

            *(int*)p = *(int*)p2;
            *((int *)p+1) = *((int *)p2+1);
          }
        }
      p2+=8;
      break;
    case CFG_COLOR:
      n-=4;
      if (n<0) goto L_ERRCONSTR3;  
      if(( *((char *)p)!=*((char *)p2) || *((char *)p+1)!=*((char *)p2+1) ||
             *((char *)p+2)!=*((char *)p2+2) || *((char *)p+3)!=*((char *)p2+3)
            )&& !strcmp(hp->name,hp2->name) )
        {
            sprintf(s,"[COLOR] %s.r: %02X -> %02X",hp->name,*((char *)p),*((char *)p2));
            AddToLog(s);
            sprintf(s,"[COLOR] %s.g: %02X -> %02X",hp->name,*((char *)p+1),*((char *)p2+1));
            AddToLog(s);
            sprintf(s,"[COLOR] %s.b: %02X -> %02X",hp->name,*((char *)p+2),*((char *)p2+2));
            AddToLog(s);
            sprintf(s,"[COLOR] %s.a: %02X -> %02X",hp->name,*((char *)p+3),*((char *)p2+3));
            AddToLog(s);

           *((char *)p)   = *((char *)p2);
           *((char *)p+1) = *((char *)p2+1);
           *((char *)p+2) = *((char *)p2+2);
           *((char *)p+3) = *((char *)p2+3);
        }
      p2+=4;
      break;
    case CFG_LEVEL:
      if (n<0) goto L_ERRCONSTR3;
      continue;
    case CFG_CHECKBOX:
      n-=4;
      if (n<0) goto L_ERRCONSTR3;
      if(type==CFG_CHECKBOX)
      {
        if(  *((int *)p) != *((int *)p2) &&   !strcmp(hp->name,hp2->name) )
        {
            sprintf(s,"[CHECKBOX] %s: %01d[%s] -> %01d[%s]",hp->name,
                                      *((int *)p),(*((int *)p) ? "On" : "Off"),
                                      *((int *)p2), (*((int *)p2) ? "On" : "Off"));
            AddToLog(s);
           *((int *)p) = *((int *)p2);           
        }
      }
      p2+=4;
      break;
    case CFG_TIME:
      n-=sizeof(TTime);
      if (n<0) goto L_ERRCONSTR3;
      if(type==CFG_TIME)
      {        
        TTime *t,*t2;
        t=(TTime*)p;
        t2=(TTime*)p2;
        if( (t->min != t2->min || t->hour != t2->hour) && !strcmp(hp->name,hp2->name) )
        {
          sprintf(s,"[Time] %s: %02d:%02d -> %02d:%02d",hp->name,t->hour,t->min,t2->hour,t2->min);
          AddToLog(s);
          
          t->min  = t2->min;
          t->hour = t2->hour;
        }
      }
      p2+=sizeof(TTime);
      break;
    case CFG_DATE:
      n-=sizeof(TDate);
      if (n<0) goto L_ERRCONSTR3;
      if(type==CFG_DATE)
      {
        TDate *d,*d2;
        d=(TDate *)p;
        d2=(TDate *)p2;
        if( ( (d->year != d2->year) || (d->month != d2->month) || (d->day != d2->day) ) &&  !strcmp(hp->name,hp2->name))
        {
         sprintf(s,"[Date] %s: %02d.%02d.%02d -> %02d.%02d.%02d",hp->name,d->day,d->month,d->year-2000,d2->day,d2->month,d2->year-2000);
         AddToLog(s);
          
         d->year = d2->year;
         d->month = d2->month;
         d->day = d2->day;
        }
      }
      p2+=sizeof(TDate);
      break;

    case CFG_RECT:
      n-=sizeof(RECT);
      if (n<0) goto L_ERRCONSTR3;

      RECT *rc,*rc2;
      rc =(RECT *)p;
      rc2=(RECT *)p2;
      if(type==CFG_RECT)
      {
        if( (rc->x!=rc2->x || rc->x2!=rc2->x2 || rc->y!=rc2->y || rc->y2!=rc2->y2) && !strcmp(hp->name,hp2->name) )
        {
          sprintf(s,"[RECT] %s.x: %d -> %d",rc->x,rc2->x);
          AddToLog(s);
          sprintf(s,"[RECT] %s.y: %d -> %d",rc->y,rc2->y);
          AddToLog(s);
          sprintf(s,"[RECT] %s.x2: %d -> %d",rc->x2,rc2->x2);
          AddToLog(s);
          sprintf(s,"[RECT] %s.y2: %d -> %d",rc->y2,rc2->y2);
          AddToLog(s);
          
          rc->x  = rc2->x;
          rc->x2 = rc2->x2;
          rc->y  = rc2->y;
          rc->y2 = rc2->y2;
        }
      }
      p2+=sizeof(RECT);
      break;

    case CFG_KEYCODE:
      n-=sizeof(int);
      if (n<0) goto L_ERRCONSTR3;
      if(type==CFG_KEYCODE)
      {
        if(*(int *)p != *(int *)p2  && !strcmp(hp->name,hp2->name) ) *(int *)p = *(int *)p2;      
        sprintf(s,"[CFG_KEYCODE] %s: %d -> %d",hp->name,*((int *)p),*((int *)p2));
        AddToLog(s);
      }
      p2+=sizeof(int);
      break;
      
    default:
      AddToLog("Unsupported item");
      goto L_ENDCONSTR3;
    }
  } 
L_ENDCONSTR3:
  mfree(s);
  return;
}

void Parser()
{
  if(!buf)
  {
    ErrorMsg("buf is empty!");
    return;
  }
  char *p=buf;
  int n=size;
  CFG_HDR *hp;

  unsigned int i;
  while(n>=sizeof(CFG_HDR))
  {
    hp=(CFG_HDR*)p;    
    n-=sizeof(CFG_HDR);
    cursize = size-n;
    p+=sizeof(CFG_HDR);
    switch(hp->type)
    {
    case CFG_UINT:
      n-=sizeof(unsigned int);
      if (n<0)
      {
       L_ERRCONSTR:
          AddToLog("Unexpected EOF!!!");
          ErrorMsg("Unexpected EOF!!!");
          return;
      }
      Compare((void *)p,hp,CFG_UINT);
      p+=sizeof(unsigned int);
      break;
   case CFG_INT:
      n-=sizeof(int);
      if (n<0) goto L_ERRCONSTR;
      Compare((void *)p,hp,CFG_INT);
      p+=sizeof(int);
      break;
    case CFG_STR_UTF8:
      n-=(hp->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR;
      Compare((void *)p,hp,CFG_STR_UTF8);
      p+=(hp->max+1+3)&(~3);
      break;
    case CFG_STR_WIN1251:
      n-=(hp->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR;
      Compare((void *)p,hp,CFG_STR_WIN1251);
      p+=(hp->max+1+3)&(~3);
      break;
      
    case CFG_UTF8_STRING_PASS:
    case CFG_UTF8_STRING:
      n-=(hp->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR;
      Compare((void *)p,hp,CFG_UTF8_STRING);
      p+=(hp->max+1+3)&(~3);
      break;
    case CFG_CBOX:
      n-=hp->max*sizeof(CFG_CBOX_ITEM)+4;
      if (n<0) goto L_ERRCONSTR;
      i=*((int *)p);
      if (i>=hp->max)
      {
       ErrorMsg("Bad index in combobox!!!");
        goto L_ERRCONSTR;
      }
      Compare((void *)p,hp,CFG_CBOX);
      p+=hp->max*sizeof(CFG_CBOX_ITEM)+4;
      break;

    case CFG_STR_PASS:
      n-=(hp->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR;
      Compare((void *)p,hp,CFG_STR_PASS);
      p+=(hp->max+1+3)&(~3);
      break;
    case CFG_COORDINATES:
      n-=8;
      if (n<0) goto L_ERRCONSTR;
      Compare((void *)p,hp,CFG_COORDINATES);
      p+=8;
      break;
    case CFG_COLOR:
      n-=4;
      if (n<0) goto L_ERRCONSTR;
      Compare((void *)p,hp,CFG_COLOR);
      p+=4;
      break;
    case CFG_LEVEL:
      if (n<0) goto L_ERRCONSTR;
      continue;
    case CFG_CHECKBOX:
      n-=4;
      if (n<0) goto L_ERRCONSTR;
      Compare((void *)p,hp,CFG_CHECKBOX);
      p+=4;
      break;
    case CFG_TIME:
      n-=sizeof(TTime);
      if (n<0) goto L_ERRCONSTR;
      Compare((void *)p,hp,CFG_TIME);
      p+=sizeof(TTime);
      break;
    case CFG_DATE:
      n-=sizeof(TDate);
      if (n<0) goto L_ERRCONSTR;
      Compare((void *)p,hp,CFG_DATE);
      p+=sizeof(TDate);
      break;

    case CFG_RECT:
      n-=sizeof(RECT);
      if (n<0) goto L_ERRCONSTR;
      Compare((void *)p,hp,CFG_RECT);
      p+=sizeof(RECT);
      break;

   case CFG_KEYCODE:
      n-=4;
      if (n<0) goto L_ERRCONSTR;
      Compare((void *)p,hp,CFG_KEYCODE);
      p+=4;
      break;
    default:
      goto L_ENDCONSTR;
    }
  }
  cursize = size;
L_ENDCONSTR:
 return;
}
        
