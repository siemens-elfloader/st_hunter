#include "../inc/swilib.h"
#include "rect_patcher.h"
#include "mainmenu.h"
#include "edit_settings.h"
#include "math.h"
//криво,но работает:)
char per_d[]="%d";
char per_s[]="%s";
char per_t[]="%t";

int len=0;
int id_input;
int id_create;
extern unsigned long  strtoul (const char *nptr,char **endptr,int base);

int menu_id;
int it=4;
char *menu_name[]={
  "Из DEC",
  "Из BIN",
  "Из OCT",
  "Из HEX"
};

//---------------------Создаем список меню-----------------------//

void menu_iconhnd(void *data, int curitem, void *unk);
const int menusoftke[]={0,1,2};

const SOFTKEY_DESC menu_[]=
{
  {0x0001,0x0000,(int)"Выбрать"},
  {0x0004,0x0000,(int)"Закрыть"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menu_s=
{
  menu_,0
};

void menu_ghook(void *data, int cmd){};
int menu_onkey2(void *data, GUI_MSG *msg);
HEADER_DESC menuhdr2={0,0,131,21,NULL,(int)"Выбор системы",LGP_NULL};

const MENU_DESC menu=
{
  8,menu_onkey2,menu_ghook,NULL,
  menusoftke,
  &menu_s,
  0x10,//0x11,
  menu_iconhnd,
  NULL,   //Items
  NULL,   //Procs
  7   //n
};

void menu_iconhnd(void *data, int curitem, void *unk)
{
  WSHDR *ws;
  char *s;
  void *item=AllocMenuItem(data);
  s=malloc(64);
  sprintf(s,"%s",menu_name[curitem]);
  if (s)
  {
    if (strlen(s))
    { 
      ws=AllocMenuWS(data,strlen(s)+4);
      wsprintf(ws,"%t",s);
    }
  }
  else  
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws,"error");
    wsInsertChar(ws,2,1);//Цвет надписей

  }
   SetMenuItemText(data,item,ws,curitem);
}

void input(int i);
int menu_onkey2(void *data, GUI_MSG *msg)
{
  int i;
  i=GetCurMenuItem(data);
  int k=msg->gbsmsg->submess;
/*if(k=='*')
   {
     about();
     return (-1);
   }*/
  if (k==ENTER_BUTTON || k==LEFT_SOFT)
  {
    input(i);
  }

 if(k==RIGHT_SOFT || msg->keys==RED_BUTTON)
 {
  GeneralFunc_flag1(menu_id,1);
  return(1);
 }
  return(0);
}

void ShowConvertMenu(GUI *gui)
{ 
 patch_header(&menuhdr2);
 menu_id=CreateMenu(0,0,&menu,&menuhdr2,0,it,0,0);
}

//---------------------------------------------------------------------------//
/*
char *dec2boh(int dec,int sys)//dec->bin,oct:) ; sys-2,8 (16 нет смысла)
{
  int i=0;
  int j=0;
  char *tmp=malloc(32);
  char *s=malloc(32);
  while(dec>0)
  {
    i=0;
    i=dec%sys;
    dec=dec/sys;
    sprintf(s,per_d,i);
    *(tmp+j)=*s;
    j++;
    if(dec==1)
     {
      sprintf(s,per_d,1);
      *(tmp+j)=*s;
      break;
     }
  }
  j++;
  *(tmp+j)=0;
  for(i=strlen(tmp)-1;i>=0;i--)//Переворачиваем число
    s[strlen(tmp)-i]=tmp[i];
  len=strlen(tmp);
return s;
}

int boh2dec(char *boh,int sys)//bin,oct,hex->dec ; sys-2,8,16 //Преобразуем в десятичное число
{
  int i=0;
  int rez=0;
  int j=0;
  for(i=strlen(boh),j=0;i>=0;j++,i--)
   {
      rez+=pow(sys,j-1);
   }
return rez;
}
*/

char *dec2bin(int dec)
{
  int i=0;
  int j=0;
  char *tmp=malloc(32);
  char *s=malloc(32);
  while(dec>0)
  {
    i=0;
    i=dec%2;
    dec=dec/2;
    sprintf(s,per_d,i);
    *(tmp+j)=*s;
    j++;
    if(dec==1)
     {
      sprintf(s,per_d,1);
      *(tmp+j)=*s;
      break;
     }
  }
  j++;
  *(tmp+j)=0;
  for(i=strlen(tmp)-1;i>=0;i--)
  {
    s[strlen(tmp)-i]=tmp[i];
  }
  len=strlen(tmp);
return s;
}

