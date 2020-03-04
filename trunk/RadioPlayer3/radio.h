
#define MAX_VOLUME 10
#define MAX_LEVEL 5

extern bool IsFontCfgUpdate;

void DrawWindow ();

void OnCloseWin ();

char *GetCurDir ();

int MyUIOnKey (int mess, int key);

void DoConfig ();

void UI_OnCreate ();
void UI_OnFocus ();
void UI_UnFocus ();

typedef struct{
  int freq;
  char name[64];
}RStation;

int CreateList (char *fname);
int GetStationCount ();
RStation GetStation (int id);
int GetCurFreq ();
void SetFreq (int freq);
void freq2str (char *str, int f);
int SearchCurStation ();

int OnEnterList (int mess, int key, int id);

int GetVolume ();
int GetLevel ();
int GetStatus ();
void UpdateInfo ();

void RunRadio ();
void RadioStop ();
bool IsStop ();
void SetStop ();

int SmartHook (int key, int mess);

void FreqDec();
void FreqInc();

void NextStation ();
void PrevStation ();

void SwitchToIdle();
void KeyLock ();

#define MakeFunc(prot,name,addr) prot* const name =  (prot*)addr

typedef void V_TYPE_V (void);
typedef void V_TYPE_I (int);
typedef int I_TYPE_I (int);
typedef void V_TYPE_IPI (int, int *ptr, int);
typedef int I_TYPE_V (void);
typedef int I_TYPE_HObj (HObj hObj);
typedef int I_TYPE_HObj_I (HObj hObj, int);

MakeFunc(V_TYPE_V,fmdl_set_mute,0xA05783F0);
//void fmdl_set_mute ();


MakeFunc(V_TYPE_V,fmdl_auto_search,0xA0578BD4);
//void fmdl_auto_search() 0xA0578BD4
//fmdl_auto_search (freq, &ptr, 1);

MakeFunc(I_TYPE_I,fmdl_get_stereo_status,0xA0579120);
//int fmdl_get_stereo_status() 0xA0579120

MakeFunc(I_TYPE_I,fmdl_get_current_level,0xA05792FC);
//int fmdl_get_current_level() 0xA05792FC

MakeFunc(I_TYPE_V,fmdl_get_mute,0xA0578508);
//int fmdl_get_mute() 0xA0578508

MakeFunc(I_TYPE_V,fmdl_get_forced_mono,0xA0577FB4);
//int fmdl_get_forced_mono () 0xA0577FB4

MakeFunc(V_TYPE_I,fmdl_set_power,0xA0577988);
//void fmdl_set_power (int) 0xA0577988

MakeFunc(V_TYPE_I,fmdl_set_forced_mono,0xA05780B8);
//void fmdl_set_forced_mono (int) 0xA05780B8


MakeFunc(I_TYPE_HObj_I,Obs_Sound_SetLogSourceEx,0xA0CC3964);
//int Obs_Sound_SetLogSourceEx (HObj hObj) 0xA0CC3964

MakeFunc(I_TYPE_HObj_I,Obs_Snd_SetPurpose,0xA0CC407C);
//int Obs_Snd_SetPurpose (HObj hObj) 0xA0CC407C

MakeFunc(I_TYPE_HObj_I,Obs_Sound_SetHandsFree,0xA0CC3FB8);
//int Obs_Sound_SetHandsFree (HObj hObj, int unk) 0xA0CC3FB8

MakeFunc(V_TYPE_IPI, fmdl_set_freq, 0xA0578A3C);
//defadr	fmdl_set_freq,0xA0578A3C
//defadr  fmdl_get_current_level,0xA05792FC
//defadr  fmdl_get_stereo_status,0xA0579120

