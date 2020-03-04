#include "../inc/swilib.h"
#include "naticq.h"
#include "history.h"
#include "language.h"
//#include "decode.h"
#include "strings.h"
//#include "strings3.h"
#include "configs.h"
#include "main.h"
#include "print.h"

//extern char HIST_PATH[];
//extern int LOG_ALL;
//extern int HISTORY_TYPE;
//extern unsigned int UIN;
//extern int HISTORY_SAVE_TYPE;
/*
  Добавлет строку в историю контакта CLIST
*/

static do_logwrite(const char *name, unsigned int u_i_n, char *text)
{
  volatile int hFile;
  unsigned int io_error = 0;
  unsigned int ul;
  char error[36];
  char fullname[128];
  char hist_path[128];
  if(HISTORY_TYPE)
    snprintf(hist_path,127,"%s\\%u",HIST_PATH, UIN);
  else
    snprintf(hist_path,127,"%s",HIST_PATH);
  if (!isdir(hist_path,&ul))
  {
    mkdir(hist_path,&ul);
  }
  if(HISTORY_SAVE_TYPE)
  {
    snprintf(fullname,127,"%s\\%s(%d).txt", hist_path, name, u_i_n);
  }     
  else
  {
    snprintf(fullname,127,"%s\\%d.txt", hist_path, u_i_n); 
  }
  //snprintf(fullname,127,"%s\\%s(%d).txt", hist_path, n_a_m_e_z, u_i_n);
//  mfree(buf);
  // Открываем файл на дозапись и создаём в случае неудачи
  hFile = fopen(fullname,A_ReadWrite + A_Create + A_Append + A_BIN,P_READ+P_WRITE, &io_error);
  if(hFile!=-1)
  {
    fwrite(hFile, text, strlen(text), &io_error);
    fclose(hFile, &io_error);
  }
  else
  {
    snprintf(error,35, LG_HISTIOERR, io_error);
    LockSched();
    ShowMSG(1,(int)error); 
    UnlockSched();
  }
  mfree(text);
}
/*
static do_logwrite(unsigned int uin,char *text)
{
  volatile int hFile;
  unsigned int io_error = 0;
  unsigned int ul;
  char error[36];
  char fullname[128];
  char hist_path[128];
  if(HISTORY_TYPE)
    snprintf(hist_path,127,"%s\\%u",HIST_PATH, UIN);
  else
    snprintf(hist_path,127,"%s",HIST_PATH);
  if (!isdir(hist_path,&ul))
  {
    mkdir(hist_path,&ul);
  }
  snprintf(fullname,127,"%s\\%u.txt", hist_path, uin);
//  mfree(buf);
  // Открываем файл на дозапись и создаём в случае неудачи
  hFile = fopen(fullname,A_ReadWrite + A_Create + A_Append + A_BIN,P_READ+P_WRITE, &io_error);
  if(hFile!=-1)
  {
    fwrite(hFile, text, strlen(text), &io_error);
    fclose(hFile, &io_error);
  }
  else
  {
    snprintf(error,35, LG_HISTIOERR, io_error);
    LockSched();
    ShowMSG(1,(int)error); 
    UnlockSched();
  }
  mfree(text);
}
*/
void Add2History(CLIST *CListEx, char *header, char *message, int direction)
{
  static const char *delim = "\r\n--------------<>-%04d\r\n";
  int len;
  if (!LOG_ALL) return;
  len=strlen(delim)+strlen(header)+strlen(message);
  char *text=malloc(len+1);
  sprintf(text,delim,direction);
  strcat(text,header);
  strcat(text,message);
  do_logwrite(CListEx->name, CListEx->uin, text);
//  SUBPROC((void*)do_logwrite,CListEx->uin,text);
//    SUBPROC((void*)do_logwrite2,CListEx->uin, text);
}

extern LOGQ *NewLOGQ(const char *s);
extern int AddLOGQ(LOGQ **queue, LOGQ *p);

//Делает ярким последний икс-статус, а остальные тусклыми
void ActivateLastX(LOGQ *p)
{
  LOGQ *q = p, *lastX = 0;
  while(q)
  {
    if((q->type&0x0F) == 3)
    {
      lastX = q;
      q->type = 0x13;
    }
    q = q->next;
  }
  if(lastX) lastX->type = 3;
}

//Добавить элемент в лог первым
void AddFirstLOGQ(CLIST *t, LOGQ *p)
{
  LOGQ *q = t->log;
  if(!p) return;
  t->log = p;
  while(p->next)
    p = p->next;
  p->next = q;
  ActivateLastX(t->log);
}

//Удаляем лог с заданного элемента
void DeleteLOGQ(LOGQ *p)
{
  if(!p) return;
  if(p->next) DeleteLOGQ(p->next);
  mfree(p);
}

//Проверка что в логе не только иксстасусы
int CheckLOGQ(CLIST *t)
{
  LOGQ *p = t->log;
  if(!p) return 0;
  for(; p->next && (p->type&0x0F)==3; p=p->next);
  if(p->next)
    if((p->type&0x0F)!=3)
      return 1;
  DeleteLOGQ(t->log);
  t->log = 0;
  return 0;
}

