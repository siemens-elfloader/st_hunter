#include "..\inc\swilib.h"
#include "local_ipc.h"
#include "../inc/xtask_ipc.h"
#include "main.h"
#include "language.h"
//#include "cl_work.h"
//Сдесь все криво,мне лень переписывать....работает и хуй с ним....


#define FUNC_PT_START	0xA085DEA4
#define FUNC_PT_END	0xA0865BA3
#define FUNC_PT_PSIZE	0x80
#define FUNC_PT_OFFSET	0x10
#define MAX_FUNC	0xFA

GBSTMR mytmr;
int menus_id;
int active;

const char ipc_my_name[]=IPC_TEXTINFO_NAME;
const IPC_REQ gipc={
  ipc_my_name,
  ipc_my_name,
  NULL
};

#define TMR_SECOND 216

void TimerProc(void)
{
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
}

const char _s[]="%s";

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
int collum=0;//столбцы))) 0 - первый, 1- второй

const char clrSelect[4]={0xFF,0x00,0x00,0x20};
const char fntSelect[4]={0xFF,0xFF,0x00,0x60};
const char bordSelect[4]={0x00,0x00,0x00,0x60};

const char clrNotSelect[4]={0x00,0x00,0x00,0x00};
const char bordNotSelect[4]={0x00,0x00,0x00,0x60};
const char fntNotSelect[4]={0xFF,0xFF,0xFF,0x60};

char clrBlack[]={0x00,0x00,0x00,0x64};
char clrWhite[]={0xFF,0xFF,0xFF,0x64};
char prozr[]={0x00,0x00,0x00,0x00};
const int font=FONT_SMALL;
const char _t[]="%t";
const char _d[]="%d";

char img_dir[128]="4:\\";//Путь до эльфа,для извлечения пути...

//IMGHDR *icons[16];
IMGHDR *noico;

#define CLIST_Y1 (YDISP+GetFontYSIZE(font)+8)

void PrintAbout()
{
  WSHDR *ws=AllocWS(128);
  wsprintf(ws,"AltMyMenu2 v0.2\n(c)kluchnik\n%s at %s",__DATE__,__TIME__);
  DrawRoundedFrame(0,YDISP+1,ScreenW()-1,ScreenH()-1,0,0,0,clrBlack,clrBlack);
  char *s=malloc(128);
  sprintf(s,"%s\\%s",img_dir,"bg.png");
  DrawImg(1,YDISP,(int)s);
  mfree(s);
  DrawString(ws,0,(ScreenH()/2)-GetFontYSIZE(font),ScreenW()-1,ScreenH()-1,font,TEXT_ALIGNMIDDLE,fntSelect,0);
}


static void DrawSoftKeys(WSHDR *ws,char *right,char *left)
{
  int scr_w=ScreenW()-1;
  int scr_h=ScreenH()-1;
  
  LockSched();
  wsprintf(ws,_t,right);
  DrawString(ws,2,scr_h-4-GetFontYSIZE(font),scr_w,scr_h-4,font,0,fntNotSelect,0);

  CutWSTR(ws,32);
  wsprintf(ws,_t,left);
  DrawString(ws,(scr_w >> 1),scr_h-2-GetFontYSIZE(font),scr_w-2,scr_h-2,font,TEXT_ALIGNRIGHT,fntNotSelect,0);
  UnlockSched();
}

void DrwImage(IMGHDR *img, int x, int y, char *pen, char *brush)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,pen,brush);
  DrawObject(&drwobj);
}

typedef struct
{
  void *next;
  unsigned int uin;
  WSHDR *ws_name;
  WSHDR *ws_uin;
}CLIST;

CLIST *cltop;

void AddContact(unsigned int uin,char *name)
{
  CLIST *cl=malloc(sizeof(CLIST));
  cl->next=0;
  cl->ws_name=AllocWS(strlen(name));
  cl->ws_uin=AllocWS(10);
  cl->uin=uin;
  
  wsprintf(cl->ws_name,_t,name);
  wsprintf(cl->ws_uin,_d,uin);
  if(!cltop)
  {
    cltop=cl; 
  }
  else
  {
    CLIST *c=cltop;
    while(c->next)
        c=c->next;
    c->next=cl;   
  }
}


