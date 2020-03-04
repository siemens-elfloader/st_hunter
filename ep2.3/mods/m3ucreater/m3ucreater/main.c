#include "..\inc\swilib.h"
#include "local_ipc.h"
#include "../inc/xtask_ipc.h"
#include "main.h"
#include "language.h"
#include "print.h"

const char ipc_my_name[]=IPC_TEXTINFO_NAME;
IPC_REQ gipc;/*={
  ipc_my_name,
  ipc_my_name,
  NULL
};*/

#define TMR_SECOND 216

void TimerProc(void)
{
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
}

const char _s[]="%s";
const char _d[]="%d";

#pragma inline=forced
int toupper(int c)
{
  if((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

int strncmpNoCase(const char *s1,const char *s2,unsigned int n)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))&&(--n)) if (!c) break;
  return(i);
}



int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}


/*------------------------------------------------------------------------*/
/*-------------------- Весь процесс отрисовки списка ---------------------*/
/*------------------------------------------------------------------------*/

int itemFromDisp;//кол-во на экране
int totalItems;//общее количество итемов
int curpos=0;//позичия курсора
int curpage=1;//текущая страница
int startItem=0;

int font, font_2;
int scr_w, scr_h;
IMGHDR *noico,
       *bgpic,
       *headpic;
void *canvasdata;

const char clrSelect[4];//={0xFF,0x00,0x00,0x20};
extern const char fntSelect[4];//={0xFF,0xFF,0x00,0x60};
extern const char bordSelect[4];//={0x00,0x00,0x00,0x60};

extern const char clrNotSelect[4];//={0xFF,0x00,0x00,0x60};
extern const char bordNotSelect[4];//={0x00,0x00,0x00,0x60}
extern const char fntNotSelect[4];//={0xFF,0xFF,0xFF,0x60};

char clrBlack[]={0x00,0x00,0x00,0x64};
char clrWhite[]={0xFF,0xFF,0xFF,0x64};
char clrRed[]={0xFF,0x00,0x00,0x64};
char transparent[]={0x00,0x00,0x00,0x00};
const char _t[]="%t";

char img_dir[128];
char header[128];


//#define CLIST_Y1 ((YDISP+2) + ( FontH(font)>(headpic->h+1)?(FontH(font)):(headpic->h+1)) )
#define CLIST_Y1 (2 + FontH(font))
#define SF_Y  scr_h - FontH(font)-2 //SoftKey Y
#define Y1 FontH(font)+2  


typedef struct
{
  void *next;
  char *fullname;
  char *name;
  int is_folder;
  int is_select;
  char *ext;
}FLIST;

typedef struct
{
  void *gui;
  int type;
}FVIEW;

volatile FLIST *fltop;



static DrwImage(IMGHDR *img, int x, int y, char *pen, char *brush)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,pen,brush);
  DrawObject(&drwobj);
}


static DrawSoftKeys(char *right,char *left)
{
  TTime t;
  GetDateTime(0,&t);
  char *s=malloc(32);
  LockSched();
  int FntH=FontH(font);
  DrawRoundedFrame(0,scr_h-FntH-2,scr_w,scr_h,0,0,0,clrBlack,clrBlack);
  for(int i=FontH(font);i>0;i-=2)
  {
    DrawRoundedFrame(0,scr_h-FntH-2,scr_w,scr_h-FntH+i,0,0,0,CursorClr,CursorClr);
  }
  //DrawRoundedFrame(0,scr_h-FontH(font)-2,ScreenW()-1,ScreenH()-1,0,0,0,clrRed,clrRed);
  if(isSystemFont) SetFontColor(clrWhite,transparent,0);  
  PrintField(2,scr_h-1-FontH(font),right,0,font,space);
  PrintField(scr_w,scr_h-1-FontH(font),left,2,font,space);
  
  sprintf(s,"%02d:%02d",t.hour,t.min);
  PrintField(scr_w/2,scr_h-1-FontH(font),s,1,font,space);
  UnlockSched();
  
  mfree(s);
}

