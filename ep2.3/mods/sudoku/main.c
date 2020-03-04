#include "..\inc\swilib.h"
#include "..\inc\rect_patcher.h"
#include "conf_loader.h"

//BCFG Переменные
extern const unsigned int bcfg_gamew_head_font;
extern const char bcfg_gamew_head_col[];
extern const char bcfg_gamew_timer_col[];
extern const int bcfg_pole_x_min;
extern const int bcfg_pole_y_min;
extern const int bcfg_pole_x;
extern const int bcfg_pole_y;
extern const char bcfg_gamew_grid_col[];
extern const char bcfg_gamew_kontur_col[];
extern const char bcfg_gamew_cursor_col[];

extern const unsigned int bcfg_mmenu_font;
extern const char bcfg_mmenu_col[];
extern const char bcfg_mmenu_bg1_col[];
extern const char bcfg_mmenu_bg2_col[];

extern const unsigned int bcfg_gmenu_font;
extern const char bcfg_gmenu_col[];
extern const char bcfg_gmenu_bg1_col[];
extern const char bcfg_gmenu_bg2_col[];

extern const unsigned int bcfg_smenu_font;
extern const char bcfg_smenu_col[];
extern const char bcfg_smenu_bg1_col[];
extern const char bcfg_smenu_bg2_col[];

extern const unsigned int bcfg_mmenu_head_font;
extern const char bcfg_mmenu_head_col[];

extern const unsigned int bcfg_mmenu_sk_font;
extern const char bcfg_mmenu_sk_col[];

extern const unsigned int bcfg_help_font;
extern const char bcfg_help_col[];
extern const unsigned int bcfg_help_scroll;

extern const int bcfg_sys_check_ena;
extern const int bcfg_sys_logo_ena;
extern const int bcfg_sys_clear_ena;
extern const int bcfg_illum_ena;
extern const unsigned int bcfg_illum_proc;

extern const int bcfg_key_control;
extern const int bcfg_key_5;

//(C)opy(R)ights
#define ELF_AUTHOR "Dimadze"
#define ELF_RESPEKT "team-sc.ru"
#define ELF_NAME "SuDoKu"
#define ELF_VERSION "1.0 beta"

//Draws
#define SCRH ScreenH()
#define SCRW ScreenW()

#define TIMER_SECOND 216

//Keys
#define KEYMSG  msg->gbsmsg->msg
#define KEYSMSG msg->gbsmsg->submess



#include "engine\fileop\elffolder.h"
#include "engine\misc\functions.h"


#include "engine\render\logo.h"
#include "engine\render\game.h"
#include "engine\render\gmenu.h"

#include "engine\render\mmenu.h"

#include "engine\fileop\fileman.h"

#include "engine\fileop\png_clean.h"
#include "engine\fileop\sdk_work.h"
#include "engine\fileop\load_save.h"




const int minus11=-11;
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

//Глобальные переменные
GBSTMR timer;
char t[6]="%t";
int key5 = 0;

//Пути к папкам
char elfpath[256];
char filepath[256];

char elffolder[256];
char imgfolder[256];
char cfgfolder[256];
char sndfolder[256];
char sdkfolder[256];

//Пути фаилов
char logo[256],sudoku[256],bgg[256],bgm[256];
char dig[30][256]; //циферки

//Игровое окно
int xc = 4,yc = 4;

//Рендеринг
int logow=1;
int mmenu=0;
int gamew=0;
int gmenu=0;
int winw=0;
int mmenu_ng=0;
int mmenu_lg=0;
int mmenu_sg=0;
int mmenu_re=0;
int mmenu_hp=0;
int mmenu_ab=0;
int fileman=0;

int sm;
int gm;
int mm;
int lm;

int mmenuc;
int smenuc;
int gmenuc;


int gcell[9][9];
int rcell[9][9];

char bcfg_path[256];

char sudokuname[32];
char level;

char timesave[5][20];
char loadname[5][32];

extern char buffer[];

extern int prev_press, prev;

//Records
int mintime;

//load vars
int loadvar=0;

//Logo vars
int imgerrors;
int logovar=0;
int pngsizeerr=0;
int bgsizeerr=0;
int sudsizeerr;
int suh;

//Help vars
int yhlp = 0;
int helpvar = 0;
int helphight;


//function
extern void getTSLN();
extern int checkhelp();






void zeroHelp()
{
 yhlp = 0;
 helpvar = 0;
}


void clearSudoku()
{
  for (int i = 0; i < 9; i ++) for (int k = 0; k < 9; k ++) {gcell[i][k]=0; rcell[i][k]=0;}
  for (int i = 0; i < 32;i++) sudokuname[i]=0;
  for (int i = 0; i < 256; i++) filepath[i]=0;
  level = 0;
}