void FreeBmkList()
{  
  LockSched();
  CLIST *cl=(CLIST *)cltop;
  cltop=0;
  UnlockSched();
  while(cl)
  {
    CLIST *cl_prev=cl;
    cl=cl->next;
    mfree(cl_prev);
  }
}

CLIST *GetBmkByItem(int curitem)
{
  CLIST *bmk;
  bmk=cltop;
  int i=0;
  while(bmk)
  {
    if(i==curitem)
      return bmk;
    i++;
    if(bmk->next) bmk=bmk->next;  
    else return 0;
  }
  return 0;
}

int TotalBmk()
{
  if(!cltop) return 0;
  CLIST *bmk;
  bmk=cltop;
  int i=1; 
  while(bmk=bmk->next) i++;
  return i;
}

//Уничтожить список
void LoadLocalCL(void)
{
 FSTATS stat;
 char fn[256];
 int f;
 unsigned int ul;
 int fsize;
 char *p,*s;
 char cn[64];
 unsigned int uin;

 strcpy(fn,"4:");
 strcat(fn,"\\local.cl");
 if (GetFileStats(fn,&stat,&ul)==-1) return;
 if ((fsize=stat.size)<=0) return;
 if ((f=fopen(fn,A_ReadOnly+A_BIN,P_READ,&ul))==-1) return;
 s=p=malloc(fsize+1);
 s[fread(f,p,fsize,&ul)]=0;
 fclose(f,&ul);
 while(*p!=0){
  char cc;
  while((cc=*p)!=';'){    //нах имя группы
   if (cc==0) goto L_EOF;
   p++;
  }
  p++;
  uin=0;
  while((cc=*p)!=';'){   //uin
   if (cc==0) goto L_EOF;
   uin*=10;
   uin+=((*p++)-'0');
  }
  p++;
  char *c=cn;
  while((cc=*p)!=';'){   //nick
   if (cc==0) goto L_EOF;
   *c++=*p++;
  }
  *c=0;
  while(*p!=13 && *p!=10 && *p!=0) p++; //нах конец строки
  if (*p!=0) p++;
  if ((*p==13)||(*p==10)) p++;
  AddContact(uin, cn);
 }
L_EOF:
 mfree(s);
}

void InitBmk()
{
  FreeBmkList();
  LoadLocalCL();
  char s[128];
  sprintf(s,"%s\\no_icon.png",img_dir);
  noico=CreateIMGHDRFromPngFile((char*)s, 0);  
}


           int clstartItem=0;
