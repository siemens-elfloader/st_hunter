#include "..\inc\swilib.h"
#include "local_ipc.h"
#include "../inc/xtask_ipc.h"
#include "main.h"

//#define me
int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
void (*old_icsm_onClose)(CSM_RAM*);
/*
int (*old_ed_onkey)(GUI *gui, GUI_MSG *msg);
void (*old_ed_ghook)(GUI *gui, int cmd);
void (*old_ed_redraw)(void *data);*/


extern void kill_data(void *p, void (*func_p)(void *));

#pragma segment="ELFBEGIN"
void ElfKiller(void)
{
  kill_data(__segment_begin("ELFBEGIN"),(void (*)(void *))mfree_adr());
}

const char ipc_my_name[]=IPC_TEXTINFO_NAME;
const IPC_REQ gipc={
  ipc_my_name,
  ipc_my_name,
  NULL
};

#define TMR_SECOND 216
GBSTMR mytmr;

int number_book=0;//количечтво закладок
unsigned int *ErrorNumber;
const char percent_t[]="%t";

int IsIdle(void)
{ 
  void *icsm=FindCSMbyID(CSM_root()->idle_id); 
  if (IsGuiOnTop(((int*)icsm)[DISPLACE_OF_IDLEGUI_ID/4 ])) return(1); 
  else return(0); 
}

void TimerProc(void)
{
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
}

const char *bmname(int bm)//возвращает имена для закладок
{
  switch(bm)
  {
  case 0: return BMNAME1;
  case 1: return BMNAME2;
  case 2: return BMNAME3;
  case 3: return BMNAME4;
  case 4: return BMNAME5;
  case 5: return BMNAME6;
  case 6: return BMNAME7;
  case 7: return BMNAME8;
  case 8: return BMNAME9;
  case 9: return BMNAME10;
  case 10: return BMNAME11;
  case 11: return BMNAME12;
  case 12: return BMNAME13;
  case 13: return BMNAME14;
  case 14: return BMNAME15;
  }
  return(0);
}

const char *description(int bm)//Описание для закладок
{
  switch(bm)
  {
  case 0: return description1;
  case 1: return description2;
  case 2: return description3;
  case 3: return description4;
  case 4: return description5;
  case 5: return description6;
  case 6: return description7;
  case 7: return description8;
  case 8: return description9;
  case 9: return description10;
  case 10: return description11;
  case 11: return description12;
  case 12: return description13;
  case 13: return description14;
  case 14: return description15;
  }
  return(0);
}

const char *bmfile(int bm)//вызов файлов
{
  switch(bm)
  {
  case 0: return BM1FILE;
  case 1: return BM2FILE;
  case 2: return BM3FILE;
  case 3: return BM4FILE;
  case 4: return BM5FILE;
  case 5: return BM6FILE;
  case 6: return BM7FILE;
  case 7: return BM8FILE;
  case 8: return BM9FILE;  
  case 9: return BM10FILE; 
  case 10: return BM11FILE;
  case 11: return BM12FILE;
  case 12: return BM13FILE;
  case 13: return BM14FILE;
  case 14: return BM15FILE;
  }
  return(0);
}

//------------------------------------------------------------------------//

void bm_menu_iconhnd(void *data, int curitem, void *unk);
const HEADER_DESC bm_menuhdr={0,0,131,21,NULL,(int)"Bookmarks",LGP_NULL};
const int menusoftkeys[]={0,1,2};

