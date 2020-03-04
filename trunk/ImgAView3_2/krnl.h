#define MAX_PIC 2000

void DoSend ();

void PrintAbout();
void switchToIdle();
void SetWallpaper();
void DoMenu();
void DrawMenu();
void KeyMenu(int key);
void DoConfig();
void DoExit();
void _Exit();
void SlideShow();

char *GetFileExt(char* fname);
char *GetFileName(char* fname);
void SearchIMG (char *GlobalFolder);
//void Sort();
char *GetFileDir(char* fname, char* buf);
int FileExists (char *fname);

//////////
int GetImgCount ();
char *GetImgName (int id);
char *GetCurImgName ();
int GetCurPos ();
void SetCurPos (int id);
void PrepareToNext ();
int ImgAViewOnKey (int mess, int key);
void Init (char *fname);
void ImgAViewOnClose ();
void ImgAViewOnCreate ();

void DrawWindow ();

void UpdateCSMname();

void Log (char *txt);

/*
void showInt (int i){
  char str[64];
  sprintf(str, "%d", i);
  ShowMSG(1, (int)str);
}
*/
