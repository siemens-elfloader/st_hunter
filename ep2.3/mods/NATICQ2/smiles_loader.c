#include "../inc/swilib.h"
#include "../inc/pnglist.h"
#include "naticq_ipc.h"
#include "smiles.h"
#include "naticq.h"
#include "strings.h"
#include "readimg.h"
#include "smiles_loader.h"


#define ICONS DYNPNGICONLIST

S_SMILES *s_top=0;

DYNPNGICONLIST *SmilesImgList;
DYNPNGICONLIST *XStatusesImgList;

int *XStatusesIconArray;
/*
volatile int total_smiles;
volatile int total_xstatuses;
volatile int xstatuses_load;
volatile int pictures_max;
volatile int pictures_loaded = 0;
volatile int xstatuses_max=0;
*/
extern const char SMILE_FILE[];
extern const char SMILE_PATH[];
extern const char XSTATUSES_PATH[];
extern const char ipc_my_name[32];
extern  int S_ICONS[];

//static IPC_REQ gipc;
static char *p_buf;
static char *s_buf;
static S_SMILES *s_bot;
static int n_pic;

int cur_smile=0;

ICONS *smilestop = NULL;
ICONS *iconstop = NULL;
ICONS *xiconstop = NULL;

extern int drawimg2img(IMGHDR * onse, IMGHDR * timage2, int x, int y, int xRect, int yRect,int xxRect, int yyRect);
extern void PressImg(IMGHDR *imgsrc, char freemap);
extern const char *icons_names[];
extern const char ICON_PATH[];


S_SMILES *FindSmileById(int n)
{
  int i=0;
  S_SMILES *sl=(S_SMILES *)s_top;
  while(sl && i!=n)
  {
    sl=sl->next;
    i++;
  }
  return sl;
}

S_SMILES *FindSmileByUni(int wchar)
{
  S_SMILES *sl=(S_SMILES *)s_top;
  while(sl)
  {
    if (sl->uni_smile == wchar) return (sl);
    sl=sl->next;
  }
  return (0);
}

void FreeSmiles(void)
{
  S_SMILES *s_smile;
  STXT_SMILES *n;
  STXT_SMILES *st;
  DYNPNGICONLIST *d;
  DYNPNGICONLIST *nd;
  LockSched();
  //total_smiles=0;
  s_smile=(S_SMILES *)s_top;
  s_top=0;
  s_bot=0;
  UnlockSched();
  while(s_smile)
  {
    S_SMILES *s;
    s=s_smile;
    st=s->lines;
    while(st)
    {
      n=st->next;
      mfree(st);
      st=n;
    }
    s_smile=(S_SMILES *)(s_smile->next);
    mfree(s);
  }
  LockSched();
  d=SmilesImgList;
  SmilesImgList=0;
  UnlockSched();
  while(d)
  {
    if (d->img)
    {
      mfree(d->img->bitmap);
      mfree(d->img);
    }
    nd=d->next;
    mfree(d);
    d=nd;
  }
  mfree(s_buf);
}

void InitSmiles(void)
{
  int f;
  unsigned int err;
  int fsize;
  char *buf;
  FSTATS stat;

  FreeSmiles();

  //n_pic=FIRST_UCS2_BITMAP;
  if (GetFileStats(SMILE_FILE,&stat,&err)==-1)
    return;

  if ((fsize=stat.size)<=0)
    return;

  if ((f=fopen(SMILE_FILE,A_ReadOnly+A_BIN,P_READ,&err))==-1)
    return;

  buf=s_buf=p_buf=malloc(fsize+1);
  buf[fread(f,buf,fsize,&err)]=0;
  fclose(f,&err);
  //f=fopen("4:\\smiles.cfg",A_ReadWrite+A_BIN+A_Create+A_Append,P_READ+P_WRITE,&err);
/*  gipc.name_to=ipc_my_name;
  gipc.name_from=ipc_my_name;
  gipc.data=0;
   
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_SMILE_PROCESSED,&gipc);*/
}

