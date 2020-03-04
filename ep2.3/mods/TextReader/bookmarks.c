#include "..\inc\swilib.h"
#include "main.h"
#include "strings.h"
//#include "bookmarks.h"
//ааааааа бл€€€€€€€€€€€ тут будут закладки)))))))))
/*****************************************
формат такой наверн...
создаетс€ файл с именем открытого файла в каталоге,
например,если открыт файл 0:\\example.txt то закладки
будут в файле 0:\\zbin\\textreader\\bookmarks\\example.txt.tbm
формат:
им€ позици€  (кодировка?)
example 10   (0)
им€ будет братьс€ из ближайших слов,
либо самому потом писать, посмотрим:)
********************************************/

//ѕо хорошему тут нужно делать св€зный список....но € не разобралс€ с ним до конца еще...

extern const char BMK_PATH[128];
extern const char per_d[];
extern const char per_s[];
extern char *file_name;
extern char *file;
extern int curpos;
extern int id_ed;
extern int codepage;

typedef struct
{
  char name[32];
  int pos;
}BMK;

BMK bm[];//20 закладок
int bm_item=0;
void process_bm(char *str, int n)
{ 
  int j=0; 
  int k=0;
  char *pos=malloc(5);
  while (str[j]!=' ') {bm[n].name[k++]=str[j];j++;}    
  j++;
  k=0;
  while (j<strlen(str)) {pos[k++]=str[j];j++;}
  pos[k++]=0;
  bm[n].pos=strtoul(pos,0,10);
}

int LoadBookmark()
{
  unsigned int err; 
  int plhandle; 
  char *mem;
  int size; 
  int i,j=0; 
  char *str; 
  bm_item=0;
  char path[256];//="0:\\zbin\\TextReader\\Bookmarks\\";
  sprintf(path,per_s,BMK_PATH);
  strcat(path,file_name);
  strcat(path,".tbm");
  if(isdir(BMK_PATH,&err))//если есть директори€
    if((plhandle = fopen( path, A_ReadOnly + A_BIN, P_READ, & err ))!=-1)
      {
        mem=malloc(10000); 
        str=malloc(256); 
        if ((mem!=0)&&(str!=0)) 
          { 
           size=fread(plhandle,mem,9999,&err); 
           i=0; 
           while (i<size) //пока не конец файлј 
            { 
              strcpy(str,""); 
              j=0; 
              while (((*(mem+i)!='\n'))&&(i<size)) {*(str+j)=*(mem+i); j++;i++;}    //читаем строку из файла 
              //обрабатываем енту строчку 
              process_bm(str,bm_item);//—охран€ем имена и пути
              i++; 
              bm_item++;
            }
          }        
        }
  else
#ifdef NEWSGOLD    
    mkdir("4:\\zbin\\TextReader\\Bookmarks\\",&err);
#else
    mkdir("0:\\zbin\\TextReader\\Bookmarks\\",&err);
#endif
/* if ( plhandle==-1 ) 
  {
  //  ShowMSG(1,(int)"Can't open bookmark");
    fclose( plhandle, & err ); 
    return 0; 
  }*/

mfree(mem); 
mfree(str); 
fclose( plhandle, & err ); 
return bm_item;
}

char *name_bm;
void AddBookmark()
{
  unsigned int err; 
  int f; 
  char *s=malloc(64);
  char path[256];//="0:\\zbin\\TextReader\\Bookmarks\\";
  sprintf(path,per_s,BMK_PATH);
  strcat(path,file_name);
  strcat(path,".tbm");//TextBookMark :)
SAVE:
  if(isdir(BMK_PATH,&err))//если есть директори€
      if((f = fopen( path,A_ReadWrite + A_BIN+A_Create+A_Append, P_READ + P_WRITE, & err ))!=-1)
        {
          if(name_bm)//если им€ писали сами
           {
            sprintf(s,"%s %d\n",name_bm,curpos);
           }
          else //иначе просто пишем номер...
           {
            sprintf(s,"Bmk_%d %d\n",bm_item,curpos);
           }
          fwrite(f,s,strlen(s),&err);      
       }
  else
   {
#ifdef NEWSGOLD    
    mkdir("4:\\zbin\\TextReader\\Bookmarks\\",&err);
#else
    mkdir("0:\\zbin\\TextReader\\Bookmarks\\",&err);
#endif
    goto SAVE;
    }

fclose( f, & err ); 
mfree(name_bm);
mfree(s);
}

//-----------------------------BOOKMARKS------------------------------//

void book_iconhnd(void *data, int curitem, void *unk);
void book_ghook(void *data, int cmd){};
const HEADER_DESC book_menuhdr2={0,0,131,21,NULL,(int)"«акладки...",LGP_NULL};
const int menusoftk[]={0,1,2};