static DrawHeader(FLIST *fl,char *s,int x)
{
   DrawRoundedFrame(0,0,scr_w,FontH(font)+4,0,0,0,clrBlack,clrBlack);
 //  char clrRed[]={0xFF,0x00,0x48,0x15};
   for(int i=FontH(font);i>0;i-=2)
   {
     DrawRoundedFrame(0,0,scr_w,FontH(font)-i+4,0,0,0,CursorClr,CursorClr);
   }  
   DrwImage(headpic,2,1,0,0);
   if(isSystemFont) SetFontColor(clrWhite,transparent,0);
   PrintField(scr_w,1,s,2,font,space);//description in header
   
   PrintField(x+2,1,header,0,font,space);
}

static DrawScrollBar(int cur,int total)
{
  int y=scr_h-CLIST_Y1-FontH(font)-2;
  int pos=y*cur/total;
  int y2=y/total;
  DrawLine(scr_w-2,CLIST_Y1,scr_w-2,scr_h-FontH(font),1,clrBlack);
  int i=0;
  DrawRoundedFrame(scr_w-4,CLIST_Y1+pos+1,scr_w,CLIST_Y1+pos+y2,0,0,0,clrBlack,clrBlack);
  for(i=0;i<y2;i+=1)
  {
    DrawRoundedFrame(scr_w-4,CLIST_Y1+pos+1,scr_w,CLIST_Y1+pos+i,0,0,0,CursorClr,CursorClr);
  }
}

void PrintAbout()
{
  char *s=malloc(128);
  sprintf(s,"m3uCreater v0.1\n(c)kluchnik\n%s at %s",__DATE__,__TIME__);
  DrawRoundedFrame(0,FontH(font)+4,scr_w,scr_h,0,0,0,clrBlack,clrBlack);  
  DrwImage(bgpic,1,FontH(font)+4,0,0);
  PrintText(ScreenW()/2,(ScreenH()/2)-(FontH(font)*1.5),s,1,font,space,1,0,0,ScreenW()-1);
  DrawSoftKeys(LG_SELECT,LG_CLOSE);
  mfree(s);
}


void Free_FLIST(void)
{
  LockSched();
  FLIST *fl=(FLIST *)fltop;
  fltop=0;
  UnlockSched();
  while(fl)
  {
    FLIST *fl_prev;
    fl_prev=fl;
    fl=fl->next;
    mfree(fl_prev->fullname);
    mfree(fl_prev->name);
    mfree(fl_prev);
  }
}

#define IS_FOLDER 1
#define IS_FILE 0

const char back[]="..";             
int GetFListN()
{
  int i=0;
  FLIST *fl=(FLIST*)&fltop;
  while((fl=fl->next)) i++;
  return (i);
}

int GetFoldersLevel(char *name)
{
  int i=0;
  char *s=name;
  while(*s++)
  {
    if (*s=='\\' &&*(s+1)!=0) i++;    
  }
  return (i);
}

int strcmp_nocase(const char *s, const char *d)
{
  int cs;
  int ds;
  do
  {
    cs=*s++;
    if (cs&0x40) cs&=0xDF;
    ds=*d++;
    if (ds&0x40) ds&=0xDF;
    cs-=ds;
    if (cs) break;
  }
  while(ds);
  return(cs);
}

FLIST *AddToFList(const char* full_name, const char *name, int is_folder)
{
  FLIST *fl;
  FLIST *pr;
  FLIST *fn=malloc(sizeof(FLIST));
  fn->fullname=malloc(strlen(full_name)+1);
  strcpy(fn->fullname,full_name);
  
  fn->name=malloc(strlen(name)+1);
  strcpy(fn->name,name);
  
  fn->is_folder=is_folder;
  fn->is_select=0;
  fn->next=0;
  fl=(FLIST *)fltop;
  if (fl)
  {
    pr=(FLIST *)&fltop;
    while(strcmp_nocase(fl->name,fn->name)<0)
    {
      pr=fl;
      fl=fl->next;
      if (!fl) break;
    }
    fn->next=fl;
    pr->next=fn;
  }
  else
  {
    fltop=fn;
  }
  return (fn);  
}