void CheckSmiles(void)
{
  int f;
  unsigned int err;
  int fsize;
  char *buf, *p_buf;
  FSTATS stat;

  if (GetFileStats(SMILE_FILE,&stat,&err)==-1)
    return;

  if ((fsize=stat.size)<=0)
    return;

  if ((f=fopen(SMILE_FILE,A_ReadOnly+A_BIN,P_READ,&err))==-1)
    return;

  buf=p_buf=malloc(fsize+1);
  buf[fread(f,buf,fsize,&err)]=0;
  fclose(f,&err);
  
//  f = pictures_max;
  for(buf=p_buf;*buf; buf++)
    if(*buf == ':')
    {
      buf++;
      while(*buf && *buf != 0x0D) buf++;
      //pictures_max++;
    }
  mfree(p_buf);
}


//-------------------------------- 21.09.09 --------------------------------//

/*
*  Функция: FreeList
*  Описание: Очищаем связный список
*  Параметры: ICONS *icon - указатель на связный список
*  Возвращаемое значение: Ничего не возвращает
*/
static void FreeList(ICONS *icon)//очистить список
{
  if(!icon) return;
  LockSched();
  icon=0;
  UnlockSched();
  while(icon)
  {
    ICONS *icon_prev = icon;
    icon = icon->next;

    if(icon_prev)
    {
      if(icon_prev->img)
      {
        mfree(icon_prev->img->bitmap);
        mfree(icon_prev->img);
      }
      mfree(icon_prev);
    }
  }
}

/*
*  Функция: GetPicByItem
*  Описание: Выбераем из связного списка номер элемента
*  Параметры: 
*     ICONS *icon - указатель на связный список
*     int curitem - номер элемента, который нужно выбрать
*  Возвращаемое значение: Элемент IMGHDR или NULL,если таковой не найден
*/
static IMGHDR *GetPicByItem(ICONS *icon, int curitem)
{
  int i=0;

  while(icon)
  {
    if(i==curitem) return (icon->img);
    i++;
    icon=icon->next;  
  }
  return 0;
}


/*
*  Функция: CountPic
*  Описание: Считает кол-во элементов в списке
*  Параметры: ICONS *icon - указатель на связный список
*  Возвращаемое значение: Кол-во элементов
*/
static int CountPic(ICONS *icon)//подсчет  общего кол-ва итемов
{
  int i=0;
  while(icon) { i++; icon = icon->next; }
  return i;
}

/*
*  Функция: AddToTopList
*  Описание: Добавить элемент в конец списка
*  Параметры: 
*       ICONS *top - указатель на первый жлемент списка
*       ICON *newitem - элемент, который будем добавлять
*  Возвращаемое значение: Ничего не возвращает
*/
static void AddToTopList(ICONS *top, ICONS *newitem)//добавить в конец списка итем
{
  ICONS *icon = (ICONS *)top;
  while(icon->next) icon = icon->next;
  icon->next = newitem;
}


/*
*  Функция: NewIcon
*  Описание: Создаем новый элемент
*  Параметры: IMGHDR *img - иконка для добавления в список
*  Возвращаемое значение: созданный элемент ICONS
*/
ICONS* NewIcon(IMGHDR *img)
{
  ICONS *icon = malloc(sizeof(ICONS));
  zeromem(icon,sizeof(icon));
  icon->next = 0;
  icon->img = img;
  return icon;
}

//--------------------------------

/*
*  Функция: AddSmile
*  Описание: Вырезаем из файла смайлов нужный кусок и сохраняем как отдельный смайл
*  Параметры: 
*      IMGHDR *img - указатель на общую картинку смайлов
*      int x - начальное смещение Х
*      int y - начальное значеие У
*  Возвращаемое значение: Ничего не возвращает
*/
static void AddSmile(IMGHDR *img,int x,int y)
{
   ICONS *icon =  NewIcon(CreateImgr(28,28));//malloc(sizeof(ICONS));
   drawimg2img(img,icon->img,0,0,x,y,x+27,y+27);
   PressImg(icon->img,1); 
   
   if(!smilestop)
     smilestop = icon; 
   else
     AddToTopList(smilestop, icon);
}