const SOFTKEY_DESC menu_sk2[]=
{
  {0x0004,0x0000,(int)"Config"},
  {0x0001,0x0000,(int)"Close"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menu_skt2=
{
  menu_sk2,0
};

void bm_menu_ghook(void *data, int cmd){}
int bm_menu_onkey2(void *data, GUI_MSG *msg);

const ML_MENU_DESC bmmenu=//Bookmarks
{
  8,bm_menu_onkey2,bm_menu_ghook,NULL,
  menusoftkeys,
  &menu_skt2,
  0x11,//0x1,
  bm_menu_iconhnd,
  NULL,   //Items
  NULL,   //Procs
  0,   //n
  1  
};

#pragma inline=forced
int toupper(int c)
{
	if ((c>='a')&&(c<='z')) c+='A'-'a';
	return(c);
}

int strncmpNoCase(const char *s1,const char *s2,unsigned int n)
{
	int i;
	int c;
	while(!(i=(c=toupper(*s1++))-toupper(*s2++))&&(--n)) if (!c) break;
	return(i);
}

#ifdef NEWSGOLD
unsigned int GetFunctionPointByName(char *name)
{
	char *p=(char *)FUNC_PT_START;
	int len=strlen(name);
	if(len==0)
		return 0;
	while((char *)FUNC_PT_END-p>0)
	{
		if(!strncmpNoCase(p, name, len))
			return (*(unsigned int *)(p+FUNC_PT_OFFSET));
		p+=FUNC_PT_PSIZE;
	}
	return 0;
}
#endif


void run_address(unsigned int address) 
{
  void (*runaddr)(void);
  runaddr=(void (*)())address;
  if (runaddr) 
  {
    runaddr();
  }
}

void run_shortcut(char *shortcut) 
{
#ifdef NEWSGOLD
   run_address(GetFunctionPointByName(shortcut));
#else
   run_address((unsigned int)GetFunctionPointer(shortcut));
#endif
}

int chr2num(char chr)
{
	if(chr>='0'&&chr<='9')
		return (chr-('0'-0));
	if(chr>='A'&&chr<='Z')
		//return (chr-0x37);
		return (chr-('A'-0xA));
	if(chr>='a'&&chr<='z')
		//return (chr-0x57);
		return (chr-('a'-0xA));
	return 0;
}

int RunBM2(int bm)
{
 const char *s=bmfile(bm);
 if((s)&&strlen(s))
 {
  if ((s[2]=='\\')&&((s[(strlen(s))-3]=='.')||//Проверяем строку на наличие символов '\\' и '.'
     (s[(strlen(s))-4]=='.')||(s[(strlen(s))-5]=='.')))
     /* Если в строке есть символы '\\' и '.', то запускаем как обычный файл*/
      {
        WSHDR *ws;
        ws=AllocWS(256);
        str_2ws(ws,s,256);
        ExecuteFile(ws,0,0);
        FreeWS(ws);
        return(1);
       }
  
  if ((s[2]!='\\')&&(s[(strlen(s))-3]!='.')&&
     (s[(strlen(s))-4]!='.')&&(s[(strlen(s))-5]!='.')&&
     ((s[0]=='a')||(s[0]=='A'))&&(s[1]=='0'))
    /* Если в строке нет символов '\\' и '.' и первые два символа А0,
       то запускаем энтрипоинт */
      {/*
        int entry;
        sscanf(s,"%08X",&entry);
        SUBPROC((void*)entry);*/
        int i;
	unsigned int addr=0;
	char *p=(char*)s;
	for(i=0;i<strlen(s);i++)
	{
           addr=chr2num(p[i])+addr*0x10;
	}
	run_address(addr);
      }
  else
  {
  	char ShortCut[32];
	char *p=(char*)s;
	int i=0;
	int j=0;
	for(;j<strlen(s);j++)
	{
          if(p[j]>='A')
	  {
	    ShortCut[i]=p[j];
	    i++;
	  }
        }
	ShortCut[i]=0;
	run_shortcut(ShortCut);
  }
  
  
   }
 return(1);
}

//------------------------------------------------------------------------//

void bm_menu_iconhnd(void *data, int curitem, void *unk)
{
  const char *s,*ss;
  WSHDR *ws;
  WSHDR *ws1;
  void *item=AllocMLMenuItem(data);
  s=bmname(curitem); 
  ss=description(curitem);
  ws1=AllocWS(128);
  if (s)
  {
    if (strlen(s))
    { 
      ws=AllocMenuWS(data,strlen(s)+128);
      wsprintf(ws,"%d.%t",curitem+1,s);
      wsInsertChar(ws,name_cl,1);//Цвет надписей
      wsInsertChar(ws,0xE008,1);//
      
      wsprintf(ws1,percent_t,ss);//Цвет описаний
      wsInsertChar(ws1,description_cl,1);//0x0002
      wsInsertChar(ws1,0xE008,1);
    }
    else
    { 
      ws=AllocMenuWS(data,10);
      wsprintf(ws,percent_t,"Ниче нет...");
      wsInsertChar(ws,0xE008,1);//Цвет надписей
      wsprintf(ws1,percent_t,"Надо задать...");//Цвет описаний
      wsInsertChar(ws1,description_cl,1);//0x0002
      wsInsertChar(ws1,0xE008,1);
    }
  }
  else
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws,"error");
    wsInsertChar(ws,0xE008,1);//Цвет надписей
  }

  switch(curitem)//Вызов иконок
  { 
  case 0:
    if(strlen(ICON1) && strlen(s))//если путь не равен 0 то рисуем иконку
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);//если пути до иконки нет то рисуем иконку NO_ICON
    break;
  case 1:
    if(strlen(ICON2) && strlen(s))
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 2:
    if(strlen(ICON3) && strlen(s))
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 3:
    if(strlen(ICON4) && strlen(s))
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 4:
    if(strlen(ICON5) && strlen(s))
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 5:
    if(strlen(ICON6) && strlen(s))
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 6:
    if(strlen(ICON7) && strlen(s))
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;    
  case 7:
    if(strlen(ICON8) && strlen(s))
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 8:
    if(strlen(ICON9) && strlen(s))
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 9:
    if(strlen(ICON10) && strlen(s))
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 10:
    if(strlen(ICON11) && strlen(s))
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 11:
    if(strlen(ICON12) && strlen(s))
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 12:
    if(strlen(ICON13) && strlen(s))
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 13:
    if(strlen(ICON14) && strlen(s))
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 14:
    if(strlen(ICON15) && strlen(s))
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;   
  }
 SetMLMenuItemText(data, item, ws, ws1, curitem);
}

