#ifndef _MC_H
#define _MC_H

#include "inc\fn_list.h"
#include "inc\exts.h"
#include "zlib\zlib.h"
#include "zlib\minizip\unzip.h"
#include "zlib\minizip\zip.h"

#include "inc\7zslib.h"
#include "inc\arch.h"

#include "inc\secret.h"
#include "inc\about.h"
#include "inc\popup.h"

#ifndef _SWILIB
#include "..\inc\swilib.h"
#define _SWILIB
#endif

#define version  "1.52"

#define MYNAME "MC"
#define IPC_SetCur 1
#define IPC_UPD_KEY 2
#define IPC_UPD_ALL 3

#define FALSE  0
#define TRUE  1

#define BUF_SIZE 0x4000

#ifdef NEWSGOLD
#define GEN 0x400
#define PICT 0x4000
#define VIDEO 0xA800
#define AUDIO_WAV 0x1000
#define AUDIO_MP3 0x1800
#else
#define PICT 0x8000
#define VIDEO 0x4000
#define AUDIO_WAV 0x2000
#endif

#define EXT_JPG "jpg"
#define EXT_JPEG "jpeg"
#define EXT_PNG "png"
#define EXT_GIF "gif"
#define EXT_BMP "bmp"

#define EXT_3GP "3gp"
#define EXT_MP4 "mp4"
#define EXT_MP3 "mp3"

#define MAX_DRV   4
#define DRV_IDX_Data 0
#define DRV_IDX_Cache 1
#define DRV_IDX_Config 2
#define DRV_IDX_MMC  3

//#define MAX_SHOW  8
#define MAX_TABS  2
#define systab   MAX_TABS
#define LONG_SCRL  itms_max
#define MAX_TEXT  19
#define MAX_TXT_W  (ITM_X2-ITM_B-TXT_X-2)

#define FNT_NONE  0
#define FNT_COPY  1
#define FNT_MOVE  2
#define FNT_ARH   3
#define FNT_DEL   4

#define ST_REVERS  0x0100
#define ST_NAME   0x0
#define ST_EXT   0x1
#define ST_SIZE   0x2
#define ST_DATE   0x3
#define STD_MASK  0x0100
#define STV_MASK  0x000F
#define ST_FIRST  ST_NAME
#define ST_LAST   ST_DATE

#define IDYES       0
#define IDNO        1
#define IDYES2ALL   2
#define IDNO2ALL    3

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

#ifdef ELKA
  #define HEADP 1
  #define SDVIG_H 28
  #define smes 8
#else
  #define HEADP 0
  #define SDVIG_H 8
  #define smes 3
#endif

extern char successed_config_filename[MAX_PATH];

#define def_new_dir  "New_dir"

#define FA_CHECK  0x1000

// for type in FILEINF
#define TYPE_COMMON_FILE 0
#define TYPE_COMMON_DIR  1
#define TYPE_ZIP_FILE  2
#define TYPE_ZIP_DIR  3
#define TYPE_7Z_FILE  4
#define TYPE_7Z_DIR   5

// fot ftype in FN_ITM

typedef struct
{
 short id;
 int ftype;
 char* sname;
 char* folder;
 short attr;
 unsigned int size;
 unsigned int csize;
 unsigned int time;
 int uccnt;
 int inited;
 WSHDR* ws_name;
 WSHDR* ws_attr;
 WSHDR* ws_size;
 WSHDR* ws_time;
 WSHDR* ws_ratio;
 WSHDR* ws_showname;
 EXTINF* ext;
 void* next;
} FILEINF;

typedef struct
{
 short ccFiles;
 short sort;
 short CurDrv;
 short iIndex[MAX_DRV];
 short iBase[MAX_DRV];
 char  szDirs[MAX_DRV][MAX_PATH];
 char  szFilter[MAX_PATH];
 ARCHINFO* zipInfo;
} TABINFO;

typedef struct
{
 int  num;
 char path[3];
 char name[7];
 int  enabled;
 //char *Icon;
 char icon[128];
} DRVINFO;

typedef struct
{
 int gsize;
 int gcsize;
 int count;
} CHKDS_PARAM;

typedef struct
{
 int sig;
 int ver;
 int size;
 int offset;
} mccfg_hdr;

typedef struct
{
	int number;
	char cleanName[MAX_PATH];
	char* cleanExt;
} ParseFileNameStruct;

typedef int (*ENUM_FILES_PROC) (DIR_ENTRY *de, int param);
typedef void (*IFN_RET_PROC) (WSHDR *wsname);
typedef int (*ENUM_SEL_PROC) (FILEINF *file, int param);


typedef int (*F_ENUM_PROC)(char* dname, ENUM_FILES_PROC enumproc, unsigned int param);
typedef int (*F_EXISTS_PROC)(char* fname);