IMGHDR *GetSmileByItem(int curitem)
{
  return GetPicByItem(smilestop, curitem);  
}

void FreeSmileList()
{
  FreeList(smilestop);
}


void ProcessNextSmile(void)  
{
  int c;
 // char fn[128];
  DYNPNGICONLIST *dp;
  S_SMILES *si;
  STXT_SMILES *st;
  char *buf;
next_s:
  buf=p_buf;
  if (!buf) return;
  while ((c=*buf))
  {
    char *p;
    if ((c==10)||(c==13))
    {
      buf++;/*
      gipc.name_to=ipc_my_name;
      gipc.name_from=ipc_my_name;
      gipc.data=0;*/
     // GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_SMILE_PROCESSED,&gipc);
      p_buf=buf;
      goto next_s;
    //  return;
    }
    p=strchr(buf,':');
    if (!p) break;
/*    zeromem(fn,128);
    strcpy(fn,SMILE_PATH);
    if (fn[strlen(fn)-1]!='\\') strcat(fn,"\\");
    c=p-buf;
    if (c>(127-strlen(fn))) break;
    strncpy(fn+strlen(fn),buf,c);*/
//    snprintf(logmsg,255,"Process file %s...",fn);
//    REDRAW();
    buf=p;
    dp=malloc(sizeof(DYNPNGICONLIST));
    zeromem(dp,sizeof(DYNPNGICONLIST));
    dp->icon=GetPicNByUnicodeSymbol(n_pic);
    dp->img=GetSmileByItem(cur_smile++);//CreateIMGHDRFromPngFile(fn,0);
    LockSched();
    if (SmilesImgList)
    {
      dp->next=SmilesImgList;
    }
    SmilesImgList=dp;
    UnlockSched();
    si=malloc(sizeof(S_SMILES));
    si->next=NULL;
    si->lines=NULL;
    si->botlines=NULL;
    si->uni_smile=n_pic;
    if (s_bot)
    {
      //Не первый
      s_bot->next=si;
      s_bot=si;
    }
    else
    {
      //Первый
      s_top=si;
      s_bot=si;
    }
    n_pic++;
    while (*buf!=10 && *buf!=13 && *buf!=0)
    {
      buf++;
      int i=0;
      while (buf[i]!=0&&buf [i]!=','&&buf [i]!=10&&buf[i]!=13)  i++;
      st=malloc(sizeof(STXT_SMILES)+i);
      strncpy(st->text,buf,i);
      st->text[i]=0;
      
      st->next=NULL;
      st->key=*((unsigned long *)st->text);
      st->mask=~(0xFFFFFFFFUL<<(8*i));
      st->key&=st->mask;
      if (si->botlines)
      {
	si->botlines->next=st;
	si->botlines=st;
      }
      else
      {
	si->lines=st;
	si->botlines=st;
      }
      buf+=i;
    }
    //pictures_loaded++;
    //total_smiles++;
  }
  //fclose(f,&err);
  //total_smiles=0;
  p_buf=NULL;
  mfree(s_buf);
  s_buf=NULL;
  REDRAW();
}

void FreeXStatusesImg(void)
{
  void FreeXIconList();
  FreeXIconList();
}

#ifdef GUI_INTERFACE  