void FindFiles(char *str,char *ext/*, int type*/)  // type == 0 SelectFolder, type == 1 SelectFile
{
  DIR_ENTRY de;
  int type=1;
  unsigned int err;
  int i, c;
  char name[256]; 
  char fname[128];

  Free_FLIST();
  strcpy(name,str);
  strcat(name,ext);
  
  i=GetFoldersLevel(str);
  if (i==0)
  {
    AddToFList("ROOT",back,IS_FOLDER);
  }
  else
  {
    char *s=str;
    char *d=fname;
    for (int k=0; k!=i && *s; )
    {
      c=*s++;
      *d++=c;
      if (c=='\\')  k++;
    }
    *d=0;
    AddToFList(fname,back,IS_FOLDER);
  }
  if (FindFirstFile(&de,name,&err))
  {
    do
    {
      
      strcpy(name,de.folder_name);
      strcat(name,"\\");
      strcat(name,de.file_name);
      
      if (de.file_attr&FA_DIRECTORY)
      {
        strcpy(fname,"\\");
        strcat(fname,de.file_name);
        strcat(name,"\\");
        AddToFList(name,fname,IS_FOLDER);
      }
      else
      {
        if (type!=0)
        {
          AddToFList(name,de.file_name,IS_FILE);
        }
      }
    }
    while(FindNextFile(&de,&err));
  }
  FindClose(&de,&err);
}



FLIST *FindFLISTtByNS(int *i, int si)
{
  FLIST *fl;
  fl=(FLIST *)fltop;
  while(fl)
  {
    if (fl->is_folder==si)
    {
      if (!(*i)) return (fl);
      (*i)--;
    }    
    fl=fl->next;
  }
  return fl;
}
  
FLIST *FindFLISTtByN(int n)
{
  FLIST *fl;
  fl=FindFLISTtByNS(&n,IS_FOLDER); if ((!n)&&(fl)) return(fl);
  fl=FindFLISTtByNS(&n,IS_FILE); if ((!n)&&(fl)) return(fl);
  return fl;
}

char header[];
void CreateRootMenu()
{
  Free_FLIST();
  AddToFList("0:\\","0:\\",IS_FOLDER);
  AddToFList("1:\\","1:\\",IS_FOLDER);
  AddToFList("2:\\","2:\\",IS_FOLDER);
  AddToFList("4:\\","4:\\",IS_FOLDER);
  strcpy(header,"Root");
}

void Create()//open_fm()
{
  CreateRootMenu();
//  FVIEW *fview;
/*  char *s;
  WSHDR *ws=AllocWS(256);
  //fview=malloc(sizeof(FVIEW));
  if (ws->wsbody[0]==0)
  {
    CreateRootMenu();
  }
  else
  {
    ws_2str(ws,header,127);
    s=strrchr(header, '\\');
    if (s) *(s+1)=0;
    int len=strlen(header);
    header[len>127?127:len]=0;
    FindFiles(header);
  }    */
}

#ifdef ELKA
  int x2_cb=15;
#else
  int x2_cb=10;
#endif
  
static DrawCheckBox(int x,int y,int flag)
{
  RECT rc;

  StoreXYXYtoRECT(&rc,x,y,x+x2_cb,y+x2_cb);
  DrawRectangle(rc.x,rc.y,rc.x2,rc.y2,0,clrBlack,transparent);
  if(flag)
  {
    DrawLine(rc.x+2,rc.y+5,rc.x2-7,rc.y2-2,0,clrBlack);
    DrawLine(rc.x2-7,rc.y2-2,rc.x+x2_cb,rc.y-2,0,clrBlack);
  }
}

