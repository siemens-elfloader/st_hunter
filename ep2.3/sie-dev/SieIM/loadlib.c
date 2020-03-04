#include "inc\swilib.h"
#include "lib.h"
#include "lib2.h"
//#include "lang.h"
#include "icq\ICQ.h"
#include "sieim.h"
#include "inc\str.h"

/*#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif*/

#define DATE 0
#define DBG 1
#define GPRS 2
#define MESG 3
#define STR 4
#define TYPE 5

////////////////////////////////////////////////////////////////////////////////////////////////////
   void (*WriteStr)(char * msg, char * fn);
   int  (*strcmp_nocase2)(const char *s, const char *d);
   char *(*MakeGlobalString2)(const char *first, int breakchar, const char *last);
   void (*reverse)(char s[]);
   int (*InitLgp)(char *lang, void *lng);
   int (*InitLgp_2)(char *lang, void **array);
   int (*CreateLgp)(char *lang,  int build, int type, int count, int size, char *cright, void *lng);
   int (*CreateLgp_2)(char *lang,  int build, int type, int count, int size, char *cright, void **array);
   int (*GetLgpInfo)(char *lang, LANGH *head);
   char *(*GetDateTimeNow)(void);
   void (*WriteDebug)(char * msg);
   void (*WriteDebugSf)(char * msg, ...);
   int  (*DumpAdr)(const void *source,int cnt);
   void (*WriteStr)(char * msg, char * fn); 
   void (*vsprintf2)(char *text, const char *msg, va_list param);
   char *(*IntToStr)(int i);
   void (*itoa)(int n, char s[]);
   int (*atoi)(char *attr);
   void (*RestartGPRS)(void);
   int (*GetSesInfo)(int Now, int Max, int Sr);
   int (*GetSessionTraffic)(int blur);
   int (*GetAllTraffic)(int blur);
   char *(*GetCharTraffic)(int Traffic, int MB, int Text);
   char *(*GetCharMoney)(int Traffic, int valute);
   char *(*GetSessionTime)(int Secunde, const char *Format);
   int (*CheckLimit)(void);
   void (*ShowMessage)(const char *Message);
////////////////////////////////////////////////////////////////////////////////////////////////////

int LoadLib = 0;  // Загружены ли либы
// Настройки lib
   int *Debug;
   char *Path;
   int *trafic_size;
   int *trafic_gold;
   int *trafic_silver;
   int *trafic_blur;
   char *text_valute;
   int *LIMIT_TRAF_ACTIVE;
   int *LIMIT_TRAF_KB;
   int *LIMIT_TRAF_SEC;  
   int *session_time_enable;
   int *TypeMes;

////////////////////////////////////////////////

const char *lib_name[LIBCOUNT]=  // путь к либам
{
  "date.elf",
  "dbg.elf",
  "gprs.elf",
  "mesg.elf",
  "str.elf",
  "type.elf"
};

char *lib_path[LIBCOUNT];

void set_libs ()  // составляем пути
{
  char path[128];
  register int i;
  zeromem(path,128);
  strcpy(path,ELF_PATH);
  strcat(path,"lib\\");
  for (i=0;i<LIBCOUNT;++i)
    lib_path[i]=MakeGlobalString(path,0,lib_name[i]);
}

void free_libs()  // очищаем пути
{
  register int i=0;
  do
  {
    mfree((void*)lib_path[i]);
    lib_path[i]=0;
    i++;
  }
  while(i<LIBCOUNT);
}

////////////////////////////////////////////////////////////////////////////////
//                          переменные структур                               //
////////////////////////////////////////////////////////////////////////////////


STRExecStruct      STREStruct ;
STRParamStruct     STRPStruct ;
DATEExecStruct     DATEEStruct;
DATEParamStruct    DATEPStruct;
DBGExecStruct      DBGEStruct ;
DBGParamStruct     DBGPStruct ;
TYPEExecStruct     TYPEEStruct;
TYPEParamStruct    TYPEPStruct;
GPRSExecStruct     GPRSEStruct;
GPRSParamStruct    GPRSPStruct;
MESGExecStruct     MESGEStruct;
MESGParamStruct    MESGPStruct;


void execelf(char *exename, void *fname)
{
  WSHDR *ws=AllocWS(256); 
  str_2ws(ws,exename,strlen(exename)+1); 
  ExecuteFile(ws,0,fname);
  FreeWS(ws);
}

void loadlib_date()
{
  DATEPStruct.Struct = &DATEEStruct;
  execelf(lib_path[DATE], &DATEPStruct);
}

void loadlib_dbg()
{
  DBGPStruct.GetDateTimeNow = &(*DATEEStruct.GetDateTimeNow);
  DBGPStruct.WriteStr = &(*STREStruct.WriteStr);
  DBGPStruct.vsprintf2 = &(*STREStruct.vsprintf2);
  DBGPStruct.Debug = *Debug;
  DBGPStruct.Struct = &DBGEStruct;
  DBGPStruct.Path = Path;
  execelf(lib_path[DBG], &DBGPStruct);
}