#define x_img_x_size 16
#define x_img_y_size 16
/*
*  Функция: AddXIcon
*  Описание: Выборка картинок Х-Статусов
*  Параметры: 
*       IMGHDR *img - указатель на пакфайл х-статусов
*       int x - смещение по Х
*
*  Возвращаемое значение: Ничего не возвращает
*/
void AddXIcon(IMGHDR *img,int x)
{
#ifdef ELKA
   ICONS *icon = NewIcon(CreateImgr(x_img_x_size, x_img_y_size));
   icon->icon = GetPicNByUnicodeSymbol(n_pic);
   n_pic++;
   drawimg2img(img,icon->img, 0, 0, x, 0, x + x_img_x_size, x_img_y_size);

   PressImg(icon->img,1);
#else
   ICONS *icon = malloc(sizeof(IMGHDR));//NewIcon(CreateImgr(img->x, img->y));
   icon->icon = GetPicNByUnicodeSymbol(n_pic);
   n_pic++;
//   memcpy((void*)icon->img, img);
   icon->img = img;
#endif
   if(!xiconstop)
     xiconstop = icon;    
   else
     AddToTopList(xiconstop, icon);
}

void FreeXIconList()
{
  FreeList(xiconstop);
}
 
int CountXIcon()
{
  return CountPic(xiconstop);
}

IMGHDR *GetXIconByItem(int curitem)
{
  return GetPicByItem(xiconstop, curitem);
}


int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}

#endif

#define img_x_size 16
#define img_y_size 16

/*
*  Функция: AddIcon
*  Описание: Добавляем в список иконки меню
*  Параметры: 
*     IMGHDR *img - файлпак картинок
*     int x - смещение по Х, место с какого вырезать
*  Возвращаемое значение: Ничего не возвращает
*/
void AddIcon(IMGHDR *img,int x)
{
#ifdef ELKA1
   ICONS *icon = NewIcon(CreateImgr(img_x_size, img_y_size));//malloc(sizeof(ICONS));
   drawimg2img(img,icon->img, 0, 0, x, 0, x + img_x_size, img_y_size);
   PressImg(icon->img,1);
#else
   ICONS *icon = malloc(sizeof(IMGHDR));;
   icon->img = img;
#endif
   if(!iconstop)
     iconstop=icon; 
   else
     AddToTopList(iconstop, icon);
}


void FreeIconList()
{
  FreeList(iconstop);
}
 
int CountIcon()
{
  return CountPic(iconstop);
}

IMGHDR *GetIconByItem(int curitem)
{
  return GetPicByItem(iconstop, curitem);
}

//подключаем модули картинок

#include "XST.c"
#include "status.c"
#include "IMAGE.c"

void free_bitmap(IMGHDR *img)
{
  if(img) 
  {
    mfree(img->bitmap);
    mfree(img);
  }
}

#define max_pixel 16

/*
*  Функция: LoadImg
*  Описание: Возвращает загруженную картинку с диска, или если такой нет, то из тела эльфа
*  Параметры:
*  char *picname - имя загружаемой картинки
*  int max_x - ширина загружаемой картинки
*  char loadFromElf - если загружаться из эльфа, то указываем что грузим
*  loadFromElf = 0 - STATUS_PNG
*  loadFromElf = 1 - IMAGE_PNG
*  loadFromElf = 2 - XST_PNG
*  Возвращаемое значение: Возвращает загруженную картинку в IMGHDR
*/
IMGHDR *LoadImg(char *picname, int max_x, char loadFromElf)//loadFromElf 0 - STATUS_PNG, 1 - IMAGE_PNG, 2 - XST_PNG
{
  IMGHDR *img;
  //если на флешке есть файл то загружаем его
  if(get_file_size(MakeGlobalString(ICON_PATH,'\\',/*"status.png"*/picname)))
  {
    img = CreateIMGHDRFromPngFile(MakeGlobalString(ICON_PATH,'\\',picname/*"status.png"*/),0);
    if(img && (img->w > (/*(IS_FFC+1)*/max_x) || img->h > max_pixel))//защита от дурака =) если размер не стандарт тоочищаем и загружаем то шо в эльфе
    {
      free_bitmap(img);   
      char *path = (loadFromElf==0) ? (STATUS_PNG) : ((loadFromElf==1) ? (IMAGE_PNG) : (XST_PNG));
      img = read_pngimg(path);
    }
  }
 
  //если файла нет то загружаем графу из эльфа
  if(!get_file_size(MakeGlobalString(ICON_PATH,'\\',picname/*"status.png"*/)))
    img = read_pngimg((loadFromElf==0) ? (STATUS_PNG) : ((loadFromElf==1) ? (IMAGE_PNG) : (XST_PNG)));
  
  return img;
}



