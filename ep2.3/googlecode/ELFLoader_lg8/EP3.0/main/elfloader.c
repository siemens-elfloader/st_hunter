#include "..\..\inc\swilib.h"
#include "..\cfg\conf_loader.h"

extern long elfload(char *filename, void *param1, void *param2, void *param3);
extern int get_file_size(const char * fname);

extern int EDL_system_init();
extern int EDL_system_reinit();
extern int EDL_system_kill();
extern int EDL_load_ep(char *fname);


/*  Оповещение об ошибки запуск? */ 

#define TYPE_ELF 0
#define TYPE_EDL 1

__arm int executer_and_messager(char *filename, void *param1, char type)
{
  char msg[96];
  
  int err;
  if (type == TYPE_ELF) err = elfload(filename,param1,0,0);
                   else err = EDL_load_ep(filename);
  if (err >= 0) return 0;
  else
   {
    char *fn = strrchr(filename,(int)'\\');
    sprintf(msg, "EP 3.0:\n%s\nerr: %d!", fn+1, err);
    MsgBoxError(1, (int)msg);
    return 1;       // Oшибк?запуск?
   }
}

/*  Вызо?из Card-Explorer'a ил?My Stuff ил?ExecuteFile()  */
__arm int elf_onload(WSHDR *filename, WSHDR *ext, void *param)
{
  char fn[128];
  ws_2str(filename,fn,126);
  if (fn[1] != ':') param = 0;
  if (executer_and_messager(fn, param, TYPE_ELF)) return 0; else return 1;
}

__arm int edl_onload(WSHDR *filename, WSHDR *ext, void *param)
{
  char fn[128];
  ws_2str(filename,fn,126);
  if (executer_and_messager(fn, 0, TYPE_EDL)) return 0; else return 1;
}



extern void(*OldOnClose)(void *);
extern void(*OldOnCreate)(void *);


extern unsigned int DEFAULT_DISK_N;
extern void InitPngBitMap(void);








/* ---------------- Loading EDL Libs ---------------- */
__arm void LoadLibs()
{
  DIR_ENTRY de;
  unsigned int err;
  unsigned int pathlen;
  char name[256];
  strcpy(name+1,":\\ZBin\\Libs\\");
  name[0]=DEFAULT_DISK_N+'0';
  pathlen=strlen(name);
  strcat(name,"*.edl");
  if (FindFirstFile(&de,name,&err))
  {
    do
    {
      name[pathlen]=0;
      strcat(name,de.file_name);
      executer_and_messager(name, 0, TYPE_EDL);
    }
    while(FindNextFile(&de,&err));
  }
  FindClose(&de,&err);
}

/* ---------------- Loading Daemons ---------------- */

__arm void LoadDaemons()
{
  DIR_ENTRY de;
  unsigned int err;
  unsigned int pathlen;
  char name[256];
  strcpy(name+1,":\\ZBin\\Daemons\\");
  name[0]=DEFAULT_DISK_N+'0';
  pathlen=strlen(name);
  strcat(name,"*.elf");
  if (FindFirstFile(&de,name,&err))
  {
    do
    {
      name[pathlen]=0;
      strcat(name,de.file_name);
      executer_and_messager(name, 0, TYPE_ELF);
    }
    while(FindNextFile(&de,&err));
  }
  FindClose(&de,&err);
}



//ELF
__no_init char smallicons_str[32];
__no_init char bigicons_str[32];

//EDL
__no_init char smallicons_edl_str[32];
__no_init char bigicons_edl_str[32];





__arm void MyIDLECSMonCreate(void *data)
{
  //ELF
  static const int smallicons[2]={(int)smallicons_str,0};
  static const int bigicons[2]={(int)bigicons_str,0};
  //EDL
  static const int smallicons_edl[2]={(int)smallicons_edl_str,0};
  static const int bigicons_edl[2]={(int)bigicons_edl_str,0};
  
  
#ifdef NEWSGOLD
  static const REGEXPLEXT elf_reg=
  {
    "elf",
    0x55,
    0xFF,
    8, //Катало?Misc
    MENU_FLAG2,
    smallicons,
    bigicons,
    (int)"Open",    //LGP "Открыт?
    (int)"AltOpen", //LGP "Опци?
    LGP_DOIT_PIC,
    (void *)elf_onload,
    0
  };
  
  static const REGEXPLEXT edl_reg=
  {
    "edl",
    0xFFFF,
    0xFF,
    8, //Катало?Misc
    MENU_FLAG2,
    smallicons_edl,
    bigicons_edl,
    (int)"Open",    //LGP "Открыт?
    (int)"AltOpen", //LGP "Опци?
    LGP_DOIT_PIC,
    (void *)edl_onload,
    0
  };
  
#else
  static const REGEXPLEXT elf_reg=
  {
    "elf",
    0x55,
    0xFF,
    7,
    MENU_FLAG2,
    smallicons,
    bigicons,
    (void *)elf_onload,
    0
  };
  
  static const REGEXPLEXT edl_reg=
  {
    "edl",
    0xFFFF,
    0xFF,
    7,
    MENU_FLAG2,
    smallicons_edl,
    bigicons_edl,
    (void *)edl_onload,
    0
  };
#endif
  
  InitConfig();

  extern void CreateEPC_PROC();
  CreateEPC_PROC();
  
  extern void LoadLibrary();
  LoadLibrary();
  EDL_system_init();
  
  InitPngBitMap();
  
  //ELF
  strcpy(smallicons_str+1,":\\ZBin\\img\\elf_small.png");
  strcpy(bigicons_str+1,":\\ZBin\\img\\elf_big.png");
  smallicons_str[0]=bigicons_str[0]=DEFAULT_DISK_N+'0';
  
  //EDL
  strcpy(smallicons_edl_str+1,":\\ZBin\\img\\edl_small.png");
  strcpy(bigicons_edl_str+1,":\\ZBin\\img\\edl_big.png");
  smallicons_edl_str[0]=bigicons_edl_str[0]=DEFAULT_DISK_N+'0';
  
  
  RegExplorerExt(&elf_reg);
  RegExplorerExt(&edl_reg);
  
  extern int edl_ena;
  if (edl_ena) LoadLibs();
  
  extern int subproc_daem;
  if (subproc_daem == 1)
  SUBPROC((void *)LoadDaemons);
  else LoadDaemons();
  
  
  extern BXR1(void *, void (*)(void *));
  BXR1(data,OldOnCreate);
}

__arm void MyIDLECSMonClose(void *data)
{
  extern BXR1(void *, void (*)(void *));
  extern void KillEPC_PROC();
  KillEPC_PROC();
  //EDL_system_kill();
  BXR1(data,OldOnClose);
}





//Патч?
#pragma diag_suppress=Pe177
__root static const int NEW_ONCREATE @ "PATCH_ONCREATE" = (int)MyIDLECSMonCreate;
__root static const int NEW_ONCLOSE @ "PATCH_ONCLOSE" = (int)MyIDLECSMonClose;
#pragma diag_default=Pe177