int bin2dec(char *bin)
{
  int i=0;
  int rez=0;
  int j=0;
  for(i=strlen(bin),j=0;i>=0;j++,i--)
   {
     if(bin[i]=='1')//на 0 нет смысла умножать:)
      rez+=pow(2,j-1);
   }
return rez;
}

//-------------------------------//

char *dec2oct(int dec)
{
 int i=0;
 int j=0;
 char *s=malloc(32);
 char *tmp=malloc(32);
 while(dec>0)
  {
   i=0;
   i=dec%8;
   dec=dec/8;
   sprintf(s,per_d,i);
   *(tmp+j)=*s;
   j++;
   if(dec==1)
    {
      sprintf(s,per_d,1);
      *(tmp+j)=*s;
     break;
    }
  }
  j++;
  *(tmp+j)=0;
  for(i=strlen(tmp)-1;i>=0;i--)
  {
    s[strlen(tmp)-i]=tmp[i];
  }
  len=strlen(tmp);
return s;
}

const char per_c[]="%c";
int oct2dec(char *bin)
{
  long rez=0;
  long c=0;
  char d[1];
  int i;
  int j=0;
  for(i=strlen(bin),j=0;i>=0;j++,i--)
   {
    sprintf(d,per_c,bin[i]);
    c=strtoul(d,0,10);
    rez+=pow(8,j-1)*c;
   }
return rez;
}

int hex2dec(char *bin)
{
  long rez=0;
  long c=0;
  char d[1];
  int i;
  int j=0;
  for(i=strlen(bin),j=0;i>=0;j++,i--)
   {
    sprintf(d,per_c,bin[i]);
    c=strtoul(d,0,16);
    rez+=pow(16,j-1)*c;
   }
return rez;
}

//--------------------------------Редактор--------------------------------//

HEADER_DESC disk_prop_hdr={0,0,131,21,NULL,0,LGP_NULL};
int id_create;
enum editcontrols
{
  ec_dec=2,
  ec_bin=4,
  ec_oct=6,
  ec_hex=8,
};

enum sys
{
  bin=2,
  oct=8,
  dec=10,
  hex=16,
};

int id_ed2;
extern int menu_id;
int empty_onkey(GUI *data, GUI_MSG *msg)
{
 if(msg->keys==LEFT_SOFT || msg->keys==RED_BUTTON)
 {
  GeneralFunc_flag1(id_input,1);
  GeneralFunc_flag1(id_create,1);
  return(1);
 }
 return(0);
}

SOFTKEY_DESC sk[];
SOFTKEYSTAB skt={ sk,0 };

void empty_locret(void){}

void empty_ghook(GUI *gui, int cmd)
{ 
  static SOFTKEY_DESC sk={0x0FFF,0x0000,(int)""};
  
  if(cmd==0xA)
  {
    DisableIDLETMR();// Отключаем таймер выхода по таймауту
  }
  
  if(cmd==0x03)// onDestroy
  {
  }
  
  if (cmd==7)
  {
    SetSoftKey(gui,&sk,SET_SOFT_KEY_N);
  }
}

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
 

void mess(char *s)
{
  ShowMSG(1,(int)s);
}

void mess2(char *s)
{
  ShowMSG(2,(int)s);
}

void messd(int i)
{
  char *s=malloc(10);
  sprintf(s,"%d",i);
  ShowMSG(1,(int)s);
}

int abs(char *num)
{
 int i;
 int neg=1;
 //if(num<0) return num=num*-1;
 if(num[0]=='-')
 {
  neg=-1;
  i=strtoul(num,0,10);
  i=i*neg;
 }
 return i;
}
//--------------------------------------------------------//
//111111->0011 1111
void wsInsert(WSHDR *ws,char *s,int len,int interval)//interval-кол-во символов до пробела
{
// int j=0;
 int k=0;
 int i=0;
/* int rez=0;   
 if(len>interval)
 {
   rez=len%interval;
//   if(rez!=0) rez-=4;
  // rez=abs(-4);
//   messd(rez);
 }*/
 CutWSTR(ws,0);   
 for(k=0,i=0;i<=len;k++)
 { 
   wsInsertChar(ws,s[i],k);
   i++;   
   /*
  if(rez!=0)
  {
    wsInsertChar(ws,'0',k);//Втавляем нули чтобы было равномерно:)
    rez--;
    j++;
    continue;
  }
  else
  {  
    if(j==interval)
     {
      wsInsertChar(ws,' ',k);//Вставляем пробел чтобы разбить по interval цифр...
      j=0;
     }
    else
     {
      wsInsertChar(ws,s[i],k);
      i++;
      j++;
     }
  }*/
 }
}
int ed_create(int i,char *s);