//----------------------------------------------------------------//

int num_book()
{
  if(strlen(BM1FILE)) number_book=1;
  if(strlen(BM2FILE)) number_book=2;
  if(strlen(BM3FILE)) number_book=3;
  if(strlen(BM4FILE)) number_book=4;
  if(strlen(BM5FILE)) number_book=5;
  if(strlen(BM6FILE)) number_book=6;
  if(strlen(BM7FILE)) number_book=7;
  if(strlen(BM8FILE)) number_book=8;
  if(strlen(BM9FILE)) number_book=9;
  if(strlen(BM10FILE)) number_book=10;
  if(strlen(BM11FILE)) number_book=11;
  if(strlen(BM12FILE)) number_book=12;
  if(strlen(BM13FILE)) number_book=13;
  if(strlen(BM14FILE)) number_book=14;
  if(strlen(BM15FILE)) number_book=15; 
return number_book;
}

int active=0;
int id=-1;
void ShowBMmenu1(void)
{
  active=1;
  num_book();
  patch_header(&bm_menuhdr);
  id=CreateMultiLinesMenu(0,0,&bmmenu,&bm_menuhdr,0,number_book);//number_book - кол-во закладок
}

int bm_menu_onkey2(void *data, GUI_MSG *msg)//кейхук для закладок
{
  int i;
  i=GetCurMenuItem(data);
  int k=msg->gbsmsg->submess;
  int m=msg->gbsmsg->msg;
  if(m==KEY_DOWN && k==ACTIVE_KEY_1 && ACTIVE_KEY_STYLE!=2 && active && k!=UP_BUTTON && k!=DOWN_BUTTON)
  {
    active=0;
    GeneralFunc_flag1(id,1);
    return 1;  
  }
  
  if(k==LEFT_BUTTON || k==RIGHT_SOFT || k==RED_BUTTON)
  {
    active=0;
    GeneralFunc_flag1(id,1);
    return 1;  
  }
  
  if (k=='#' || k==LEFT_SOFT)// '#'
  {
    WSHDR *ws;
    ws=AllocWS(150);
    str_2ws(ws,"4:\\Zbin\\utilities\\CfgEdit.elf",128);
#ifdef me
    ExecuteFile(ws,0,"4:\\ZBin\\etc\\AltMyMenu_me.bcfg");
#else
    ExecuteFile(ws,0,"4:\\ZBin\\etc\\AltMyMenu.bcfg");
#endif
    FreeWS(ws);
    return(-1);
  }
  
  if(k=='*')
   {
    about();
    return (-1);
   }
  
  if (k=='0')
  {
    if (RunBM2(9)){active=0; return 1;}
     return(-1);
  }
  if ((k>='1')&&(k<='9'))
   { 
     if (RunBM2(k-'1')) {active=0; return 1;}
     return(-1);
   }
  
  if (msg->keys==0x3D)
  {
    if (RunBM2(i))
    {
      active=0;
      return(1);
    }
    return(-1);
  }
return(0);
}

