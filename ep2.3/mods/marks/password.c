#include "..\\inc\\swilib.h"
#include "main.h"

extern char filesetting[128];
extern int typePass;//1-проверка, 0-ввод
extern int testPass;//1-true, 0-false
extern int get_file_size(char* fname);

WSHDR *GetPass()
{
  unsigned int err; 
  int f; 
  WSHDR *pass=AllocWS(16);
  int len=0;
  WSHDR *ws=AllocWS(16);
  if((f = fopen(filesetting,A_ReadOnly+A_BIN,P_READ,&err ))!=-1)
  {
    fread(f,&len,2,&err);
    ws->wsbody[0]=len;   
    fread(f,ws->wsbody+1,len*2,&err);
  }
  fclose(f,&err); 
  pass=ws;
  FreeWS(ws);
  return pass;
}

void SavePass(WSHDR *ws)
{
  unsigned int err; 
  int f; 
  int len=ws->wsbody[0];
  if(!(get_file_size(filesetting)))
  {
    if((f = fopen(filesetting,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&err ))!=-1)
    {
      fwrite(f,ws->wsbody,len*2+2,&err);
      unsigned int crc = ws->wsbody[0];
      void *temp;
      char *arr;
      temp = ws->wsbody;   
      arr = temp;
      for(int i=2;i<(len+1)*2;i++)
      {
        crc = crc ^ arr[i];
      }
      fwrite(f,&crc,2,&err); 
      ShowMSG(1,(int)"Password save!");
    }
  }
  else ShowMSG(1,(int)"Password already input!");
  fclose(f,&err); 
}

//-------------------------------Input password------------------------------//

int _id;
extern void ws2str(WSHDR *ws,char *s);
extern MNU *changefile;
HEADER_DESC _hdr={0,0,131,21,NULL,(int)"Ввод пароля",LGP_NULL};
int _onkey(GUI *data, GUI_MSG *msg)
{
  char *s=malloc(32);
  char *s2=malloc(32);
  int  cmp;
  WSHDR *ws;
  WSHDR *pass=GetPass();
  if(msg->keys==0x0FFF)
  {
    EDITCONTROL ec;
    ExtractEditControl(data,2,&ec);	
    ws=ec.pWS;
    if(!(get_file_size(filesetting)))
      SavePass(ws);
    else
    {
      ws2str(ws,s);
      ws2str(pass,s2);
      cmp=strcmp(s,s2);
      if(cmp==0)
      {
        testPass=1;
//        ShowMSG(1,(int)"ok");
        DrawText(NULL,changefile);
        GeneralFuncF1(1);
      }
      else
      {
        ShowMSG(1,(int)"error");
        testPass=0;
      }
    }
  }
  if(msg->gbsmsg->submess==RIGHT_SOFT || msg->keys==RED_BUTTON)
  {
    return 1;
  }
return(0);
}

void _ghook(GUI *gui, int cmd)
{ 
  static SOFTKEY_DESC sk={0x0FFF,0x0000,(int)"Сохранить"};
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

void _locret(void){}

SOFTKEY_DESC _sk[]=
{
  {0x0018,0x0000,(int)"Лев"},
  {0x0001,0x0000,(int)"Прав"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB _skt=
{
  _sk,0
};

INPUTDIA_DESC _desc=
{
  1,
  _onkey,
  _ghook,
  (void *)_locret,
  0,
  &_skt,
  {0,NULL,NULL,NULL},
  FONT_SMALL,
  100,
  101,
  0,
  0x00000000,
  0x40000000
};

void InputPass()
{
   WSHDR *wss=AllocWS(256);
   void *ma=malloc_adr();
   void *eq=AllocEQueue(ma, mfree_adr());
   EDITCONTROL ec;
   PrepareEditControl(&ec);
   wsprintf(wss,"%t","Пароль:\n");
   ConstructEditControl(&ec,ECT_HEADER,ECF_NORMAL_STR,wss,32);
   AddEditControlToEditQend(eq,&ec,ma);  

   PrepareEditControl(&ec);
   CutWSTR(wss,0);
   ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL,wss,16);
   AddEditControlToEditQend(eq,&ec,ma);
   
   patch_header(&_hdr);
   patch_input(&_desc);      
 
   _id=CreateInputTextDialog(&_desc,&_hdr,eq,1,0);//создаем диалоговое окно
}