/*
*  Функция: LoadImages
*  Описание: Загружает картинки, статусы, х-статусы, и картинки меню
*  Параметры: Ни паких параметров
*  Возвращаемое значение: ничего не возвращает
*/
void LoadImages()
{
  IMGHDR *img;
  
  img = LoadImg("status.png", (IS_FFC+1)*max_pixel, 0);

  int start_x = 0;
//  int pos_w = 0, pos_h = 0;
  for(int i = 0 ; i < TOTAL_ICONS ; i++)
  {
#ifdef ELKA
    if(i == IS_MSG)//если дошли до картинки сообщений, то подгружаем файл с картинками
    {
      free_bitmap(img);      
      img = LoadImg("image.png", (ICON_UNCHECK - IS_MSG)*max_pixel, 1);
      start_x = 0;
    }

    if(i <= IS_FFC)//загружаем статусы
    {
      AddIcon(img, start_x);
      start_x += img_x_size;
    }
    else
    {
      if(i == ICON_LOGO || i == ICON_BG)
      {
        //просто добавим NULL...не нужна она...
        AddIcon(NULL,1);            
        i++;
      }

      AddIcon(img, start_x);
      start_x += img_x_size;
      if(start_x > img->w) break;
    }
#else
      extern const char *icons_names[];
      char fn[128];
      sprintf(fn,"%s\\%s",ICON_PATH, icons_names[i]);
      if(get_file_size(fn))
        AddIcon(CreateIMGHDRFromPngFile(fn,0), 0);
      else
        AddIcon(NULL,1);
#endif
   }
  
  //x-statuses
#ifdef ELKA
  free_bitmap(img);
  img = LoadImg("xst.png",594,2);
  
  start_x = 0;
#else
  char fn[128];
#endif
  
  for(int i = 0 ; i<35 ; i++)
   {
#ifdef ELKA
     AddXIcon(img, start_x);
     start_x += 17;
     if(start_x > img->w) break;
#else
     sprintf(fn,"%s\\%d.png",XSTATUSES_PATH, i);
     AddXIcon(CreateIMGHDRFromPngFile(fn,0), 0);
#endif
   }
  free_bitmap(img);
}


/*
*  Функция:  LoadingSmiles
*  Описание: Загружает смайлики в буфер
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
void LoadingSmiles(void)
{
  //xstatuses_load=0;
  if(get_file_size(MakeGlobalString(ICON_PATH,'\\',"smiles.png")))//если есть файл со смайлами, то грузим
  {
    int x=0,y=0;
    IMGHDR *img = CreateIMGHDRFromPngFile(MakeGlobalString(ICON_PATH,'\\',"smiles.png"),0);
    for(int count=0;count<96;count++)
    {
      if((count > 0 && count%12 == 0) || (x > img->w)) { x = 0; y+=28; }
      AddSmile(img,x,y);
      x += 28;
      if(y > img->h) break;
    }
    free_bitmap(img);
  }
  else
  {
    smilestop = 0;
    //pictures_loaded = pictures_max;
    FreeSmiles();
  }
}


/*
*  Функция: LoadingImages
*  Описание: При старте эльфа последовательно загружает все картинки нужные в эльфе
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
void LoadingImages()//загружаем смайлы, последовательности функций не менять...
{
   n_pic = FIRST_UCS2_BITMAP;
   CheckSmiles();
   LoadingSmiles(); 
   InitSmiles();
   ProcessNextSmile();
   LoadImages();
}

void FreeAllList()//очищаем все списки
{
   FreeSmileList();
   FreeSmiles();
   FreeIconList();
   FreeXIconList();
}