void ConvertRezult(GUI *gui)
{ 
  ed_create(0,NULL); 
}

extern int create_id;
int ed_create(int i,char *s)
{     
  EDITCONTROL ec;
  PrepareEditControl(&ec);
  void *edgui=FindGUIbyId(create_id,NULL);
  if(!s)
  {
    ExtractEditControl(edgui,2,&ec);
    s=malloc(ec.maxlen);
    ws_2str(ec.pWS,s,ec.maxlen);
    if(wstrlen(ec.pWS)==0)
    {
     mess("Нет данных!");
     GeneralFunc_flag1(id_create,1);
     return 1;
    }
  }
  PrepareEditControl(&ec);
  char *ss=malloc(32);
  void *ma=malloc_adr();
  void *eq=AllocEQueue(ma, mfree_adr());
  WSHDR *ws=AllocWS(256);
  WSHDR *ws_dec=AllocWS(32);
  WSHDR *ws_bin=AllocWS(32);
  WSHDR *ws_oct=AllocWS(32);
  WSHDR *ws_hex=AllocWS(32);

  switch(i)
   {
    case 0: //DEC
     wsprintf(ws_dec,per_s,s);
     
     ss=dec2bin(strtoul(s,0,10));
     wsInsert(ws_bin,ss,len,4);//dec->bin
       
     *(ss)=0;
     ss=dec2oct(strtoul(s,0,10));
     wsInsert(ws_oct,ss,len,3);//dec->oct
      
     CutWSTR(ws_hex,0);
     wsprintf(ws_hex,"%X",strtoul(s,0,10));//dec->hex
     break;
     
    case 1: //BIN
     wsprintf(ws_bin,per_s,s);//ws_bin->bin 
      
     wsprintf(ws_dec,per_d,bin2dec(s));//bin->dec
     ws_2str(ws_dec,s,wstrlen(ws_dec));
      
     *(ss)=0;
     ss=dec2oct(strtoul(s,0,10));//dec->oct
     wsInsert(ws_oct,ss,len,3);
     CutWSTR(ws_hex,0);
     wsprintf(ws_hex,"%X",strtoul(s,0,10));//dec->hex
     break;       
       
    case 2: //OCT
     wsprintf(ws_oct,per_s,s);//oct
     
     wsprintf(ws_dec,per_d,oct2dec(s));//oct->dec       
     ws_2str(ws_dec,s,wstrlen(ws_dec));
     
     ss=dec2bin(strtoul(s,0,10));//dec->bin
     wsInsert(ws_bin,ss,len,4);
 
     CutWSTR(ws_hex,0);
     wsprintf(ws_hex,"%X",strtoul(s,0,10));//dec->hex      
     break;      

   case 3:  //HEX
     wsprintf(ws_hex,per_s,s);//hex
   
     wsprintf(ws_dec,per_d,hex2dec(s));//hex->dec
     ws_2str(ws_dec,s,wstrlen(ws_dec));
       
     ss=dec2bin(strtoul(s,0,10));//dec->bin
     wsInsert(ws_bin,ss,len,4);
     
     *(ss)=0;
     ss=dec2oct(strtoul(s,0,10));//dec->oct
     wsInsert(ws_oct,ss,len,3);
     break;
  }
  
  wsprintf(ws,per_t,"DEC:\n");
  ConstructEditControl(&ec,ECT_HEADER,ECF_NORMAL_STR,ws,10);
  AddEditControlToEditQend(eq,&ec,ma);
  
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL|ECF_DISABLE_MINUS|ECF_DISABLE_POINT,ws_dec,32);
  AddEditControlToEditQend(eq,&ec,ma);  

  wsprintf(ws,per_t,"BIN:");
  ConstructEditControl(&ec,ECT_HEADER,ECF_SET_CURSOR_END,ws,10);
  AddEditControlToEditQend(eq,&ec,ma);

  ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL|ECF_DISABLE_MINUS|ECF_DISABLE_POINT,ws_bin,32);
  AddEditControlToEditQend(eq,&ec,ma);
  
  wsprintf(ws,per_t,"OCT:\n");
  ConstructEditControl(&ec,ECT_HEADER,ECF_NORMAL_STR,ws,11);
  AddEditControlToEditQend(eq,&ec,ma);

  ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL|ECF_DISABLE_MINUS|ECF_DISABLE_POINT,ws_oct,32);
  AddEditControlToEditQend(eq,&ec,ma);  
  
  wsprintf(ws,per_t,"HEX:\n");
  ConstructEditControl(&ec,ECT_HEADER,ECF_NORMAL_STR,ws,11);
  AddEditControlToEditQend(eq,&ec,ma);

  ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL,ws_hex,32);
  AddEditControlToEditQend(eq,&ec,ma);  

  disk_prop_hdr.lgp_id = (int)"Результат";        
  patch_header(&disk_prop_hdr);
  patch_input(&edit_desc);      

  FreeWS(ws);
  FreeWS(ws_dec);
  FreeWS(ws_oct);
  FreeWS(ws_bin);
  FreeWS(ws_hex);
  return id_create=CreateInputTextDialog(&edit_desc,&disk_prop_hdr,eq,1,0);
}