//-------------------Вызов на идле по интер буттон-----------------//
/*
int (*old_ed_onkey)(GUI *gui, GUI_MSG *msg);
void (*old_ed_ghook)(GUI *gui, int cmd);
void (*old_ed_redraw)(void *data);

void (*old_icsm_onClose)(CSM_RAM*);

int my_ed_onkey(GUI *gui, GUI_MSG *msg)
{
  int key=msg->gbsmsg->submess;
  int m=msg->gbsmsg->msg;
  int r;
  if (IsUnlocked()&&(m==KEY_DOWN)&&(key==ENTER_BUTTON))
  {
    ShowBMmenu1();
    r=0;
  }
  else    
    r=old_ed_onkey(gui,msg);
  return(r);
}

void DoSplices(GUI *gui)
{
  static INPUTDIA_DESC my_ed;
  memcpy(&my_ed,gui->definition,sizeof(INPUTDIA_DESC));
  if (my_ed.onKey != my_ed_onkey)
  {
    old_ed_onkey=my_ed.onKey;
    my_ed.onKey=my_ed_onkey;
    gui->definition=&my_ed;
  }
}
*/
//----------------------------------------------------------------//
int mode;
int mode_enter;
int my_keyhook(int submsg, int msg)
{
  if(submsg==ACTIVE_KEY_1 && ACTIVE_KEY_STYLE!=2 && !active)
    {
      switch(msg)
      {
        case KEY_DOWN:
          if (!ACTIVE_KEY_STYLE)
          {
            if(IDLE_MODE)//Если запуск только на идле
            {
              if(ENA_LOCK)//если активировать при залоченной клаве
              {
                 if(IsIdle())//если идле то в бой
                 {
                  ShowBMmenu1();
                 }
                 else//иначе пропускаем вызов и посылаем кей ап процесс
                 {
                  GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_1);break;
                 }
              }
              else//если вызывать только при разлоченной клаве
              {
                if(IsIdle() && IsUnlocked())
                 {
                  ShowBMmenu1();
                 }
                else
                 {
                  GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_1);break;
                 }
              }
             }            
            else //вызов отовсюду
              if(ENA_LOCK) ShowBMmenu1();
              else if(IsUnlocked())
               {
                ShowBMmenu1();
               }
            return(2);
          }
          else
          {
            if (mode_enter==2)
            {
              GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_1);
              return (0);
            }
            mode_enter=0;
            return (2);
          }          
        case KEY_UP:
          if (ACTIVE_KEY_STYLE)
          {
            if (mode_enter==0)
            {
              mode_enter=2;
              GBS_SendMessage(MMI_CEPID,KEY_DOWN,ACTIVE_KEY_1);
              return (2);
            }
            if (mode_enter==2)
            {
              mode_enter=0;
              return (0);
            }
            mode_enter=0;
            return (2);
          }          
        case LONG_PRESS:
          if (ACTIVE_KEY_STYLE)
          {
            mode_enter=1;
            if(IDLE_MODE)
            {
              if(ENA_LOCK)
              {
                 if(IsIdle())
                 {
                  ShowBMmenu1();
                 }
                 else
                 {
                   GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_1);break;
                 }
              }
              else
              {
                if(IsIdle() && IsUnlocked())
                 {
                  ShowBMmenu1();
                 }
              else
                 {
                   GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_1);break;
                 }
              }
             }
            else
            {
              if(ENA_LOCK) ShowBMmenu1();
              else if(IsUnlocked())
               {
                ShowBMmenu1();
               }
            }
          }
         break;
      }
    }
  else
  {
  // * + # implementation
  if ((ACTIVE_KEY_STYLE==2) && !active)
  {
    if (msg==KEY_UP)
    {
      mode=0;
      return KEYHOOK_NEXT;
    }
    if (msg==KEY_DOWN)
    {
      switch (submsg)
      {
      case '*':
        mode=1;
        return (0);
      case '#':
        if (mode==1)
        {/*
            if(IDLE_MODE)
            {
              if(ENA_LOCK)
              {
                 if(IsIdle())
                 {
                  ShowBMmenu1();
                 }
                 else
                 {
                   mode=0;
                   GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_1);break;
                 }
              }
              else
              {
                if(IsIdle() && IsUnlocked())
                 {
                  ShowBMmenu1();
                 }
                else
                 {
                   mode=0;
                   GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_1);break;
                 }
               }
             }
            else
            {*/
              if(ENA_LOCK) ShowBMmenu1();
              else if(IsUnlocked())
                {
                 ShowBMmenu1();
                }
              else mode=0;
            //}
        }
        else { active=0; return KEYHOOK_NEXT; }
      }
     }
    }
  }
  
  