static void DrawList(WSHDR *ws)
{
  int font_y = GetFontYSIZE(font)+2;//16+2
  int scr_w=ScreenW();
  int scr_h=ScreenH()-1;
  int y2=scr_w-(GetFontYSIZE(font)+2);
#ifdef ELKA
  itemFromDisp = (sdiv(font_y,ScreenH()-CLIST_Y1-GetFontYSIZE(font)-2)*2)-1;
#else
  itemFromDisp = sdiv(font_y,ScreenH()-CLIST_Y1-GetFontYSIZE(font)-2);
#endif
  
  LockSched();
  totalItems = TotalBmk();
 
  CLIST *cl;
//  bmk=bmktop;
  int i=0;
  int start_y;
  int x=16;
        collum=0;
  while(i+clstartItem < itemFromDisp)
  {
//     if((i>=(curpage-1)*itemFromDisp) && (i<=itemFromDisp*curpage))//если те итемы который нужно выводить на экран со страници curpage
        {
          cl=GetBmkByItem(i+clstartItem);//ищем итем
          if(cl)//если такой есть то рисуем его
          {
//          wsprintf(out_ws,"%d. %t",i+1,bmname(i));//имя закладки          
            wsprintf(ws,"(%d/%d) %d",curpos+1,totalItems,cl->uin);
            if(!collum)
              start_y = (CLIST_Y1 + (((i/* - (curpage-1)*itemFromDisp*/)*font_y)/2) );    //определяем начальное смещение по у      
//            if(i>curpos) start_y+=font_y+4;
            if(i+clstartItem==curpos)//Если наш итем то выделяем
            {
              if(!collum)
              {
                collum=!collum;
                DrawRoundedFrame(1,start_y+2,(scr_w/2)-1,start_y+(font_y)+3,5,5,0,bordSelect,clrSelect);
                DrawString(cl->ws_name,x,start_y+2,(scr_w/2)-1,start_y+font_y,font,0,fntSelect,0);
  //              DrawString(cl->ws_uin,x,start_y+font_y+1,(scr_w/2)-1,start_y+(font_y*2),8,0,fntSelect,0);                
              }
              else
              {
                collum=!collum;                
                DrawRoundedFrame((scr_w/2)-4,start_y+2,scr_w-1,start_y+(font_y)+3,5,5,0,bordSelect,clrSelect);
                DrawString(cl->ws_name,x+(scr_w/2),start_y+2,scr_w-1,start_y+font_y,font,0,fntSelect,0);
//                DrawString(cl->ws_uin,x+(scr_w/2),start_y+font_y+1,scr_w-1,start_y+(font_y*2),8,0,fntSelect,0);                
              }
              
              DrawString(ws,x,YDISP+2,(scr_w/2)-1,YDISP+font_y,8,0,fntSelect,0);
            }
            else
            {
              if(!collum)
              {
               collum=!collum;              
               DrawRoundedFrame(x,start_y+1,(scr_w/2)-1,start_y+font_y,0,0,0,bordNotSelect,clrNotSelect);//clrBlack
               DrawString(cl->ws_name,x,start_y+2,(scr_w/2)-1,start_y+font_y,font,0,fntNotSelect,0);  
      //         DrawString(cl->ws_uin,x,start_y+font_y+1,(scr_w/2)-1,start_y+(font_y*2),8,0,fntSelect,0); 
              }
              else
              {
               collum=!collum;          
//               start_y-=font_y;
               DrawRoundedFrame(scr_w/2,start_y+1,scr_w-1,start_y+font_y,0,0,0,bordNotSelect,clrNotSelect);//clrBlack
               DrawString(cl->ws_name,x+(scr_w/2),start_y+2,scr_w-1,start_y+font_y,font,0,fntNotSelect,0);  
//               DrawString(cl->ws_uin,x+(scr_w/2),start_y+font_y+1,scr_w-1,start_y+(font_y*2),8,0,fntSelect,0);  
              }                
             }            
           }
         }
        i++;
//        if(i>(pageNum*itemFromDisp)) break;
  }
  UnlockSched();  
}


                        /*
// Управление курсором
void MoveUp(int i)
{
  if(!itemFromDisp)return;
  if(curpos<1)
  {
    curpos=totalItems-1;
    
    if(itemFromDisp!=totalItems)
    {
      curpage = sdiv(itemFromDisp,totalItems)+1;
    }
    else
      curpage = 1;
  }
  else
  {
    if(i)
     curpos-=2;
    else
      curpos--;
    if(curpos<(curpage-1)*itemFromDisp){ curpos++; curpage--; }
  }
 // REDRAW();
}


void MoveDown(int i)
{
  if(!totalItems)return;
  if(i) curpos+=2;
  else  curpos++;  
  if(curpos>=totalItems) { curpos=0; curpage=1; }
  if(curpage*itemFromDisp<curpos){ curpos--; curpage++; } 
 // REDRAW();
}                    */

// Управление курсором
void MoveUp(int i)
{
  if(curpos > itemFromDisp + 2 )
  {
    curpos-=i; 
    if(clstartItem > 0 && curpos > totalItems - curpos - ((itemFromDisp/2)+1) && curpos < totalItems-((itemFromDisp/2)+1))
      clstartItem-=i; 
  }
  else
    if(curpos > 0)
    {
      curpos-=i;
      if(clstartItem > 0 && curpos > ((itemFromDisp/2)+1))
        clstartItem-=i; 
      else
        if(curpos < ((itemFromDisp/2)+1))
          clstartItem=0; 
    }
  else
  {
    curpos=totalItems-1;
    if(totalItems==itemFromDisp)
      clstartItem=1;//totalItems-itemFromDisp/2;//-itemFromDisp;//-((itemFromDisp/2)+1));
    else
      clstartItem=totalItems-itemFromDisp;
  }
}