static void DrawList()
{
  int font_y = FontH(font)+4;//16+2
  int ii;
  itemFromDisp = sdiv(font_y,scr_h-CLIST_Y1-FontH(font)-2)-2;
  
  LockSched();
  totalItems = GetFListN();
  
  if(itemFromDisp > (CLIST_Y1 - (2-FontH(font)))) itemFromDisp--; //Вдруг надпись будет налазить на софты ;)
  
  FLIST *fl;
  int i=0;
  int y2=1;
  int start_y=0;
  //int end_y=0;
//  int start_x=0;
  int x=iconSize;
  char *s=malloc(64);
  RECT rc;


  while(i<itemFromDisp)
  {
  //  if((i>=(curpage-1)*itemFromDisp) && (i<=itemFromDisp*curpage))//если те итемы который нужно выводить на экран со страници curpage
     {
        fl=FindFLISTtByN(i+startItem);
        if(!fl) break;      
        if(fl)//если такой есть то рисуем его
        {
          sprintf(s,"%d(%d)/%d",curpos+1,startItem,totalItems);
          start_y=0;

          if(i+startItem>curpos) 
              start_y+=font_y+3;
          y2=2; 
         
          start_y += (CLIST_Y1 + 2 + (i/* - (curpage-1)*itemFromDisp*/)*font_y);    //определяем начальное смещение по у      
          StoreXYXYtoRECT(&rc,2,start_y,scr_w,start_y+(font_y*y2));            

          
          if(i+startItem==curpos)//Если наш итем то выделяем
          {
            if(isSystemFont)  SetFontColor(clrWhite,transparent,0);              
            if(isGradient)
            {
              DrawRoundedFrame(rc.x,rc.y,rc.x2-5,rc.y2,0,0,0,clrBlack,clrBlack);//cursor        
              for(ii=(font_y*y2);ii>0;ii-=4)
               {
                 DrawRoundedFrame(rc.x,rc.y,rc.x2-5,rc.y2-ii,0,0,0,CursorBGClr,CursorClr);
               }
            }
            else DrawRoundedFrame(rc.x,rc.y,rc.x2-5,rc.y2,0,0,0,CursorBGClr,CursorClr);//cursor        

            DrawHeader(fl,s,x);
            DrawScrollBar(i,totalItems);            
            PrintText(rc.x+x,rc.y+font_y,fl->fullname,0,font,space,0,1,0,rc.x2);//description under item name
          }
          else
              if(isSystemFont)  SetFontColor( ((fl->is_folder)?(clrRed):(clrBlack)),transparent,0); 

          if(fl->is_folder==IS_FILE)
          {
            DrawCheckBox(rc.x,rc.y,fl->is_select);
            rc.x+=10;
          }
          PrintText(rc.x+x,rc.y+1,fl->name,0,font,space,0,1,0,rc.x2-5);//item name                   
         }
       }
      i++; 
      if(i+startItem > totalItems) break;
  }  
  mfree(s);
  UnlockSched();  
}

void MoveUp(int i)
{
  //if(
  if(curpos > itemFromDisp + 2 )
  {
    curpos-=i; 
    if(startItem > 0 && curpos > 5 && curpos < totalItems-5)
      startItem-=i; 
  }
  else
    if(curpos > 0)
    {
      curpos-=i;
      if(startItem > 0 && curpos > 5)
        startItem-=i; 
      else
        if(curpos < 5)
            startItem=0; 
    }
  else
  {
    curpos=totalItems-1;
    startItem=totalItems-itemFromDisp;
    if(startItem<0) startItem=0;
  }
}


void MoveDown(int i)
{
  if(curpos < itemFromDisp - 5) curpos+=i;
  else
    if(curpos < totalItems - 1)
    {
      if(startItem < totalItems-itemFromDisp) startItem+=i;
      
      curpos+=i; 
    }
  else   
  {
    curpos=0;
    startItem=0;
  }
}

static void OnRedraw(MAIN_GUI *data)
{
//  if(!bmktop) return;
#ifdef ELKA
  DisableIconBar(1);
#endif
  //ClearScreen();
  LockSched();
  DrawRoundedFrame(0,0,scr_w,scr_h,0,0,0,clrWhite,clrWhite);
  
//  DrwImage(bgpic,1,CLIST_Y1,0,0);
//  DrawImg(0,20,(int)"0:\\alt\\img\\bg.png");
 // DrawLine(0,CLIST_Y1,scr_w,CLIST_Y1,0,clrWhite);
  
  DrawSoftKeys(LG_SELECT,LG_CLOSE);
  UnlockSched();

  DrawList();
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
#ifdef ELKA
  DisableIconBar(1);
#endif
  totalItems = GetFListN();
  Create();
  data->ws1=AllocWS(256);
  data->gui.state=1;
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(0);
#endif
  FreeWS(data->ws1);
  data->gui.state=0;
}

