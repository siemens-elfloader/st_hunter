#include "..\inc\swilib.h"
#define TMR_SECOND 216

//#pragma swi_number=0x80CF: RamAlarm
//__swi __arm int *RamAlarm;

const int minus11=-11;
//extern void ShowBMmenu1(void);
unsigned short maincsm_name_body[140];


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


//------------------------------------------------
WSHDR *ws;
int i=0;
int xx=0,yy=0;
void DrawBG(int x,int y)
{
  char *s;
  char *ss[1]={"Данная книга преследует две цели: она является учебником,  a  так  жепостоянным справочным  пособием  для  работы.  Чтобы  наиболее  эффективновосполнить затраты на микрокомпьютер и программное обеспечение, необходимотщательно прорабатывать каждую главу и перечитывать материал,  котоpый  несразу ясен.  Ключевые моменты находятся в примерах  программ,  их  следуетпреобразовать  в  выполнимые  модули  и  выполнить   их.    Прорабатывайтеупражнения, приведенные в конце каждой главы.     Первые восемь глав составляют базовый материал для данной книги и дляязыка ассемблера.  После этих глав можно продолжить с глав 9, 11, 12,  14,15, 19, 20 или 21. Связанными являются главы с 8 по 10, 12 и 13, с  15  по18, главы с 22 по 25 cодержат справочный материал.     Когда вы завертшите работу с книгой, вы сможете:          - понимать устpойство персонального компьютера;          - понимать коды машинного языка и шестнадцатиричный формат;          -  понимать  назначение  отдельных  шагов  при  ассемблировании,     компановке и выполнении;          - писать программы на языке ассемблера для  управления  экраном,     арифметических  действий,  преобразования  ASCII  кодов  в   двоичные     форматы,  табличного  поиска  и   сортировки,    дисковых    операций     ввода/вывода;          - выполнять трассировку при выполнении программы,  как  средство     отладки;          - писать собственные макрокоманды;          - компановать вместе отдельные программы."};

  DrawRoundedFrame(0,0,131,175,0,0,0,GetPaletteAdrByColorIndex(4),GetPaletteAdrByColorIndex(1)); 
   ws = AllocWS(10000); 
 //strncpy(s,ss[0],i);
 //snprintf(s,i,"%s",ss[0]);
 s=malloc(256);
// strcpy(s,"");
 int j=i;
 int k=0;
// int y=GetFontYSIZE(7);
// while(j<i+80){*(s+k)=*(ss+j);j++;k++;}
 //*(s+j++)=0;

 wsprintf(ws,"%s\n\ni=%d\nj=%d",ss[0],i,j);
 DrawString(ws,x,y,131,172,7,0,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23)); 
 FreeWS(ws);
}

void OnRedraw(MAIN_GUI *data)
{
 DrawBG(xx,yy);
}

void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  data->gui.state=1;
}

void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  data->gui.state=0;
}

void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  data->gui.state=2;
  DisableIDLETMR();
}

void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  DirectRedrawGUI();  
  if (msg->gbsmsg->msg==KEY_DOWN )
  {
    switch(msg->gbsmsg->submess)
    {
      case '2':
        yy+=10;
    //    if(i>0) i-=10;
      //  else i=0;
        break;
      case '8':
        yy-=10;
//        if(i<80) i+=10;
  //      else i=80;
        break;
    case '4':xx--;break;
    case '6':xx++;break;
      case RED_BUTTON:return(1);
    }
    return 0;
  }
  if (msg->gbsmsg->msg==LONG_PRESS )
  {
    switch(msg->gbsmsg->submess)
    {
      case '2':
        yy+=10;
    //    if(i>0) i-=10;
      //  else i=0;
        break;
      case '8':
        yy-=10;
//        if(i<80) i+=10;
  //      else i=80;
        break;
    case '4':xx--;break;
    case '6':xx++;break;
      case RED_BUTTON:return(1);
    }
    return 0;
  }    
  return(0);
}
#ifdef NEWSGOLD
void kill_data(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  mfree_adr(data);
}
#else
extern void kill_data(void *p, void (*func_p)(void *));
#endif
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
  (void *)kill_data,	//Destroy
  (void *)method8,
  (void *)method9,
  0
};

const RECT Canvas={0,0,131,172};
void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  csm->gui_id=CreateGUI(main_gui);
  MAINGUI_ID=csm->gui_id;
}

void ElfKiller(void)
{
  
  extern void kill_data(void *p, void (*func_p)(void *));
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

void maincsm_onclose(CSM_RAM *csm)
{
  SUBPROC((void *)ElfKiller);
}

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }
  return(1);
}

const struct
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
  WSHDR *ws=AllocWS(256);
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"exe");
  FreeWS(ws);
}


int main(char *exename, char *fname)
{
  char dummy[sizeof(MAIN_CSM)];
  MAINCSM_ID = CreateCSM(&MAINCSM.maincsm,dummy,0);
  UpdateCSMname();
  return 0;
}