void IllumControl() //Функция снова включает подсветку, если она потухла (Яркость подсветки зависит от активного профиля) Спасибо RevolteD за пример.
{
  if (bcfg_illum_ena==1) { if (!*(GetIlluminationDataTable()+3)) SetIllumination(0,1,(short)bcfg_illum_proc,0);}
}

void OpenConfig() 
{
     WSHDR *config_path = AllocWS(256);
     wsprintf(config_path, bcfg_path);//Или на 0:/
     ExecuteFile(config_path,0,"");
     FreeWS(config_path);
}

static void OnRedraw(MAIN_GUI *data)//эта функция нужна для перерисовки экрана,тут рисуем все что нам нужно
{
  IllumControl();
  
  if (logow == 1) LogoRender();
  
  if (gamew==1 || gmenu==1) GameRender();
  if (gmenu==1) GMenuRender();
  
  
   if (mmenu==1) MMenuRender();
   if (mmenu_ng==1) MMenuRender_NewGame();
   if (mmenu_lg==1) MMenuRender_LoadGame();
   if (mmenu_sg==1) MMenuRender_SaveGame();
   if (mmenu_re==1) MMenuRender_Records();
   if (mmenu_ab==1) MMenuRender_About();
   if (mmenu_hp==1) MMenuRender_Help();
   
   if (winw==1) WinerWindowRender();
 
  GBS_StartTimerProc(&timer,20,OnRedraw);
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  
  if (ScreenH()>300) DisableIconBar(1);
  GBS_StartTimerProc(&timer,20,OnRedraw);
  data->gui.state=1;
  
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (ScreenH()>300) DisableIconBar(0);
  GBS_DelTimer(&timer); 
  data->gui.state=0;
  if (bcfg_sys_clear_ena==1) clear_cache();
}

static void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))//Если гуи на верху то опять же делаем что нужно
{
  if (ScreenH()>300) DisableIconBar(1);
  
  data->gui.state=2;
  DisableIDLETMR();//отключаем таймер,иначе через 2 минуты эльф закроется
}

static void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))//а если гуи не вверху,то можно закрыть эльф например:)
{
  if (ScreenH()>300) DisableIconBar(0);
  GBS_StopTimer(&timer);
  if (data->gui.state!=2) return;
  data->gui.state=1;
}