//extern int HISTORY_SAVE_TYPE;

int GetHistory(CLIST *t, int bufsize)
{
  LOGQ *log, *head;
  static const char *delim = "\r\n--------------<>-000";
  volatile int hFile;
  unsigned int io_error = 0;
  char fullname[128], *s, *b, *e, *text, *str, *buf;
  int i, delimlen = strlen(delim)+3, direction;
//  unsigned uin = t->uin;
  
  if(CheckLOGQ(t)) return 0;
  
  buf = text = malloc(bufsize);
  text[0] = 0;
  text[bufsize-1] = 0;
  if(HISTORY_SAVE_TYPE)
  {
    if(HISTORY_TYPE)
      snprintf(fullname,127,"%s\\%u\\%s(%d).txt", HIST_PATH, UIN, t->name, t->uin);
    else
      snprintf(fullname,127,"%s\\%s(%d).txt", HIST_PATH, t->name, t->uin);
  }     
  else
  {
    if(HISTORY_TYPE)
      snprintf(fullname,127,"%s\\%u\\%u.txt", HIST_PATH, UIN, t->uin);
    else
      snprintf(fullname,127,"%s\\%u.txt", HIST_PATH, t->uin);
  }
  hFile = fopen(fullname,A_ReadOnly + A_BIN,P_READ, &io_error);
  if(hFile!=-1)
  {
    lseek(hFile, -(bufsize-1), S_END, &io_error, &io_error);
    i = fread(hFile, text, bufsize-1, &io_error);
    text[i] = 0;
    fclose(hFile, &io_error);
    s = strstr(text, delim);  
    
    head = NewLOGQ("");
    head->next = 0;
    
    while(s && text)
    {
      direction = (*(s+delimlen-3))-0x30; 
      text = s+delimlen;
      s = strstr(text, delim); 
      e = !s?(text+strlen(text)):s;
      
      str = malloc(e-text+1);
      if(e-text > 0) memcpy(str, text, e-text);
      str[e-text] = 0; 

//      snprintf(fullname, 127, "\r\n-----\r\n%08X\r\n%08X, %d\r\n%08X, %d\r\n----\r\n", text, e, direction, s, e-text);
      b = strstr(str, "\r\n");

      
//      log = NewLOGQ(fullname);
      if(b)
      {
        log = NewLOGQ(b+2);
        if(b-str >= 0) memcpy(log->hdr, str, b-str);
        log->hdr[b-str] = 0;
      }
      else
      {
        log = NewLOGQ("");
        strcpy(log->hdr, str);
      }
      
      log->type = direction|0x10;
      log->acked = 0;
      log->ID=0xFFFFFFFF;
      
      AddLOGQ(&head, log);
      
      //mfree(log);
      mfree(str);
    }
    AddFirstLOGQ(t, head->next);
    mfree(head);
      
  }

  mfree(buf);
  return 0;
}

void GetStatusById(char *buffer, int id)
{  
  // ??? Тут надо разбираться со статусами
  // Похоже, не все коды верные...
  switch(id)
  {
    case 0: {strcpy(buffer,"Online");break;}
    case 1: {strcpy(buffer, "Invisible");break;}
    case 2: {strcpy(buffer, "Away");break;}
    case 3: {strcpy(buffer, "N/A");break;}
    case 4: {strcpy(buffer, "Occupied");break;}
    case 5: {strcpy(buffer, "DND");break;}
    case 6: {strcpy(buffer, "Online");break;}
    case 7: {strcpy(buffer, "FFC");break;}
    default: break;// {sprintf(buffer, "UNK: %d", id);}
  }
}


static AddToLog(char *s)
{
  volatile int hFile;
  unsigned int io_error = 0;
  char fullname[128];
  snprintf(fullname,127,"%s\\logs.txt",TEMPLATES_PATH);
  char *ss=malloc(256);
  TTime t;
  GetDateTime(0,&t);  
  sprintf(ss,"%02d:%02d:%02d %s",t.hour,t.min,t.sec,s);
  hFile = fopen(fullname,A_ReadWrite + A_Create + A_Append + A_BIN,P_READ+P_WRITE, &io_error);
  if(hFile!=-1)
  {
    fwrite(hFile, ss, strlen(ss), &io_error);
    fclose(hFile, &io_error);
  }
  mfree(ss);
}

char _u[]="%u";

void LogStatusChange(CLIST *CListEx)
{
  extern int LOG_STATCH;
  char hdr[] = "(System message) ";
  char msg[] = "%s меняет статус на %s\r\n";
  char message[100];
  char nickname[64];
  char status[20];
  if(CListEx->name)
  {
    strcpy(nickname, CListEx->name);
  }
  else
  {
    snprintf(nickname,63,_u, CListEx->uin);
  }
  if(CListEx->state>7){return;}
  GetStatusById(status, CListEx->state);
  sprintf(message, msg, nickname, status);
  if(!LOG_STATCH){return;}
  if(CListEx->log==NULL){return;}
  Add2History(CListEx, hdr, message, 0);
}


