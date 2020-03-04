
#include "lua_support.h"
#include <swilib.h>
#include "main.h"
#include "FormBuilder/LForm.h"
#include "FormBuilder/EForm.h"
#include "google.h"
#include "socket.h"
#include "lang.h"
#include "trafic.h"
#include "LangPack.h"
#include <unistd.h>

//#define TEST
//#define DEBUG

extern int uni2ansi(unsigned short*, char*, size_t);
//extern unsigned long  strtoul (const char *nptr,char **endptr,int base);
unsigned short maincsm_name_body[140];
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;
const int minus11=-11;
int my_csm_id=0;
int closeElf();
int is_use_lua = 0;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
  int i1;
}MAIN_GUI;

char dir[128];
char temp[128];

const char *ver = "1.7.2";
//class
Read * myread=0;
EForm *edit=0, *info=0;
LForm *menu=0;
LangPack *lgp;

extern "C" Google *gl;
char trans[2*1024+1]={0};
char headstring[128]={0};

int CleanEnterSpace();
/*------------------------------------------------------------------------*/
/*---------------- Весь процесс отрисовки translator`a  ------------------*/
/*------------------------------------------------------------------------*/
int scr_w,scr_h;

// ============ трафик ================

int ShowTraficDialog()
{
   traf->Show();
   return 1;
}


// ============ диалог "О программе" =============

int InfoClose()
{
   info->close();
   return 0;
}

void BuldInfoForm(EForm *ef)
{
   char *Info = new char[512];

   snprintf(Info, 512, "Google Translate Tool v%s\r\nlibrary FormBuild ver 1.4 Release\r\n(c) Z.Vova\n"
                      "Программа предназначенная для перевода через translate.google.com\n"
                      "Плюсы:\r\n"
                      " + Экономия трафика\n"
                      " + Простота использования\n"
                      " + Удобный интерфейс\n"
                      "Так же все пожелания и предложения вы можете высловить на форумах:\n"
                      "team-sc.ru и waper.ru\n"
                      "Спасибо за пользование!\n"
                      "All rights reserved. Only for siemens user :)\r\n"
                      , ver);

   ef->SetFont(4);
   ef->AddFormElement(Info, EForm :: ReadOnly, 512);
   delete Info;
}

int About()
{
   info->Show();
   return 1;
}

// ======== редактор ==========
void Edit(EForm *ef) // эта функция выполняется при создании едит контроля
{
   //ef->SetColor(0, 9);
   ef->AddFormElement(lgp->lang[EnterText].c_str(), EForm::NormalText, 256);  // обычный текст который только читается глазами
   ef->AddFormElement("", EForm::TextEnter, 2*1024);            // вводится текст максимум 2*1024 символов

   ef->AddFormElement(lgp->lang[Translate].c_str(), EForm::NormalText, 256);
   ef->AddFormElement("", EForm::TextEnter, 2*1024);

   // ПРИМЕЧАНИЕ
   //  где нить в коде, эти функции абсолютно безполезны
   //  если уж понадобится добалять пункты или удалять
   //  то юзаем:
   //  void RemoveEditControl(int item);
   //  void InsertEditControl(char *name,int item,int type, int size);
}


void OpenOptionsMenuWidthEdit(USR_MENU_ITEM *item)
{
   if (item->type==0)
   {
     switch(item->cur_item)
     {
     case 0:
        if(inputlang!=1){
            wsprintf(item->ws, "%t","ru->en");
        }
       else{
          wsprintf(item->ws, "%t","en->ru");
       }
       break;

     case 1:
       wsprintf(item->ws, "%t",lgp->lang[Clean].c_str());
       break;
     }
   }

   if (item->type==1)
   {
     switch(item->cur_item)
     {
     case 0:
        if(inputlang!=1)
        {
           inputlang = 1;
           outputlang = 1;
        } else {
           inputlang = 2;
           outputlang = 0;
        }
        myread->SetHeadString();
        break;
     case 1:
       CleanEnterSpace();
       break;
     }
   }
}


void AskExit(int type)
{
   if(!type) closeElf();
}