//--------------------------------INPUT----------------------------------//

int lock_durak(int i,char *s)
{
 if(i==1)
 {
  for(int j=0;j<=strlen(s);j++)
    if(s[j]>='2' && s[j]<='9')
     {
      mess("Неверный ввод!");    
      return 1;
     } 
 } 
 if(i==2)
 {
  for(int j=0;j<=strlen(s);j++)
    if(s[j]=='9')
     {
      mess("Неверный ввод!");    
      return 1;
     } 
 } 
return 0;
}

char hdr[64];
HEADER_DESC pos_hdr={0,0,131,21,NULL,(int)hdr,LGP_NULL};

int conv=0;

int pos_onkey(GUI *data, GUI_MSG *msg)
{
  WSHDR *ws=AllocWS(64);
  char num[32];

  if (msg->keys==0x0018)//0xFFF
  {
    EDITCONTROL ec;
    ExtractEditControl(data,1,&ec);	
    ws=ec.pWS;
    ws_2str(ws,num,ec.maxlen);
    if(!lock_durak(conv,num))
      ed_create(conv,num);
  }
return(0);
}

void pos_ghook(GUI *gui, int cmd)
{ 
  static SOFTKEY_DESC sk={0x0018,0x0000,(int)"Перевод"};
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
  }
  if(cmd==0x03){}
}

void pos_locret(void){}

SOFTKEY_DESC menu_sk_pos[]=
{
  {0x0018,0x0000,(int)"Лев"},
  {0x0001,0x0000,(int)"Прав"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB menu_skt_pos=
{
  menu_sk_pos,0
};

INPUTDIA_DESC pos_desc=
{
  1,
  pos_onkey,
  pos_ghook,
  (void *)pos_locret,
  0,
  &menu_skt_pos,
  {0,NULL,NULL,NULL},
  FONT_SMALL,
  100,
  101,
  0,
  0x00000000,
  0x40000000
};

char *cc[]={"DEC","BIN","OCT","HEX"};
void input(int i)
{
   conv=i;
   EDITCONTROL ec;
   sprintf(hdr,"%s %s","Ввод",cc[i]);
   void *ma=malloc_adr();
   void *eq=AllocEQueue(ma, mfree_adr());

   PrepareEditControl(&ec);
   if(i!=3)
    ConstructEditControl(&ec,ECT_NORMAL_NUM,ECF_APPEND_EOL|ECF_DISABLE_MINUS|ECF_DISABLE_POINT,ec.pWS,32);
   else
     ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL,ec.pWS,32);
   AddEditControlToEditQend(eq,&ec,ma);

   patch_header(&pos_hdr);
   patch_input(&pos_desc);      
 
   id_input=CreateInputTextDialog(&pos_desc,&pos_hdr,eq,1,0);//создаем диалоговое окно
}