static int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  
  if (KEYMSG==LONG_PRESS && KEYSMSG==RED_BUTTON) goto end;
  
  
  if (KEYMSG==KEY_DOWN || KEYMSG==LONG_PRESS)
  {
    if (gamew==1 && bcfg_key_control==0){
    switch(KEYSMSG)
    {
  
    case '*': if (gcell[yc][xc]<20) gcell[yc][xc]=rcell[yc][xc]; break;
      
    case '1': if (key5==1) gcell[yc][xc]=11; key5=0; break;
    case '3': if (key5==1) gcell[yc][xc]=13; key5=0; break;
    case '7': if (key5==1) gcell[yc][xc]=17; key5=0; break;
    case '9': if (key5==1) gcell[yc][xc]=19; key5=0; break;
      
    case '2': case UP_BUTTON:     if (key5==0) {if (yc>0) yc--;else yc=8;} else gcell[yc][xc]=12; key5=0; break;
    case '8': case DOWN_BUTTON:   if (key5==0) {if (yc<8) yc++;else yc=0;} else gcell[yc][xc]=18; key5=0; break; 
    case '4': case LEFT_BUTTON:   if (key5==0) {if (xc>0) xc--;else xc=8;} else gcell[yc][xc]=14; key5=0; break;
    case '6': case RIGHT_BUTTON:  if (key5==0) {if (xc<8) xc++;else xc=0;} else gcell[yc][xc]=16; key5=0; break;
    case '5':                     if (gcell[yc][xc]<10) if (key5==0) key5=1; else {gcell[yc][xc]=15; key5=0;} break;
    
    case ENTER_BUTTON:  if (bcfg_key_5==1) {if (gcell[yc][xc]<10) if (key5==0) key5=1; else {gcell[yc][xc]=15; key5=0;}} break;
    
    case RIGHT_SOFT:  {gmenu=1;gamew=0;} goto end;
    
    }}
    if (gamew==1 && bcfg_key_control==1){
    switch(KEYSMSG)
    {  
      case '*': if (gcell[yc][xc]<20) gcell[yc][xc]=rcell[yc][xc]; break;
     
      case UP_BUTTON:     if (yc>0) yc--;else yc=8; break;
      case DOWN_BUTTON:   if (yc<8) yc++;else yc=0; break; 
      case LEFT_BUTTON:   if (xc>0) xc--;else xc=8; break;
      case RIGHT_BUTTON:  if (xc<8) xc++;else xc=0; break;
      
      case ENTER_BUTTON: if (gcell[yc][xc]<10 && bcfg_key_5==1) gcell[yc][xc]=15; break;
      
      
      case '1': if (gcell[yc][xc]<10) gcell[yc][xc]=11;  break;
      case '3': if (gcell[yc][xc]<10) gcell[yc][xc]=13;  break;
      case '7': if (gcell[yc][xc]<10) gcell[yc][xc]=17;  break;
      case '9': if (gcell[yc][xc]<10) gcell[yc][xc]=19;  break;
      
      case '2': if (gcell[yc][xc]<10) gcell[yc][xc]=12;  break;
      case '8': if (gcell[yc][xc]<10) gcell[yc][xc]=18;  break; 
      case '4': if (gcell[yc][xc]<10) gcell[yc][xc]=14;  break;
      case '6': if (gcell[yc][xc]<10) gcell[yc][xc]=16;  break;
      case '5': if (gcell[yc][xc]<10) gcell[yc][xc]=15;  break;
     
             
      case RIGHT_SOFT:  {gmenu=1;gamew=0;} goto end;
     }
    }
    
    if (gmenu==1){
    switch(KEYSMSG)
       {
       case '2': case UP_BUTTON:    if (gm==0) gm=4; else gm--; break;
       case '8': case DOWN_BUTTON:  if (gm==4) gm=0; else gm++; break;                       
       case '5': case ENTER_BUTTON: if (gm==0) {gmenu=0;gamew=1;checker();}
                                    if (gm==1) {prev_press=1;resetTimer(&prev,0);gmenu=0;gamew=1;saverec();}
                                    if (gm==2) {gmenu=0;gamew=0;mmenu_lg=1;loadvar=1;getTSLN();}
                                    if (gm==3) {gmenu=0;gamew=0;mmenu_sg=1;getTSLN();}
                                    if (gm==4) {gmenu=0;gamew=0;mmenu=1;}
                                    goto end;
       case RIGHT_SOFT: gmenu=0;gamew=1; goto end;
       case RED_BUTTON: gmenu=0;gamew=1; goto end;
       }}
    
    if (mmenu==1){
    switch(KEYSMSG)
       {

       case '2': case UP_BUTTON:    if (mm==0) mm=(mmenuc - 1); else mm--; break;
       case '8': case DOWN_BUTTON:  if (mm==(mmenuc - 1)) mm=0; else mm++; break;    
       
       
       case '5': case ENTER_BUTTON:     if (mm==0) {mmenu=0;mmenu_ng=1;goto end;}
                                        if (mm==1) {mmenu=0;mmenu_lg=1;getTSLN();}
                                        if (mm==2) OpenConfig();
                                        if (mm==3) {mmenu=0;mmenu_re=1;}
                                        if (mm==4) {mmenu=0;mmenu_hp=1;}
                                        if (mm==5) {mmenu=0;mmenu_ab=1;}
                                        if (mm==6) {clear_cache();return(1);}
                                        goto end;
                                    
                                    
       case RIGHT_SOFT:            if (gcell[0][0]!=0) {mmenu=0;gamew=1; goto end;} else {open_fm();fileman=1;goto end;}
       }}
    
    if (mmenu_ng==1){
    switch(KEYSMSG)
       {
       case LEFT_SOFT:              {open_fm();fileman=1;goto end;}
       case RIGHT_SOFT:             if (filepath[1]==':') { if (check_sudoku()==1) {sdk_read(filepath);mmenu_ng=0;gamew=1;goto end;} else {clearSudoku();ShowMSG(1,(int)"Неверно\nсоставлен!");mmenu_ng=0;mmenu=1;goto end;}} else {mmenu_ng=0;mmenu=1;goto end;}                
       case RED_BUTTON:             mmenu_ng=0;mmenu=1;goto end;
       }}
    
    if (mmenu_lg==1){
    switch(KEYSMSG)
       {
       case '2': case UP_BUTTON:                     if (sm==0) sm=(smenuc - 1); else sm--; break;
       case '8': case DOWN_BUTTON:                   if (sm==(smenuc - 1)) sm=0; else sm++; break;  
       case '5': case RIGHT_SOFT: case ENTER_BUTTON: if (checkgame(sm+1)==1) {loadgame(sm + 1);mmenu_lg=0;gamew=1;}goto end;
       case LEFT_SOFT: case RED_BUTTON:               if(loadvar==0) { mmenu_lg=0;mmenu=1;goto end;} else {mmenu_lg=0;gamew=1;loadvar=0;goto end;}
       }}
    
    if (mmenu_sg==1){
    switch(KEYSMSG)
       {
       case '2': case UP_BUTTON:                     if (lm==0) lm=(smenuc - 1); else lm--; break;
       case '8': case DOWN_BUTTON:                   if (lm==(smenuc - 1)) lm=0; else lm++; break;  
       case '5': case RIGHT_SOFT: case ENTER_BUTTON: savegame(lm + 1);getTSLN();goto end;
       case LEFT_SOFT:                     mmenu_sg=0;gamew=1;goto end;                      
       case RED_BUTTON:                    mmenu_sg=0;gamew=1;goto end;
       }}
    
    
    if (mmenu_hp==1){
    switch(KEYSMSG)
       {
       case '8': case DOWN_BUTTON:                   if (yhlp>(-helphight)) yhlp-=bcfg_help_scroll; break;
       case '2': case UP_BUTTON:                     if (yhlp<0) yhlp+=bcfg_help_scroll; break;  
       case RIGHT_SOFT: case RED_BUTTON:             mmenu_hp=0;mmenu=1;zeroHelp();goto end;

       }}
    
    if (mmenu_ab==1){
    switch(KEYSMSG)
       {
       case RIGHT_SOFT: case RED_BUTTON:             mmenu_ab=0;mmenu=1;goto end;
       }}
    
    if (mmenu_re==1){
    switch(KEYSMSG)
       {
       case RIGHT_SOFT: case RED_BUTTON:             mmenu_re=0;mmenu=1;goto end;
       }}
    
     if (winw==1){
    switch(KEYSMSG)
       {
       case RIGHT_SOFT: case RED_BUTTON:             saverec(); mmenu=1; winw=0; goto end;
       }}
    
    if (logow==1){
    switch(KEYSMSG)
       {
       case RIGHT_SOFT: case RED_BUTTON:       if (imgerrors>0 || sudsizeerr==1 || pngsizeerr==1 || bgsizeerr==1 || checkhelp()==1) return(1);
       }}
   
    
    end : ;
  }
  
 return(0); 
}