int EDitKey(EForm *ef, GUI *gui, GUI_MSG *msg)
{
   int key = msg->gbsmsg->submess;
   int type = msg->gbsmsg->msg;

   if(type==KEY_DOWN && key==ENTER_BUTTON)
   {
      EDIT_OpenOptionMenuWithUserItems(gui,OpenOptionsMenuWidthEdit,0,2);
      return -1;
   }

   if(msg->keys==LForm :: RIGHTSOFT && key == RIGHT_SOFT && type==KEY_DOWN)
   {
      MsgBoxYesNo(1, (int)"Ты хочеш выйти отсюдова?", AskExit);
      return -1;
   }

   if (msg->keys==LForm :: LEFTSOFT)
   {
      if(edit->IsItemEmpty(2))
         strcpy(temp, lgp->lang[Empty].c_str());
      else
         strcpy(temp, lgp->lang[Clean].c_str());

      //debug("copy status - done");
      menu->SetMitliStringText(1, temp);

      //debug("update status on menu - done");

      if(ef->IsItemEmpty(2))
      {
         //debug("empty enter line");
         strcpy(trans, lgp->lang[EnterText].c_str());
         //debug("cpy lang pack - done");
         menu->SetMitliStringText(2,trans);
         //debug("set on multi line text - done");
         menu->Show();
         //debug("menu->Show - done");
         return 0;
      }

      //debug("enter space not empty");

      WSHDR *ws = AllocWS(10);
      edit->Extract(2, ws, 9);
      zeromem(trans, 15);
      uni2ansi((unsigned short*)ws->wsbody, trans, wstrlen(ws));
      if(strlen(trans)>=10) strcat(trans, "...");

      //debug("extract from edit control - done");

      //debug("convert uni to ansi - done");

      FreeWS(ws);
      menu->SetMitliStringText(2,trans);

      //debug("set multi text - done");
      menu->Show();

      //debug("menu->Show - done");
      return (0);
   }

   if(msg->keys==LForm :: RIGHTSOFT)
   {
      return (1);
   }

   if(type == KEY_DOWN)
   {
      switch(key)
      {
         case GREEN_BUTTON:

            if(ef->IsItemEmpty(2))
            {
               ef->SetTextToItem(4, lgp->lang[PleaseEnterText].c_str());
               return -1;
            }
            WSHDR *ws = AllocWS(2*1024+1);
            ef->Extract(2, ws, 2*1024-1);
            trans[0] =0;
            uni2ansi((unsigned short*)ws->wsbody, trans, wstrlen(ws));
            if(gl->translate(trans, inlang[1][inputlang], lang[1][outputlang]))
            {
               ef->SetTextToItem(4, lgp->lang[Translating].c_str());
            }
            FreeWS(ws);

            break;
      }
   }
   return (0);
}


int CleanEnterSpace()
{
   edit->Clear(2);
   edit->Clear(4);
   edit->SetFocusTo(2);
   return 1;
}


// ========= менюха ==========

int MenuOnKey(LForm *lf, void *data, GUI_MSG *msg)
{
   int key=msg->gbsmsg->submess;

   if(key==ENTER_BUTTON || key==LEFT_SOFT)
   {
      if(lf->GetCurentMenuItem()==1)
      {
         if(edit->IsItemEmpty(2))
            {
               edit->SetTextToItem(4, lgp->lang[PleaseEnterText].c_str());
               return 1;
            }
            WSHDR *ws = AllocWS(2*1024+1);
            edit->Extract(2, ws, 2*1024-1);
            trans[0] =0;
            uni2ansi((unsigned short*)ws->wsbody, trans, wstrlen(ws));

            if(gl->translate(trans, inlang[1][inputlang], lang[1][outputlang]))
            {
               edit->SetTextToItem(4, lgp->lang[Translating].c_str());
            }
            FreeWS(ws);
            return 1;
      }else
         lf->DoCurentItemProc();
      return 1;
   }

   return 0;
}

void Read :: SetHeadString()
{
   sprintf(temp, "%sRes\\google.png", dir);
   sprintf(headstring, "%s->%s", inlang[1][inputlang].c_str(), lang[1][outputlang].c_str());
   edit->SetHeader(temp, (int)headstring);

   sprintf(temp, "%s", inlang[0][inputlang].c_str());
   menu->SetMitliStringText(3, temp);

   sprintf(temp, "%s", lang[0][outputlang].c_str());
   menu->SetMitliStringText(4, temp);
}

