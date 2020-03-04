#ifndef _MAIN_H_
  #define _MAIN_H_
typedef struct
{ 
  int x;
  int y;
  int show;
 
}crds;

typedef struct
{
  char *description;
  char icon[128];
  char file[256];//пути к файлам
  char name[236];//Название закладок
  char *shortcut;
}DESC;
DESC descmenu[16];

crds crd[7][3];

typedef struct
{
 char file[256];
 char name[32];
 char small[256];
 char big[256];
 int NumItems;
}MENUPIC;

MENUPIC mnu[9];

#define clock 0
#define NetDB 1
#define NetAdv 2
#define BattCap 3
#define BattVolt 4
#define Dialogs 5
#define FreeRam 6

typedef struct
{ 
  char filename[64];
  char file[256];
  char pic[256];
  int NumItems;//Количество записей в файле конфига
}Panel;

Panel name[9];
int NumOfItems=0;
int NumOfItems2=0;
int speed_cur;//Скорость курсора
int workdesctop=2;
int typecur=0;
int IRDA=0;
int cr_y=157;
int Cross_x=50, Cross_y=50;
int DrawMamory=0;
int TypeLine=0;
int kdblock=0;
int moveclock=0;
int Pusk=0;
int menu=1;
int startmenu=0;

char *NAME[9];
char *NAME2[9];

void SaveNamePanel();
void LoadParams();
void DrawInfo();
void ShowBMmenu1(void);
char *shortcut(char cut[15]);//запуск шоткартов
void SaveNameMENU();
void DrawStrInt(int s,int x, int y);//Для отладки...


#endif

//STUP_CONCT_IRDA GPRS