extern void kill_data(void *p, void (*func_p)(void *));//Эта функция убивает эльф и освобождает память

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

static void maincsm_oncreate(CSM_RAM *data)//Вызывается при открытии эльфа,делаем что нужно
{
  static const RECT Canvas={0,0,0,0};
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  csm->gui_id=CreateGUI(main_gui);
}

void ElfKiller(void)//освобождаем все занятое место эльфом в оперативе
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}


static void maincsm_onclose(CSM_RAM *csm)//функция закрытия эльфа,тут освобождаем памчть если нужно и т.п.
{
  SUBPROC((void *)ElfKiller);
}


#pragma inline=forced
int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)//тут вообщем протекают все сообщения поступаемые из эльфа так сказать:)
{
  //сообщение о реконфигурации например,ипц и т.д. и т.п.
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }
  if (msg->msg==MSG_RECONFIGURE_REQ)//Елси была реконфигурация,то обновляем настройки эльфа
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"Реконфиг!");
      InitConfig();
    }
  }

  return(1);
}


static const struct //это структура цсм,разбираться не надо:)
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




void UpdateCSMname(void)//ну собственно имя эльфа передаваемое хтаску
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),t,ELF_NAME);
}

int main(char *exename, char *fname)//exename-путь запускаемого эльфа, fname параметр передаваемый эльфу
{
  #ifdef SGOLD
  if (isnewSGold()!=0) ShowMSG(1,(int)"Данный эльф\nдля SGOLD!");
  if (isnewSGold()==0){
  #endif
  
  #ifdef NEWSGOLD
  #ifndef ELKA
  if (isnewSGold()!=1) ShowMSG(1,(int)"Данный эльф\nдля NEWSGOLD!");
  if (isnewSGold()==1){
  #endif
  #endif
    
  #ifdef ELKA
  if (isnewSGold()!=2) ShowMSG(1,(int)"Данный эльф\nдля E/EL71!");
  if (isnewSGold()==2){
  #endif
    
  
  clearSudoku();
  
  sprintf(elfpath,exename);
  sprintf(filepath,fname);
  ElfFolderOperations();
  
  
  
  InitConfig();
  if (bcfg_sys_check_ena==1) ElfFolderCheck();
  
  MAIN_CSM main_csm;
  LockSched();
  UpdateCSMname();
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);//создаем цсм
  UnlockSched();
  
  }
  return 0;
}