#ifdef me
   if(submsg==ACTIVE_KEY_2 && ACTIVE_KEY_STYLE_2!=2 && !active)
    {
      switch(msg)
      {
        case KEY_DOWN:
          if (!ACTIVE_KEY_STYLE_2)
          {
            if(IDLE_MODE_2)//Если запуск только на идле
            {
              if(ENA_LOCK)//если активировать при залоченной клаве
              {
                 if(IsIdle())//если идле то в бой
                 {
                  ShowBMmenu1();
                 }
                 else//иначе пропускаем вызов и посылаем кей ап процесс
                 {
                  GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_2);break;
                 }
              }
              else//если вызывать только при разлоченной клаве
              {
                if(IsIdle() && IsUnlocked())
                 {
                  ShowBMmenu1();
                 }
                else
                 {
                  GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_2);break;
                 }
              }
             }            
            else //вызов отовсюду
              if(ENA_LOCK) ShowBMmenu1();
              else if(IsUnlocked())
               {
                ShowBMmenu1();
               }
            return(2);
          }
          else
          {
            if (mode_enter==2)
            {
              GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_2);
              return (0);
            }
            mode_enter=0;
            return (2);
          }          
        case KEY_UP:
          if (ACTIVE_KEY_STYLE_2)
          {
            if (mode_enter==0)
            {
              mode_enter=2;
              GBS_SendMessage(MMI_CEPID,KEY_DOWN,ACTIVE_KEY_2);
              return (2);
            }
            if (mode_enter==2)
            {
              mode_enter=0;
              return (0);
            }
            mode_enter=0;
            return (2);
          }          
        case LONG_PRESS:
          if (ACTIVE_KEY_STYLE_2)
          {
            mode_enter=1;
            if(IDLE_MODE_2)
            {
              if(ENA_LOCK)
              {
                 if(IsIdle())
                 {
                  ShowBMmenu1();
                 }
                 else
                 {
                   GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_2);break;
                 }
              }
              else
              {
                if(IsIdle() && IsUnlocked())
                 {
                  ShowBMmenu1();
                 }
              else
                 {
                   GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_2);break;
                 }
              }
             }
            else
            {
              if(ENA_LOCK) ShowBMmenu1();
              else if(IsUnlocked())
               {
                ShowBMmenu1();
               }
            }
          }
         break;
      }
    }
  else
  {
  // * + # implementation
  if ((ACTIVE_KEY_STYLE_2==2) && !active)
  {
    if (msg==KEY_UP)
    {
      mode=0;
      return KEYHOOK_NEXT;
    }
    if (msg==KEY_DOWN)
    {
      switch (submsg)
      {
      case '*':
        mode=1;
        return (0);
      case '#':
        if (mode==1)
        {
          if(ENA_LOCK) ShowBMmenu1();
              else if(IsUnlocked())
                {
                 ShowBMmenu1();
                }
              else mode=0;
            
        }
        else { active=0; return KEYHOOK_NEXT; }
      }
     }
    }
  }