Read::Read()
{
    sprintf(temp, "%sgts_logfile.txt", dir);

    //qDebug::setDebugFile(temp);
    //qDebug::ClearLog();

    //qDebug::info(" ===== Phone =====\r\n [i] Siemens %s sw %s\r\n",  Get_Phone_Info(9) , Get_Phone_Info(7));

    sprintf(temp, "%sLang.lang", dir);

   lgp = new LangPack(temp);

   sprintf(temp, "%sRes\\data.inf", dir);
   int fp = _open(temp, A_ReadOnly+A_BIN,P_READ, 0);
   if(fp!=-1)
   {
      zeromem(temp, 16);
      _read(fp, temp, sizeof(int)*2, 0);
      memcpy(&inputlang, temp, sizeof(int));
      memcpy(&outputlang, temp+sizeof(int), sizeof(int));
      _close(fp,0);

      sprintf(temp, "%sRes\\data.inf", dir);
      SetFileAttrib(temp, FA_HIDDEN, 0);
   }

   Start();
   gl = new Google();
   edit = new EForm(EDitKey, 0);  // создаем наш редактор

   edit->SetHookFormBuilder(Edit);    // наша функция формо-буилдинга
   edit->ReallocEws(2*1024);
   edit->SetLeftSoft((int)lgp->lang[Options].c_str());
   edit->SetRightSoft((int)lgp->lang[Exit].c_str());
   edit->SetStandartKeyState(false);
   edit->Show();

#ifdef NEWSGOLD
   menu = new LForm(LForm::MultiLine);
#else
   menu = new LForm(LForm::Line);
#endif
   //menu->SetSmallMenu(true);

   menu->SetStandartKey(false);  // у нас свой обработчик
   sprintf(temp, "%sRes\\google.png", dir);
   menu->SetHeader(temp, (char*)-1);

   sprintf(temp, "%sRes\\clean.png", dir);
   menu->AddItemElement(temp, lgp->lang[Clean].c_str(), CleanEnterSpace);
   sprintf(temp, "%sRes\\translate.png", dir);
   menu->AddItemElement(temp, lgp->lang[DoTranslate].c_str(), 0);
   sprintf(temp, "%sRes\\SETTINGS.png", dir);
   menu->AddItemElement(temp, lgp->lang[OriginalLang].c_str(), ShowMenuInput);
   menu->AddItemElement(temp, lgp->lang[TranslateLang].c_str(), ShowMenuOutput);


   sprintf(temp, "%sRes\\traf.PNG", dir);
   menu->AddItemElement(temp, lgp->lang[Trafic].c_str(), lgp->lang[AccountingTraffic].c_str(), ShowTraficDialog);

   sprintf(temp, "%sRes\\ABOUT.PNG", dir);
   menu->AddItemElement(temp, lgp->lang[Aboute].c_str(), "Google Translator", About);
   sprintf(temp, "%sRes\\exit.png", dir);
   menu->AddItemElement(temp, lgp->lang[Exit].c_str(), lgp->lang[Completion].c_str(), closeElf);
   menu->SetKeyListener(MenuOnKey);

   SetHeadString();


   // Пункт "О программе"

   info = new EForm();  // создаем наш редактор
   info->SetShowEditControlAtrib(2); // readonly

   info->ReallocEws(2048);   // по умолчанию стоит 256

   sprintf(temp, "%sRes\\ABOUT.PNG", dir);
   info->SetHeader(temp, (int)lgp->lang[Aboute].c_str());
   info->SetHookFormBuilder(BuldInfoForm);
   info->SetLeftSoft((int)lgp->lang[Clear].c_str(), (void*)InfoClose);
   info->SetRightSoft((int)lgp->lang[Back].c_str());


#ifdef __LUA_SUPPORT__
   is_use_lua = lua_init() == 0? 1 : 0;
#endif
}


void SaveStatesConfig()
{
   unsigned int err=0;
   sprintf(temp, "%sRes\\data.inf", dir);
   int fp = _open(temp, A_Create+A_WriteOnly+A_Truncate+A_BIN,P_READ+P_WRITE, &err);
   if(fp!=-1)
   {
      zeromem(temp, 16);
      memcpy(temp, &inputlang, sizeof(int));
      memcpy(temp+sizeof(int), &outputlang, sizeof(int));
      _write(fp, temp, sizeof(int)*2, &err);
      _close(fp, &err);

      err=0;
      sprintf(temp, "%sRes\\data.inf", dir);
      SetFileAttrib(temp, FA_HIDDEN, &err);
   }
}

Read::~Read()
{
   //NET :: Destroy();
   SUBPROC((void*)SaveStatesConfig);
   Close();

#ifdef __LUA_SUPPORT__
   if(is_use_lua)
    lua_finish();
#endif

   delete info;
   delete edit;
   delete menu;
   delete gl;
   extern GBSTMR reconnect_timer;
   GBS_DelTimer(&reconnect_timer);
   delete lgp;
}



//  ========================= csm ==================================


static void maincsm_oncreate(CSM_RAM *data)
{
   scr_w=ScreenW()-1;
   scr_h=ScreenH()-1;
   //net.init();
   MAIN_CSM*csm=(MAIN_CSM*)data;

   myread = new Read();
   my_csm_id=csm->gui_id= edit->GetGUIId();
}

void ElfKiller(void)
{
  kill_elf();
}

static void maincsm_onclose(CSM_RAM *csm)
{
   delete myread;
   //SUBPROC((void *)ElfKiller);
   ElfKiller();
}

static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
   MAIN_CSM *csm=(MAIN_CSM*)data;
   if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
   {
     csm->csm.state=-3;
   }

   gl->GBS_ConnectMessages(data, msg);

   DisableIDLETMR();
   return (1);
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
    139,
    0
  }
};

int closeElf(){
   CloseCSM(MAINCSM_ID);
   return 1;
}

void UpdateCSMname(void)
{
    wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"Google Translate Tool v%t", ver);
}


int main(char *exename, char *fname)
{
   getcwd(dir, 256);

   MAIN_CSM main_csm;
   LockSched();
   UpdateCSMname();
   MAINCSM_ID = CreateCSM(&MAINCSM.maincsm,&main_csm,0);
   UnlockSched();
   return 0;
}