void LogStatusChange2(CLIST *CListEx)
{
  if(!IsLogEna) return;
  extern int LOG_STATCH;
  char msg[] = "%s[%d] -> %s \r\n";
  char message[100];
  char nickname[64];
  char status[20];
  if(CListEx->name)
  {
    strcpy(nickname, CListEx->name);
  }
  else
  {
    snprintf(nickname,63, _u, CListEx->uin);
  }
  if(CListEx->state>7){return;}
  GetStatusById(status, CListEx->state);
  sprintf(message, msg, nickname,CListEx->uin,status);
  AddToLog(message);
}

static int _itemFromDisp;//кол-во на экране
static int _totalItems;//общее количество итемов
static int startItem;

static char *temp_chars; //Собственно файл
static char **temp_lines; //Массив указателей на строки

void FreeLog(void)
{
  if (temp_lines) mfree(temp_lines);
  if (temp_chars) mfree(temp_chars);
  temp_lines=NULL;
  temp_chars=NULL;
}

static int LoadLog()
{
  FSTATS stat;
  char fn[256];
  int f;
  unsigned int ul;
  int i;
  int fsize;
  char *p;
  char *pp;
  int c;
  FreeLog();
  strcpy(fn,TEMPLATES_PATH);
  i=strlen(fn);
  sprintf(fn+i,"\\logs.txt");
  if (GetFileStats(fn,&stat,&ul)==-1) return 0;
  if ((fsize=stat.size)<=0) return 0;
  if((f=fopen(fn,A_ReadOnly+A_BIN,P_READ,&ul))==-1) return 0;
  p=temp_chars=malloc(fsize+1);
  p[fread(f,p,fsize,&ul)]=0;
  fclose(f,&ul);
  i=0;
  pp=p;
  for(;;)
  {
    c=*p;
    if (c<32)
    {
      if (pp&&(pp!=p))
      {
	temp_lines=realloc(temp_lines,(i+1)*sizeof(char *));
	temp_lines[i++]=pp;
      }
      pp=NULL;
      if (!c) break;
      *p=0;
    }
    else
    {
      if (pp==NULL) pp=p;
    }
    p++;
  }
  return i;
}

extern char clrBlack[];
extern char clrWhite[];
extern char transparent[];

#define CLIST_Y1 (YDISP+2)


//extern void DrwImage(IMGHDR *img, int x, int y, char *pen, char *brush);
static int _font;
int start_x=2;
static void DrawList()
{
  int font_y = FontH(_font)*2;
  int fnt = FontH(_font);
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;
  _itemFromDisp = sdiv(font_y,scr_h-CLIST_Y1-FontH(_font)-2)+1;
  
  LockSched();
  
  int i=0;
  int start_y=0;
 
  RECT rc;
  SetFontColor((char*)FontClrLog,transparent,0);        
  char s[64];
  sprintf(s,"Current: %d Total: %d",startItem+i,_totalItems);
  char *ss=malloc(256);
  while( (i < _itemFromDisp) && (startItem+i < _totalItems) )
  {
   start_y = (font_y + 2 + (i*font_y));    //определяем начальное смещение по у      
   StoreXYXYtoRECT(&rc,start_x,start_y,ScreenW()-1,start_y+font_y);            
   
   if(temp_lines[startItem+i])
   {
     sprintf(ss,"%02d. %s",startItem+i,temp_lines[startItem+i]);
     PrintText(rc.x,rc.y-1,ss,0,_font,1,320,0,0,(rc.x2*2));//item name      
   }
   
   PrintField(2,2,s,0,_font,1);
//   PrintField(rc.x,rc.y-1,temp_lines[startItem+i],0,_font,1);
   DrawLine(0,rc.y-(fnt/2),scr_w,rc.y-(fnt/2),1,clrWhite);
   i++;
  }
  UnlockSched();  
  mfree(ss);
}

// Управление курсором
static void MoveUp(int i)
{ 
  if(startItem > 0){ startItem--; }
  else
  {
    startItem=_totalItems-_itemFromDisp;
  }
}


static void MoveDown(int i)
{
  if(startItem <_totalItems - 5) startItem++;
  else   
  {
    startItem=0;
  }
}  

static void OnRedraw(MAIN_GUI *data)
{  
#ifdef ELKA
  DisableIconBar(1);
#endif
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,BGLog,BGLog);
  DrawList();
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{ 
  SetCanvasFlag(1);
  start_x=2;
  _font=_fontlog;
  data->gui.state=1;
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  FreeLog();
#ifdef ELKA
  DisableIconBar(0);
#endif
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
    case RIGHT_BUTTON: /*MoveDown(1);*/start_x-=10; break;
    case LEFT_BUTTON:  /*MoveUp(1);*/start_x+=10;   break;
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

static int method8(void){return(0);}
static int method9(void){return(0);}

static const void * const gui_methods[11]={
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


void CreateLog()
{
  const RECT Canvas={0,0,0,0};
  _totalItems=LoadLog();  
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  //patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
  CreateGUI(main_gui);   
}