const SOFTKEY_DESC menu_skk[]=
{
  {0x0001,0x0000,(int)"ƒобавить"},
  {0x0004,0x0000,(int)"«акрыть"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menu_sktk=
{
  menu_skk,0
};

void NameBM();

int book_onkey2(void *data, GUI_MSG *msg)
{
  int i;
  i=GetCurMenuItem(data);
  char *s=malloc(64);
  int k=msg->gbsmsg->submess;
  if (k==ENTER_BUTTON)
  {
    curpos=bm[i].pos;//запоминаем позицию
    GeneralFunc_flag1(id_ed,1);//закрываем предыдущий текст
    DrawText(file,codepage);//тут думаю пон€тно))
    return 1;
  }
    
  if(k==GREEN_BUTTON)
  {
    AddBookmark();
    return 1;
  }
  if(k==LEFT_SOFT) 
  {
    NameBM();
  }
return(0);
}

const MENU_DESC book_menu=
{
  8,book_onkey2,book_ghook,NULL,
  menusoftk,
  &menu_sktk,
  0x10,//0x11,
  book_iconhnd,
  NULL,   //Items
  NULL,   //Procs
  0   //n
};

void book_iconhnd(void *data, int curitem, void *unk)
{
  WSHDR *ws;
  char *s;
  void *item=AllocMenuItem(data);
  s=malloc(128);
  sprintf(s,"%s %d",bm[curitem].name,bm[curitem].pos);
  if (s)
  {
    if (strlen(s))
    { 
      ws=AllocMenuWS(data,1024);
      wsprintf(ws,"%d.%t",curitem+1,s);
    }
  }
  else
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws,"error");
    wsInsertChar(ws,2,1);//÷вет надписей

  }
   SetMenuItemText(data,item,ws,curitem);
   mfree(s);
   FreeWS(ws);
}

void ShowBookmarks(void)
{  
   LoadBookmark();
   patch_header2(&book_menuhdr2);
   CreateMenu(0,0,&book_menu,&book_menuhdr2,0,bm_item,0,0);
}
//------------------------------NAME FOR BOOKMARKS----------------------------//

int id_bm;

HEADER_DESC name_bm_hdr={0,0,131,21,NULL,0,LGP_NULL};

int name_bm_onkey(GUI *data, GUI_MSG *msg)
{
  WSHDR *ws=AllocWS(64);
  if (msg->keys==0x0018)//0xFFF
  {
    EDITCONTROL ec;
    ExtractEditControl(data,1,&ec);	
    wstrcpy(ws,ec.pWS);
    char *s=malloc(32);
    int len;
    int i;
    name_bm=malloc(32);
    len=ws->wsbody[0];
    for(i=0;i<len;i++)//—охранение в win1251
      *(s+i)=char16to8(ws->wsbody[i+1]);
      *(s+i++)=0;
    sprintf(name_bm,per_s,s);
    AddBookmark();
    GeneralFunc_flag1(id_bm,1);
  }
return(0);
}

void name_bm_ghook(GUI *gui, int cmd)
{ 
  static SOFTKEY_DESC sk={0x0018,0x0000,(int)"Save!"};
  if (cmd==7)
  {
    SetSoftKey(gui,&sk,1);
  }
  if(cmd==0xA)
  {
    DisableIDLETMR(); // ќтключаем таймер выхода по таймауту
  }
  if (cmd==0x0C)
  {
    EDIT_SetCursorPos(gui,1);
  }
  if(cmd==0x03){}
}

void name_bm_locret(void){}

SOFTKEY_DESC menu_sk_2[]=
{
  {0x0018,0x0000,(int)"Ћев"},
  {0x0001,0x0000,(int)"ѕрав"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB menu_skt_2=
{
  menu_sk_2,0
};

INPUTDIA_DESC name_bm_desc=
{
  1,
  name_bm_onkey,
  name_bm_ghook,
  (void *)name_bm_locret,
  0,
  &menu_skt_2,
  {0,NULL,NULL,NULL},
  FONT_SMALL,
  100,
  101,
  0,
  0x00000000,
  0x40000000
};


void NameBM()
{
   char *ss=malloc(64);
   sprintf(ss,per_s,"»и€ закладки...");
   name_bm_hdr.lgp_id = (int)ss;
   EDITC_OPTIONS ec_options2;
   void *ma=malloc_adr();
   void *eq=AllocEQueue(ma, mfree_adr());
   EDITCONTROL ec;
   PrepareEditControl(&ec);
   ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_NORMAL_STR,ec.pWS,32);
   PrepareEditCOptions(&ec_options2);
   SetFontToEditCOptions(&ec_options2,1);//Ўрифт
   CopyOptionsToEditControl(&ec,&ec_options2);// опируем настройки
   AddEditControlToEditQend(eq,&ec,ma);

   patch_header2(&name_bm_hdr);
   patch_input2(&name_bm_desc);      
 
   GeneralFuncF1(1);
   mfree_adr();
   mfree(ss);
   id_bm=CreateInputTextDialog(&name_bm_desc,&name_bm_hdr,eq,1,0);//создаем диалоговое окно
}