#endif
return 0;
}

//---------------------------------------------------------------------------//
/*
#pragma inline=forced
int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}*/

int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}

void maincsm_oncreate(CSM_RAM *data)
{
   GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
}

int MyIDLECSM_onMessage(CSM_RAM* data,GBS_MSG* msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data; 
  //int csm_result=0;
  //csm_result=
/*if(ACTIVE_KEY_STYLE==2) 
{
  old_icsm_onMessage(data,msg); //Вызываем старый обработчик событий

#define idlegui_id (((int *)data)[DISPLACE_OF_IDLEGUI_ID/4])  
 if (IsGuiOnTop(idlegui_id)) //Если IdleGui на самом верху
  {
    GUI *igui=GetTopGUI();
    if (igui) //И он существует (а не в проекте ;))
    {
     if(ACTIVE_KEY_STYLE==2) DoSplices(igui);//елси не тип key mode: enter button(idle)
    }
  }
}*/
 
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"AltMyMenu config updated!");
      InitConfig();
    }
  }
  
  if (msg->msg==MSG_IPC)
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
  }
  
  if (msg->msg==MSG_GUI_DESTROYED)
  {
    if ((int)msg->data0==csm->gui_id)
    {
      csm->csm.state=-3;
    }
    active=0;
    if(id) GeneralFunc_flag1(id,1);//если запущен гуй то его тоже валим
  }  
  return 1;// csm_result;  
}

void MyIDLECSM_onClose(CSM_RAM *data)
{ 
  if(id) GeneralFunc_flag1(id,1);//если запущен гуй то его тоже валим
  active=0;
  RemoveKeybMsgHook((void *)my_keyhook);
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
}

static const struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
}MAINCSM =
{
  {
  MyIDLECSM_onMessage,
  maincsm_oncreate,
#ifdef NEWSGOLD
  0,
  0,
  0,
  0,
#endif
  MyIDLECSM_onClose,
  sizeof(CSM_RAM),
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

CSM_DESC icsmd;

static void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"AltMyMenu");
}

int main()
{ 
  CSMROOT *csmr;
  CSM_RAM *save_cmpc;
  CSM_RAM main_csm;
  InitConfig();
  UpdateCSMname();
  LockSched();

  //if(ACTIVE_KEY_STYLE!=2)
    AddKeybMsgHook((void *)my_keyhook);//елси не тип key mode: enter button(idle)
 /* else
  {
    CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
    memcpy(&icsmd,icsm->constr,sizeof(icsmd));
    old_icsm_onMessage=icsmd.onMessage;
    old_icsm_onClose=icsmd.onClose;
    icsmd.onClose=MyIDLECSM_onClose;
    icsmd.onMessage=MyIDLECSM_onMessage;
    icsm->constr=&icsmd;
  }*/
  
  csmr=CSM_root();
  save_cmpc=csmr->csm_q->current_msg_processing_csm;
  csmr->csm_q->current_msg_processing_csm=csmr->csm_q->csm.first;
  my_csm_id=CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  csmr->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();

  return 0;
}