#define MAX_INIBUF 256
typedef void (*INIPROC) (char* name, char* value);

typedef void (*KEY_PROC) ();

#define _CurTab   tabs[curtab]
#define _CurCount _CurTab->ccFiles
#define _CurDrv   _CurTab->CurDrv
#define _CurPath  _CurTab->szDirs[_CurDrv]
#define _CurIndex _CurTab->iIndex[_CurDrv]
#define _CurBase  _CurTab->iBase[_CurDrv]

//global vars
extern volatile int Terminate;
extern volatile int GUIStarted;
extern volatile int Busy;
extern volatile int Busy1;
extern unsigned int MAINGUI_ID;

extern int doredraw;

extern int show_hidden;
extern int show_system;
extern int show_hiddrv;

extern int MsgBoxResult;

extern int on_redraw;
extern int selmode;
extern NativeExplorerData* NatExpStr;

extern unsigned int err;

extern char* str_empty;

extern const char ext_file[];
extern const char keys_file[];
extern const char cfg_file[];
extern char etc_path[];

extern int progr_stop;
extern int progr_act;
extern int progr_max;
extern int progr_cur;
extern int progr_start;
extern double progrsp_max;
extern double progrsp_cur;
extern int progrsp_start;
extern char* pathbuf;
extern char* pathbuf2;
extern char* pathbuf_fn;
extern char* msgbuf;
extern char* zippathbuf;
extern WSHDR* wsbuf;
extern WSHDR* guibuf;
extern WSHDR* progr_name;
extern char* szLastNewFile;
extern char* szLastNewDir;
extern char* szLastNewArchive;
extern FN_LIST tmp_files;

extern int curtab;
extern TABINFO* tabs[];
extern FILEINF* FileListBase[];
extern DRVINFO Drives[];
extern EXTINF ext_chk;
extern EXTINF ext_dir;
extern char in_open_path[MAX_PATH];
extern char mcpath[MAX_PATH];
extern int back_tab;

extern int fa_attr;
extern int fa_sb;
extern FN_LIST buffer;

extern int Colors[];
extern const char mcbm_ext[];

extern SOFTKEYSTAB empty_menu_skt;
extern HEADER_DESC tmenuhdr;

extern GBSTMR offtm;

extern const char _fullnull[];
extern const char _null[];
extern const char _s[];
extern const char _d[];
extern const char _s_stars[];
extern const char _s_s[];
extern const char _s_sbcfg[];
extern const char _s_s_d[];
extern const char _ss[];
extern const char _t[];
extern const char _tc[];
extern const char _d_t[];
extern const char _s_i_s[];

extern const int CONFIG_GRAFIC_GUI;
extern const int CONFIG_GRAFIC_x;
extern const int CONFIG_GRAFIC_Y;

extern const int CONFIG_SHOW_HIDDEN;
extern const int CONFIG_SHOW_SYSTEM;
extern const int CONFIG_SHOW_SYSDRV;
extern const int CONFIG_SHOW_MMC;
extern const int CONFIG_SHOW_EXT;

extern const char graf_folder[128];
extern const int CONFIG_PP_CUR;

extern const int CONFIG_BACK_EXIT;
extern const int CONFIG_SAVE_PATH;

extern const int CONFIG_AUTO_EXIT_AFTER_MIN;
extern const int CONFIG_ENABLE_BACKGROUND_EXIT;

extern const int CONFIG_7Z_ENABLE;
extern const int CONFIG_7Z_DETECT_BY;
extern const int CONFIG_ZIP_ENABLE;
extern const int CONFIG_ZIP_DETECT_BY;
extern const int CONFIG_JAR_ENABLE;
extern const int CONFIG_SDT_ENABLE;
extern const char CONFIG_TEMP_PATH[64];
extern const int CONFIG_DELETE_TEMP_FILES_ON_EXIT;

extern const int CONFIG_MEDIA_ENABLE;

extern const int CONFIG_SCROLL_TEXT_SPEED;
extern const int CONFIG_SCROLL_TEXT_WAIT;

extern const int CONFIG_LOAD_ICONS;
extern const int CONFIG_LOAD_KEYS;
extern const int CONFIG_LOAD_MUI;
extern const int CONFIG_LOAD_CS;
extern const char CONFIG_MCCONFIG_PATH[64];

extern const int CONFIG_CONFIRM_EXIT;
extern const int CONFIG_CONFIRM_DELETE;
extern const int CONFIG_CONFIRM_DELETERO;
extern const int CONFIG_CONFIRM_REPLACE;
extern const int CONFIG_CONFIRM_MOVE;
extern const int CONFIG_CONFIRM_COPY;
extern const int cursave;
extern const int CONFIG_GREEN_BUUTON_TYPE;

extern const char CONFIG_VIEW_PUT[128];

