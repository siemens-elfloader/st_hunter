#include "..\inc\swilib.h"
#include "../inc/cfg_items.h"
#include "conf_loader.h"
#include "main.h"
#include "strings.h"
#include "fileman.h"
#include "bookmarks.h"
//Исходники кривые, над ними еще работать и работать...а впадлу...
//[*]Если нажать на сам эльф то пикоффа не будет и откроется меню "Последние..." :)
//[+]Если размер файла нулевой то тоже откроется меню "Последние...". Если же эдитор запускается первый раз,то откроется меню.
int maincsm_id;
int maingui_id;
const int minus11=-11;

unsigned short maincsm_name_body[140];

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

int my_csm_id=0;

//----------------------------------//

extern const int screen_mode;
extern const int size_font;
extern const int pen_font;
extern const int brush_font;
extern void InitConfig();
extern int codepage;//Кодировка
extern const int illum_mode;//постоянна подсветка али нет...
extern const int backup;
//extern const int codepg;//кодировка по умолчанию
//extern const int menu_codepg;
extern const int autosave;
extern const int scin_mode;//Юзать цвет,размер шрифта из скина...
extern const char IMG_PATH[128];

//----------------------------------//

extern int last_id;//ид для меню посление...
extern void LoadINI();
extern void SaveINI();

//----------------------------------//

EDITCONTROL ec;
WSHDR *ws_edit = NULL;

int id_ed;//id drawtext
int id_ed2;//id ShowCodePage
extern int MenuCode_id;

int codepage;
int curpos=1;//Позиция курсора
int total_symb=0;//общее количество символов
int file_size;
int f;
GBSTMR tmr;

char *file_name;//Показываемое имя файла в заголовке окна,используется для истории и еще чего та =)
char *file;
char info[1024];//инфа по зеленой кнопке...
const char per_d[]="%d";
const char per_t[]="%t";
const char per_s[]="%s";
const char empty_str[]="";
char exe_name[128];//сохраняем путь эльфа для запуска второй копии из меню "последние..." по зеленой кнопке

//----------------------------------//

#pragma inline
void patch_header(const HEADER_DESC* head)
{
  ((HEADER_DESC*)head)->rc.x=0;
  ((HEADER_DESC*)head)->rc.y=YDISP;
  ((HEADER_DESC*)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC*)head)->rc.y2=HeaderH()+YDISP-1;
}

#pragma inline
void patch_input(const INPUTDIA_DESC* inp)
{
  ((INPUTDIA_DESC*)inp)->rc.x=0;
  ((INPUTDIA_DESC*)inp)->rc.y=HeaderH()+1+YDISP;
  ((INPUTDIA_DESC*)inp)->rc.x2=ScreenW()-1;
  ((INPUTDIA_DESC*)inp)->rc.y2=ScreenH()-SoftkeyH()-1;
}

#pragma inline
void patch_header_n(HEADER_DESC* head)
{
  head->rc.x=0;
  head->rc.y=0;
  head->rc.x2=0;
  head->rc.y2=0;
}

#pragma inline
void patch_input_n(INPUTDIA_DESC* inp)
{
  inp->rc.x=0;
  inp->rc.y=0;
  inp->rc.x2=ScreenW()-1;
  inp->rc.y2=ScreenH()-SoftkeyH()-1;

}

#pragma inline
void patch_rect(RECT*rc,int x,int y, int x2, int y2)
{
  rc->x=x;
  rc->y=y;
  rc->x2=x2;
  rc->y2=y2;
}

CSM_DESC icsmd;

//------------------------------------//

void on_utf8ec(USR_MENU_ITEM *item)
{
  if (item->type==0)
  {
    switch(item->cur_item)
    {
    case 0: wsprintf(item->ws,per_t,LG_SAVE); break;
    }
  }
  if (item->type==1)
  {
    switch(item->cur_item)
    {
      case 0: SaveFile(file); break;
    }
  }   
}

//-------------------------------Подсветка...---------------------------------//

void illum()
{
 short light;
 char *ram=(char*)0xA8000000;
 long a;
 unsigned char *l=(unsigned char*)0xA0FD435C; //Начальный указатель BFL(смещение)  
 a=l[2]*0x10000+l[1]*0x100+l[0];
 light=ram[a];
 SetIllumination(0,1,light,256);
 GBS_StartTimerProc(&tmr,216,illum);
}