void MoveDown(int i)
{
  if(curpos < itemFromDisp - ((itemFromDisp/2)+1) ) curpos+=i;
  else
    if(curpos < totalItems - 1)
    {
      if(clstartItem < totalItems-itemFromDisp)
        clstartItem+=i;
      curpos+=i; 
    }
  else   
  {
    curpos=0;
    clstartItem=0;
  }
}

static void OnRedraw(MAIN_GUI *data)
{  
  DrawRoundedFrame(0,YDISP+1,ScreenW()-1,ScreenH()-1,0,0,0,clrBlack,clrBlack);

  LockSched();
  char *s=malloc(128);
  DrawImg(1,YDISP,(int)"4:\\bg.png");
  DrawLine(0,CLIST_Y1-3,ScreenW()-1,CLIST_Y1-3,0,clrWhite);
  DrawSoftKeys(data->ws1,LG_SELECT,LG_CLOSE);
  UnlockSched();

  DrawList(data->ws1);
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
//  LoadIcons();
  active=1;
  data->ws1=AllocWS(256);
  data->gui.state=1;
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  active=0;
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
    switch(msg->gbsmsg->submess)
    {
    case UP_BUTTON:    MoveUp(2);   break;
    case DOWN_BUTTON:  MoveDown(2); break;
    case RIGHT_BUTTON: MoveDown(1); break;
    case LEFT_BUTTON:  MoveUp(1);   break;
    case '*' : PrintAbout();        break;    
   /* case '#' :
      {
        WSHDR *ws;
        ws=AllocWS(150);
        extern const char *successed_config_filename;
        str_2ws(ws,successed_config_filename,128);
        ExecuteFile(ws,0,0);
        FreeWS(ws);
      }
      break;      */
    case RIGHT_SOFT:case RED_BUTTON: return (1);
    }
  }
  /*
  if (k=='0')
  {
    if (RunBM2(9)) return 1;
  }

  if ((k>='1')&&(k<='9'))
  { 
    if (RunBM2(k-'1')) return 1;
  }
  
  if ((msg->gbsmsg->msg==KEY_DOWN) && (k==ENTER_BUTTON || k==LEFT_SOFT))
  {
    if (RunBM2(curpos)) return(1);
  }*/
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
  InitBmk();
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  //patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
  menus_id=CreateGUI(main_gui); 
  
}


//---------------------------------------------------------------------------//

int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}


void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}


static void maincsm_onclose(CSM_RAM *csm)
{
  FreeBmkList();
  SUBPROC((void *)ElfKiller);
}


static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }
  /*
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"AltMyMenu2 config updated!");
      InitConfig();
      InitBmk();
    }
  }*/
  
/*  if (msg->msg==MSG_IPC)
  {
    IPC_REQ *ipc;
    if ((ipc=(IPC_REQ*)msg->data0))
    {
      if (strcmp_nocase(ipc->name_to,ipc_my_name)==0)
      {
        switch (msg->submess)
        {
        case IPC_UPDATE_STAT:
          GBS_StartTimerProc(&mytmr, 10*TMR_SECOND/10, TimerProc);break;
        }
      }
    }
  }*/

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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"DrawList");
}


int main(char *exename, char *fname)
{ 
  MAIN_CSM main_csm;
//  InitConfig();
  LockSched();
  UpdateCSMname();
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  UnlockSched();
  /*
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
  my_csm_id=CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  csmr->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();
  */
  //strcpy(img_dir,exename);
//  for(int i=strlen(img_dir);img_dir[i]!='\\';i--) img_dir[i]=0;
  //strcat(img_dir,"img");  
//  strcpy(img_dir,path);
  return 0;
}