extern const int CONFIG_SCROLL_STEP;
extern const int CONFIG_FREE_PNG_CACHE_ON_CLOSE;

extern const int FONT_HEAD;
extern const int FONT_SPISOK;
extern const int FONT_SOFT;
extern const int FONT_MENUNV;
extern const int FONT_MENUV;
extern const int FONT_PROP;
extern const int FONT_MSG;

extern const int Clock;
extern const int SMECHENIE;
extern const int CONFIG_USE_FUNCT;
extern const int CONFIG_LOAD_GRAF;

#ifdef LOG
void _WriteLog(char* buf);
void _WriteLogWS(WSHDR *buf);
void _WriteLog2(char* buf1, char* buf2);
void _WriteLog3(char *buf1, char *buf2);
void _WriteLogInt(char* buf, int ii);
void _WriteLogHex(char* buf, int ii);
#endif


//global procs
void UpdateCSMname(void);
void InitConfig();
char* MCFilePath(const char* sfile);
char* MCFilePluginsPath(const char* sfile);
FILEINF *_CurFile();
FILEINF *_CurTabFile(int tab);
void UpdateAll();
void SetTabIndex(int tab, int num, int slide);
int GetTabIndex(int tab);
char* GetTabPath(int tab);
void SetCurTabIndex(int num, int slide);
int SetTabDrv(int tab, int num);
int SetCurTabDrv(int num);
int GetFileIndex(int tab, char* fname);
int GetCurTabFileIndex(char* fname);
char* CurFullPath(FILEINF* sfile);
char* CurFullPathByName(char* sfile);
char* CurPath();
char* CurFullPathBM(char* buff, char* sfile);
char* TmpFullPath2(char* buff, char* sfile);
int GetFilesCnt(char* path);
int TestFileName(WSHDR *wsname);
void CorFileName(WSHDR *wsname);

int MsgBoxYesNoWithParam(int lgind, char* str);
int cd(int tab, char* dname);
int cdsys(char* dname);
void _cd_tab(int tab,int drv,char* dname);
int IsChkAll();
int ChkAll(int chk);
int InvChk();
void ChkFile(FILEINF *file);
int EnumChk(ENUM_SEL_PROC EnumProc, int param);
int EnumSel(ENUM_SEL_PROC EnumProc, int param);
int IsMultiChk();

int cdzip(char* dname);

int isNumericStr(char* s);

void nricp(char* src, char* dst);
int EnumIni(int local, char* ininame, INIPROC proc);
char* GetFileExt(char* fname);
char* strreplace(char* s, char cprev, char cnew);
char* strtolower(char* src, char* dst, int sz);
int stricmp(char* str1, char* str2);
int strnicmp(char* str1, char* str2, int count);
int stristr(char *s, char *find);
int strircmp(char* str1, char* str2);
int strnircmp(char* str1, char* str2, int count);
int wstrcmpi(WSHDR* ws1, WSHDR* ws2);

char* sz2s(unsigned int size, char* buf);
char* attr2s(int attr, char* buf);
char* fdt2s(unsigned int time, char* buf);

char *fd2s(unsigned int time, char *buf);
char *ft2s(unsigned int time, char *buf);

char* GetFileDir(char* fname, char* buf);
char* GetFileName(char* fname);

int fexist(char* fname);
int _EmptyEFP(DIR_ENTRY *de, int param);
int EnumFilesInDir(char* dname, ENUM_FILES_PROC enumproc, unsigned int param, int recursive, int enumDirs);
int EnumFiles(char* dname, ENUM_FILES_PROC enumproc, unsigned int param);
int mktree(char* path);
int fsrm(char* path, int ip);
int fscp(char* src, char* dst, int ip);
int fsmv(char* src, char* dst);
/*
void prep_hd(EDITCONTROL* ec);
void prep_inf(EDITCONTROL* ec);
*/
int _DirSize(DIR_ENTRY *de, int param);
int M_DirSize(FILEINF *file, int param);
int _SetAttr(DIR_ENTRY *de, int param);
void _NewDir(WSHDR *wsname);
void _NewFile(WSHDR *wsname);
void _Rename(WSHDR *wsname);
int M_SetAttr(FILEINF *file, int param);
int M_MoveCopy(FILEINF *file, int param);
int M_Add2arh(FILEINF *file, int param);
void S_ZipOpen(void);
void S_7ZOpen(void);

int InitTab(int tab);
void FreeTab(int tab);

FILEINF* CreateFileInfo(int findex, char* fnameOriginal,
    unsigned int fsize, short fattr, unsigned int ftime,
    int fcsize, int ftype, char* ffolderorg);
void FillFileInfo(FILEINF* file);
void FreeFileInfo(FILEINF* file);

int FillFiles(int tab, char* dname);
void DelFiles(int tab);
void LoadKeys();
void SortFiles(int tab);