void loadlib_gprs()
{
   GPRSPStruct.trafic_size = (int)trafic_size;
   GPRSPStruct.trafic_gold = (int)trafic_gold;
   GPRSPStruct.trafic_silver = (int)trafic_silver;
   GPRSPStruct.trafic_blur = (int)trafic_blur;
   GPRSPStruct.text_valute = text_valute;
   GPRSPStruct.LIMIT_TRAF_ACTIVE = (int)LIMIT_TRAF_ACTIVE;
   GPRSPStruct.LIMIT_TRAF_KB =(int)LIMIT_TRAF_KB;
   GPRSPStruct.LIMIT_TRAF_SEC = (int)LIMIT_TRAF_SEC;  
   GPRSPStruct.session_time_enable = (int)session_time_enable;
  GPRSPStruct.Struct = &GPRSEStruct;
  execelf(lib_path[GPRS], &GPRSPStruct);
}

void loadlib_mesg()
{
  MESGPStruct.TypeMes = (int)TypeMes;
  MESGPStruct.Struct = &MESGEStruct;
  execelf(lib_path[MESG], &MESGPStruct);
}

void loadlib_str()
{
  STRPStruct.Struct = &STREStruct;
  execelf(lib_path[STR], &STRPStruct);
}

void loadlib_type()
{
  TYPEPStruct.reverse = &(*STREStruct.reverse);
  TYPEPStruct.Struct = &TYPEEStruct;
  execelf(lib_path[TYPE], &TYPEPStruct);
}

void SetFunc()
{
   WriteStr = STREStruct.WriteStr;
   strcmp_nocase2 = STREStruct.strcmp_nocase;
   MakeGlobalString2 = STREStruct.MakeGlobalString;
   reverse = STREStruct.reverse;
   InitLgp = STREStruct.InitLgp;
   InitLgp_2 = STREStruct.InitLgp_2;
   CreateLgp = STREStruct.CreateLgp;
   CreateLgp_2 = STREStruct.CreateLgp_2;
   GetLgpInfo = STREStruct.GetLgpInfo;
   WriteDebug = DBGEStruct.WriteDebug;
   WriteDebugSf = DBGEStruct.WriteDebugSf;
   DumpAdr = DBGEStruct.DumpAdr;
   GetDateTimeNow = DATEEStruct.GetDateTimeNow;
   WriteStr = STREStruct.WriteStr;
   vsprintf2 = STREStruct.vsprintf2;
   IntToStr = TYPEEStruct.IntToStr;
   itoa = TYPEEStruct.itoa;
   atoi = TYPEEStruct.atoi;
   RestartGPRS = GPRSEStruct.RestartGPRS;
   GetSesInfo = GPRSEStruct.GetSesInfo;
   GetSessionTraffic = GPRSEStruct.GetSessionTraffic;
   GetAllTraffic = GPRSEStruct.GetAllTraffic;
   GetCharTraffic = GPRSEStruct.GetCharTraffic;
   GetCharMoney = GPRSEStruct.GetCharMoney;
   GetSessionTime = GPRSEStruct.GetSessionTime;
   CheckLimit = GPRSEStruct.CheckLimit;
   ShowMessage = MESGEStruct.ShowMessage;  
}

int loadlib()
{
   set_libs();
  loadlib_str();
    STREStruct =  *STRPStruct.Struct;
  if (!STREStruct.LibID == 0)
    return 0;
  loadlib_date();
    DATEEStruct =  *DATEPStruct.Struct;
  if (!DATEEStruct.LibID == 1)
    return 0;
  loadlib_dbg();
    DBGEStruct =  *DBGPStruct.Struct;
  if (!DATEEStruct.LibID == 2)
    return 0;
  loadlib_type();
    TYPEEStruct =  *TYPEPStruct.Struct;
  if (!DATEEStruct.LibID == 3)
    return 0;
  loadlib_gprs();
    GPRSEStruct =  *GPRSPStruct.Struct;
  if (!GPRSEStruct.LibID == 4)
    return 0;
  loadlib_mesg();
    MESGEStruct =  *MESGPStruct.Struct;
  if (!MESGEStruct.LibID == 5)
    return 0;
  
  SetFunc();
         
  LoadLib = 1; 
    return 1;
}

int closelib()
{
  if (!MESGEStruct.LibID == 5)
    return 0;
  MESGEStruct.CloseLib();
  if (!GPRSEStruct.LibID == 4)
    return 0;
  GPRSEStruct.CloseLib();
  if (!TYPEEStruct.LibID == 3)
    return 0;
  TYPEEStruct.CloseLib();
  if (!DBGEStruct.LibID == 2)
    return 0;
  DBGEStruct.CloseLib();
  if (!DATEEStruct.LibID == 1)
    return 0;
  DATEEStruct.CloseLib();
  if (!STREStruct.LibID == 0)
    return 0;
  STREStruct.CloseLib();  
  LoadLib = 0;
  free_libs();
  
    return 1;
}