int pos_onkey(GUI *data, GUI_MSG *msg)
{
  if (msg->keys==0x0018)//а вот тут обрабатываем нашу кнопку
  {
    
  }
return(0);
}

void pos_ghook(GUI *gui, int cmd)
{ 
  static SOFTKEY_DESC sk={0x0018,0x0000,(int)"Go!"};//обозначаем нашу кнопку
  if (cmd==7)
  {
    SetSoftKey(gui,&sk,SET_SOFT_KEY_N);//устанавливаеем ее на наш софт, SET_SOFT_KEY_N , из свилиб.х
  }
  if(cmd==0xA)
  {
    DisableIDLETMR(); // Отключаем таймер выхода по таймауту
  }
  if (cmd==0x0C)
  {
    EDIT_SetCursorPos(gui,1);
  }
  if(cmd==0x03){}
}
//--------------------------------GO TO LINE----------------------------------//

HEADER_DESC pos_hdr={0,0,131,21,NULL,0,LGP_NULL};

int pos_onkey(GUI *data, GUI_MSG *msg)
{
  WSHDR *ws=AllocWS(64);
  char num[10];
  if (msg->keys==0x0018)//0xFFF
  {
    EDITCONTROL ec;
    ExtractEditControl(data,1,&ec);	
    ws=ec.pWS;
    ws_2str(ws,num,9);
    curpos=atoi(num);
    curpos*=20;//пока будем считать что в строке 20 символов...не так важно пока что...
    GeneralFunc_flag1(id_ed2,1);
    DrawText(file,codepage);    
  }
return(0);
}

void pos_ghook(GUI *gui, int cmd)
{ 
  static SOFTKEY_DESC sk={0x0018,0x0000,(int)"Go!"};
  if (cmd==7)
  {
    SetSoftKey(gui,&sk,SET_SOFT_KEY_N);
  }
  if(cmd==0xA)
  {
    DisableIDLETMR(); // Отключаем таймер выхода по таймауту
  }
  if (cmd==0x0C)
  {
    EDIT_SetCursorPos(gui,1);
  }
  if(cmd==0x03){}
}

void pos_locret(void){}