void AddFile(int tab, int findex, char* fname, unsigned int fsize, short fattr,
      unsigned int ftime, int fcsize, int ftype, char* ffolder);
void AddFileFromDE(int tab, int findex, DIR_ENTRY* pde);

char GetKeynameIndexByKey(char key);
KEY_PROC GetKeyprocByKey(char key);
KEY_PROC GetKeyprocLongByKey(char key);

void ExitFromZip();
void ExitFrom7Z();
void DoKey(int isLongPress, int key);
void DoMenu();
int DoRefresh();
void DoFilter();
void DoOpen();
void DoAltOpen();
void DoNatExpl();
void DoSent();
void DoMenu();
void DoExit();
int DoBack();
void DoSwapTab();
void DoPrvDrv();
void DoNxtDrv();
void DoUp();
void DoDwn();
void DoPgUp();
void DoPgDwn();
void DoBegin();
void DoChk();
void DoChkAll();
void DoUnCAll();
void DoInvChk();
void DoDrvInf();
void DoNewDir();
void DoNewFile();
void DoRen();
void DoPaste();
void DoCancel();
void DoCopy();
void DoMove();
void DoDel();
void DoRoot();
void DoShowPath();
void DoErrKey();

void CB_Exit(int id);

void DoPlayList(int state);

void DoBMAdd();
void DoBMList();
void UseBM(char* filename);

void DoSortN();
void DoSortE();
void DoSortS();
void DoSortD();
void DoSetSort();
void DoSortR();

void DoTabCopy();
void DoTabMove();

void DoOptions();
void DoKeyHelp();
void DoAboutDlg();

void empty_ghook(GUI *gui, int cmd);
void empty_locret(void);
int empty_onkey(GUI *gui, GUI_MSG *msg);
int TextInput(char* caption, char* prmpt, int TestFileName, WSHDR *wsname, IFN_RET_PROC retproc);
void createTextView(WSHDR *ws);

void S_Paste(void);
void S_Delit(void);

void initprogr(int act);
void incprogr(int inc);
void incprogrset(int inc);
void incfname(char *name);
void incprogrsp(int inc);
void endprogr();
void endprogrsp();

void InitAllMD();

extern unsigned int MAINCSM_ID;

void ResetAutoExit();
void AutoExitProc();
// Инфа о дисках
extern int drwinfo;
void DrvInfo();
void DPROP_key(int key);
// из STDLIB.H
extern long  strtol (const char* nptr,char* *endptr,int base);

// Инфа о файле
extern int filepropshow;
void DrwFileProp();
void DoFilePropNew();
void fileprop_key(int key);
void FullPatch(WSHDR *wsname);
void FillInfo();
void FreeInfo();

int IsMedia(char* file);
int IsPict(char* file);
int IsVideo(char* file);


extern int fropon;

#ifdef NEWSGOLD
extern ZPARAMS zpp;
extern int total;
extern int s_arh_pos;
void DoArchive();
void DoArchClose();
void Arch_key(int key);
void DrawArch();
extern int arch;

int CreateZip(ZPARAMS *ZipP);
int Copy2Zip(zipFile file, char* fnamezip, char* fname, int level, char *password);
int ZipCopy(zipFile file, char* dst, char* src, char *pass);

int DeleteFileFromZIP();
#endif

int AddSearchFile(int tab);
extern int isseach;
extern int seach;
void DoSeach();
void DoSeachClose();
void Seach_key(int key);
void DrawSeach();
void FreeGrap();
extern WSHDR *Page_1, *Page_3;

void PeReDraw();

void DoGetFF();
extern int PassResult;

void SetWallpaper(char *param);
void SetLogo(char *param);
void SetScreenSaver(char *param);
void SetCall(char *param);
void SetMessages(char *param);
void SetAlarmClock(char *param);
void SetAppointments(char *param);

char *utf16_to_utf8(char **s, size_t *len);
void uni2dos(char* s, char *p, int maxlen);
void ws2char8(WSHDR* ws, char *p, int maxlen, int type);
void dos2utf8_2(char* s, char *utf8);
char* str2lower(char *st);
void allclose();

void DoCrtPL();

int file_inlist(FILEINF *list, char *name);
int isdir2(char* dname, unsigned int *Err);

void parse_name(char* fname, ParseFileNameStruct* pfns);
int find_next_name(ParseFileNameStruct* pfns);

int GetStrCount(WSHDR *prop,int width, int font);
int utf82win(char* utf8);

int Enum7ZFiles(ARCHINFO* pzi, char* subdname, ENUM_SEL_PROC enumproc, unsigned int param);
void DoUnPack();
void GetMD5(char *file);
void DoMD5();

void Win1251(WSHDR *ws1);
void RusTeg(WSHDR *ws1);
void DoLang();
void CB_Del(int id);
void SentNatExpStr();
#endif