static void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(1);
#endif
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
  int k=msg->gbsmsg->submess;

  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(k)
    {  
      case UP_BUTTON:   MoveUp(1);  break;
      case DOWN_BUTTON: MoveDown(1);break;
      case LEFT_BUTTON: MoveUp(2);  break;
      case RIGHT_BUTTON:MoveDown(2);break;
      case '*' : PrintAbout();        break;    
      case '#' :
      {
        WSHDR *ws;
        ws=AllocWS(150);
        extern const char *successed_config_filename;
        str_2ws(ws,successed_config_filename,128);
        ExecuteFile(ws,0,0);
        FreeWS(ws);       
      }
      break;
      case RED_BUTTON: return (1);
    }
  }
  
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(k)
    {  
      case ENTER_BUTTON:
        {
            FLIST *fl;
            fl=FindFLISTtByN(curpos);
            if (fl) 
            {
              if (fl->is_folder==IS_FOLDER)
              {
                int len;
                if (strcmp(fl->fullname,"ROOT"))
                {
                  strncpy(header,fl->fullname,127);
                  len=strlen(fl->fullname);
                  header[len>127?127:len]=0;
                  FindFiles(fl->fullname);
                }
                else
                {
                  void CreateRootMenu();
                  CreateRootMenu();
                }         
                curpos=0;
                startItem=0;
                REDRAW();
              }
              else
              {
                fl->is_select=!fl->is_select;
//                SavePath(fview->gui,fl);        
//                return (1);
                REDRAW();
              }
            }

            //    GeneralFunc_F1(1);
           // return(-1);
          }
          break;
      case RIGHT_SOFT:          
          {
            FLIST *fl;
            fl=FindFLISTtByN(curpos);
  
            if (fl)
            {
              if (strcmp(fl->name,back))
              {                
//                SavePath(fview->gui,fl);
//                return (1);
              }
            }   
            return (-1);
          }   
      }
  }

  
  if (msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(k)
    {
      case UP_BUTTON:   MoveUp(0); break;
      case DOWN_BUTTON: MoveDown(0);break;
      case LEFT_BUTTON: MoveUp(1);  break;
      case RIGHT_BUTTON:MoveDown(1);break;
      case '*' : PrintAbout();      break;         
    }
  }

  
  if ((msg->gbsmsg->msg==KEY_DOWN) && (k==ENTER_BUTTON || k==LEFT_SOFT))
  {   
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

static void maincsm_oncreate(CSM_RAM *data)
{
  scr_w=ScreenW()-1;
  scr_h=ScreenH()-1;
  
  MAIN_CSM*csm=(MAIN_CSM*)data;
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
 // main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  //patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
  csm->csm.state=0;
  csm->csm.unk1=0;
  csm->gui_id=CreateGUI(main_gui);  
}


//---------------------------------------------------------------------------//


void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}


static void maincsm_onclose(CSM_RAM *csm)
{
  Free_FLIST();
  PrintLibFree();
  /*
  if(bgpic)   mfree(bgpic->bitmap);
  if(headpic) mfree(headpic->bitmap);
  if(noico)   mfree(noico->bitmap);
  */
  SUBPROC((void *)ElfKiller);
}


static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }
  
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"m3uCreater config updated!");
      InitConfig();

      if(isSystemFont) font=100+pngFont;
      else font=1;
    
      PrintLibFree();
      PrintLibInit(1);  
      SetCanvasFlag(1);
       
      FontPathInit((char *)fontpath, fullfont);
      if(isSystemFont)
        SetFontColor((char*)fntSelect,(char*)bordSelect,isOutline);  
      Create();
    }
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"msuCreater");
}

int main(char *exename, char *fname)
{  
  InitConfig();
  MAIN_CSM main_csm;
  LockSched();
  UpdateCSMname();
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  UnlockSched();

  strcpy(img_dir,path);
  
  if(!isSystemFont)
  {
    if(!strlen(fontpath))
    {      
      WSHDR *ws;
      ws=AllocWS(150);
      extern const char *successed_config_filename;
      str_2ws(ws,successed_config_filename,128);
      ExecuteFile(ws,0,0);
      FreeWS(ws);
      ShowMSG(1,(int)"Set fonts!!!");
      CloseCSM(my_csm_id);
    } 
  }
  
  if(isSystemFont)   font=100+pngFont;
  else font=1;
  
  PrintLibInit(1);  
  SetCanvasFlag(1);
  
  FontPathInit((char *)fontpath, fullfont);
  if(isSystemFont)
    SetFontColor((char*)fntSelect,(char*)bordSelect,isOutline); 
  
  return 0;
}