SOFTKEY_DESC menu_sk[]=
{
  {0x0018,0x0000,(int)"Лев"},
  {0x0001,0x0000,(int)"Прав"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB menu_skt=
{
  menu_sk,0
};

INPUTDIA_DESC pos_desc=
{
  1,
  pos_onkey,
  pos_ghook,
  (void *)pos_locret,
  0,
  &menu_skt,
  {0,NULL,NULL,NULL},
  FONT_SMALL,
  100,
  101,
  0,
  0x00000000,
  0x40000000
};


void pos()
{
   char *ss=malloc(64);
   sprintf(ss,per_s,LG_LINE);
   pos_hdr.lgp_id = (int)ss;
   EDITC_OPTIONS ec_options2;
   void *ma=malloc_adr();
   void *eq=AllocEQueue(ma, mfree_adr());

   PrepareEditControl(&ec);
   ConstructEditControl(&ec,ECT_NORMAL_NUM,ECF_APPEND_EOL|ECF_DISABLE_MINUS|ECF_DISABLE_POINT,ec.pWS,9);
   PrepareEditCOptions(&ec_options2);
   SetFontToEditCOptions(&ec_options2,1);//Шрифт
   CopyOptionsToEditControl(&ec,&ec_options2);//Копируем настройки
   AddEditControlToEditQend(eq,&ec,ma);

   patch_header2(&pos_hdr);
   patch_input2(&pos_desc);      
 
   GeneralFuncF1(1);
   mfree_adr();
   mfree(ss);
   id_ed2=CreateInputTextDialog(&pos_desc,&pos_hdr,eq,1,0);//создаем диалоговое окно
}

//--------------------------------DRAW && EDIT TEXT---------------------------//

char hdr_txt[64];
HEADER_DESC disk_prop_hdr={0,0,NULL,NULL,NULL,(int)hdr_txt,LGP_NULL};
int empty_onkey(GUI *data, GUI_MSG *msg);
void empty_locret(void){};
void empty_ghook(GUI *gui, int cmd);

SOFTKEY_DESC sk[]=
{
  {0x0018,0x0000,(int)"Лев"},
  {0x0001,0x0000,(int)"Прав"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB skt=
{
  sk,0
};

INPUTDIA_DESC edit_desc=//Редактирование
{
  1,
  empty_onkey,
  empty_ghook,
  (void *)empty_locret,
  0,
  &skt,
  {0,0,0,0},
  FONT_SMALL,
  100,
  101,
  0,
  0,
  0x40000000
};

int empty_onkey(GUI *data, GUI_MSG *msg)
{ 
  if (msg->keys==0xFFF)
  {
    ExtractEditControl(data,1,&ec);	
    wstrcpy(ws_edit,ec.pWS);//сохраняем измененный текст в ws_edit на случай последующего сохранение в файл :)
    ShowMainMenu();
  }
  
  if (msg->gbsmsg->msg==KEY_DOWN)
  { 
   curpos=EDIT_GetCursorPos(data)-1;
   switch(msg->gbsmsg->submess)
   {    
    case GREEN_BUTTON:
      {
//         SaveINI();
        sprintf(info,"%s%s %s%d b%s %d/%d","Файл:\n",file,"\nРазмер:",file_size,"\nПозиция:",curpos,total_symb);
        ShowMSG(2,(int)info);
      } break;
    case RED_BUTTON:
      GeneralFunc_flag1(id_ed,1);
      GeneralFunc_flag1(MenuCode_id,1);
      GeneralFunc_flag1(last_id,1);
      break;
    case VOL_UP_BUTTON: curpos-=50; EDIT_SetCursorPos(data,curpos); DirectRedrawGUI(); break;
    case VOL_DOWN_BUTTON: curpos+=50; EDIT_SetCursorPos(data,curpos); DirectRedrawGUI(); break;
    case ENTER_BUTTON:
       ExtractEditControl(data,1,&ec);	
       wstrcpy(ws_edit,ec.pWS);//сохраняем измененный текст в ws_edit на случай последующего сохранение в файл :)
       EDIT_OpenOptionMenuWithUserItems(data,on_utf8ec,data,1);
       return (-1);
   }
  }
  
  if (msg->gbsmsg->msg==LONG_PRESS)
  {  
    curpos=EDIT_GetCursorPos(data);
    switch(msg->gbsmsg->submess)
    {
     case VOL_UP_BUTTON:   curpos-=50; EDIT_SetCursorPos(data,curpos); DirectRedrawGUI(); break;
     case VOL_DOWN_BUTTON: curpos+=50; EDIT_SetCursorPos(data,curpos); DirectRedrawGUI(); break;
    }
  }  
return(0);
}

void empty_ghook(GUI *gui, int cmd)
{ 
  static SOFTKEY_DESC sk2={0x0FFF, 0x0000,(int)LG_MENU};
  if (cmd==7)
  {
    SetSoftKey(gui,&sk2,SET_SOFT_KEY_N);
  }

  if(cmd==0xA)
  {
    DisableIDLETMR();   // Отключаем таймер выхода по таймауту
  }
  if (cmd==0x0C)
  {  
    EDIT_SetCursorPos(gui,curpos);    
  }
  if(cmd==0x03){}

};

//----------------------------------//

void SaveFile(char *filename)
{
  unsigned int err;
  int f;
  char *s=malloc(wstrlen(ws_edit));
  int len;
  int i;
  len=ws_edit->wsbody[0];
  for(i=0;i<len;i++)//Сохранение русского языка....
    *(s+i)=char16to8(ws_edit->wsbody[i+1]);
    *(s+i++)=0;
  if(backup)
  {
    char ff[128];
    strcpy(ff,file);
    strcat(ff,".bkp");
    fmove(file,ff,&err);//переименовываем
  }
  unlink(file,&err);
  if((f=fopen(file,A_Create+A_ReadWrite/*+A_Append*/+A_BIN,P_WRITE,&err))!=-1)
   {
     fwrite(f,s,strlen(s),&err);
     ShowMSG(1,(int)LG_FILESAVE);
   }
  else ShowMSG(1,(int)LG_NOTSAVE);
  fclose(f,&err);
  mfree(s);
}

//----------------------------------//

int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}

//----------------------------------//
void Killer(void);
int DrawText(char *filename,int cp)//Cобственно и вызов самого эдитконтроля:)
{
   if(illum_mode) illum();//Подсветка постоянна или нет
   else GBS_DelTimer(&tmr);
   
   file_name=GetName(file,1);//Понятно из названия...получаем имя файла...:)
   strcpy(file,filename);//Чтоб при сохранении не было недоразумений с именем....

   unsigned int err;
   EDITC_OPTIONS ec_options;
   PrepareEditControl(&ec);
   void *ma=malloc_adr();
   void *eq=AllocEQueue(ma, mfree_adr());
   unsigned int size=0;
   
   if((file_size=get_file_size(filename))==0)//Если размер файла равен нулю то выходим!
   {
      ShowMSG(1,(int)"Zero lenght file!");
      GeneralFunc_flag1(id_ed,1);
      GeneralFunc_flag1(MenuCode_id,1);
      GeneralFunc_flag1(last_id,1);
      ShowMainMenu();
      return 1;
   }
   
   char *s=malloc(file_size);
   if((f=fopen(filename,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
       size=fread(f,s,file_size,&err);
   fclose(f,&err);

   ws_edit=AllocWS(size);//Выделяем память столько,сколько прочитали данных из файла

   switch(cp)//выбор кодировки
    {
      case 0: win1251_2ws(ws_edit,s);break;
      case 1: utf8_2ws(ws_edit,s,file_size); break;
      case 2: koi2ws(ws_edit,s); break;
      case 3: ascii2ws(ws_edit,s); break;
      case 4: win1250_2ws(ws_edit,s); break;
    }
   
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_NORMAL_STR,ws_edit,size*2);//умножаем на 2 чтобы было место для редактирования...
 
  if(scin_mode)//Исполузуем цвет и шрифт от скина
  {
    SetFontToEditCOptions(&ec_options,size_font);
    SetPenColorToEditCOptions(&ec_options,pen_font);
    SetBrushColorToEditCOptions(&ec_options,brush_font);
    CopyOptionsToEditControl(&ec,&ec_options);
  }
  AddEditControlToEditQend(eq,&ec,ma);//Добавляем в эдитконтрол
  
  if(screen_mode)//Показывать заголовок :)
  {    
    extern const char per_2s[];
  //  snprintf(s,127,per_2s,IMG_PATH,"\\null.png");
   // *((int **)(&disk_prop_hdr.icon))=(int *)s;
    sprintf(hdr_txt," %s",file_name);
    patch_header(&disk_prop_hdr);
    patch_input(&edit_desc);     
  }
  else//или нет)
  {
    patch_header_n(&disk_prop_hdr);
    patch_input_n(&edit_desc);      
  }
  mfree(s);
  return id_ed=CreateInputTextDialog(&edit_desc,&disk_prop_hdr,eq,1,0);
  //сохраняем ид для последующих махинаций)))
}


//-------------------------------Create new file------------------------------//


int new_file_id;
void CreateNewFile(char *name,int size)
{
  int f;
  unsigned int err;
  if((f=fopen(name,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&err))!=-1)
    {
      fwrite(f,"",size,&err);
      fclose(f,&err);
      DrawText(name,0);
    }
}

HEADER_DESC new_file_hdr={0,0,131,21,NULL,(int)LG_NEW_FILE,LGP_NULL};

int new_file_onkey(GUI *data, GUI_MSG *msg)
{
  WSHDR *ws=AllocWS(64);
  char name[32];
  int size;
  char s[9];
  if (msg->keys==0x0FFF)//0xFFF
  {
    EDITCONTROL ec;
    ExtractEditControl(data,2,&ec);	
    ws=ec.pWS;
    ws_2str(ws,name,32);//Извлекаем имя
    
    ExtractEditControl(data,4,&ec);//извлекаем размер
    ws=ec.pWS;
    ws_2str(ws,s,32);
    size=atoi(s);
    
    CreateNewFile(name,size);
    GeneralFunc_flag1(new_file_id,1);
  }
return(0);
}

void new_file_ghook(GUI *gui, int cmd)
{ 
  static SOFTKEY_DESC sk={0x0FFF,0x0000,(int)LG_CREATE};
  if (cmd==7)
  {
    SetSoftKey(gui,&sk,SET_SOFT_KEY_N);
  }
  if(cmd==0xA)
  {
    DisableIDLETMR(); // Отключаем таймер выхода по таймауту
  }
  if (cmd==0x0C)
  {
    EDIT_SetCursorPos(gui,1);
  }
  if(cmd==0x03){}
}

void new_file_locret(void){}

SOFTKEY_DESC new_file_sk[]=
{
  {0x0018,0x0000,(int)"Лев"},
  {0x0001,0x0000,(int)"Прав"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB new_file_skt=
{
  menu_sk,0
};

INPUTDIA_DESC new_file_desc=
{
  1,
 new_file_onkey,
  new_file_ghook,
  (void *)new_file_locret,
  0,
  &menu_skt,
  {0,NULL,NULL,NULL},
  FONT_SMALL,
  100,
  101,
  0,
  0x00000000,
  0x40000000
};

void new_file_ec()
{
   char *ss=malloc(64);
   WSHDR *wss=AllocWS(64);

   void *ma=malloc_adr();
   void *eq=AllocEQueue(ma, mfree_adr());

   PrepareEditControl(&ec);
   wsprintf(wss,per_t,"Имя:\n");
   ConstructEditControl(&ec,ECT_HEADER,ECF_NORMAL_STR,wss,12);
   AddEditControlToEditQend(eq,&ec,ma);  
#ifdef NEWSGOLD
   wsprintf(ec.pWS,per_s,"4:\\Temp\\");
#else
   wsprintf(ec.pWS,per_s,"0:\\Temp\\");
#endif
   ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_NORMAL_STR,ec.pWS,256);
   AddEditControlToEditQend(eq,&ec,ma);
   
   PrepareEditControl(&ec);
   wsprintf(wss,per_t,"\nРазмер:\n");
   ConstructEditControl(&ec,ECT_HEADER,ECF_NORMAL_STR,wss,12);
   AddEditControlToEditQend(eq,&ec,ma);  
   ConstructEditControl(&ec,ECT_NORMAL_NUM,ECF_APPEND_EOL|ECF_DISABLE_MINUS|ECF_DISABLE_POINT,ec.pWS,9);
   AddEditControlToEditQend(eq,&ec,ma);

   patch_header2(&new_file_hdr);
   patch_input2(&new_file_desc);      
 
   GeneralFuncF1(1);
   mfree(ss);
   new_file_id=CreateInputTextDialog(&new_file_desc,&new_file_hdr,eq,1,0);//создаем диалоговое окно
}


//-----------------------------------------------------------------------------//


const RECT Canvas={0,0,131,172};

void Killer(void)
{
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
}

void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  csm->gui_id=ShowMenuCode();
}   

void maincsm_onclose(CSM_RAM *csm)
{
  FreeWS(ws_edit);
  GBS_DelTimer(&tmr);
  SaveINI();
  SUBPROC((void *)Killer);
}
#define idlegui_id(icsm) (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])

#pragma inline=forced
int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}
#pragma inline
int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if (msg->msg==MSG_GUI_DESTROYED)
  {
    if ((int)msg->data0==csm->gui_id)
    {
      {
	csm->csm.state=-3;
      }
    }
  }
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      InitConfig();
      GeneralFunc_flag1(id_ed,1);
      DrawText(file,codepage);
      ShowMSG(1,(int)"TextReader config updated!");
    }
  } 
  return(1);
}

unsigned short maincsm_name_body[140];

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

WSHDR *ws_name;

void UpdateCSMname(void)
{ 
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"TR: %w",ws_name);
}

int main(char *exename, char *fname)
{
  InitConfig();
/*  if(!(file_size=get_file_size(fname)))//Если размер файла равен нулю то выходим!
   {
      if(LoadHist()) ShowMenuLast();//Если есть файлы истории
      else ShowMainMenu();//если нет,то окрываем главное меню)
      return 1;
   }*/
  file=malloc(128);
  if (fname)
  {
   if (strlen(fname)<128)
   {
     strcpy(file,fname);
     strcpy(exe_name,exename);//сохраняем путь эльфа для запуска второй копии из меню "последние..." по зеленой кнопке
     
     file_name=GetName(file,1);
  
     ws_name=AllocWS(strlen(file_name));
     wsprintf(ws_name,per_s,file_name);
     
     LoadINI(); 
   }
  }
  else
  {
//    if(LoadHist()) ShowMenuLast();//Если есть файлы истории
  //  else
    ShowMainMenu();//если нет,то окрываем главное меню)
  }
    
  MAIN_CSM main_csm;
  UpdateCSMname();
  LockSched();
  maincsm_id=CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  UnlockSched();
  return 0;
}


