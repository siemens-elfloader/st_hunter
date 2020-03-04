#include "conf_loader.h"

int isElka();
void patch_header(const HEADER_DESC* head);
void CreateMMenu();
void ReadSettings();
void Get_Table();
void Actions();
void RunAction(char *s);
int ShowMenu(void);
void pass_pos_cler();
void EnterAction();
int ShowOptMenu(void);
void Casher();
typedef struct
{
  int *icon_inact;
  int *icon_big;
  int lgp_id;
  void (*run)();
} NATMENU_ITEM_DESC;

typedef struct
{
  int *icon_inact;
  int *icon_big;
  int lgp_id;
  int Anim;
  int pass;
  int CloseMenu;
//  char *passw;
  char *open;
  void (*run)();
} ITC;

typedef struct
{
  CSM_RAM csm;
}MAIN_CSM;

extern NATMENU_ITEM_DESC *Menu_Items;
extern ITC Items[12];

////////////////////////////////////////////////////////////////////////////////
extern const char name_1[15],
                  name_2[15],
                  name_3[15],
                  name_4[15],
                  name_5[15],
                  name_6[15],
                  name_7[15],
                  name_8[15],
                  name_9[15],
                  name_10[15],
                  name_11[15],
                  name_12[15];

extern const char open_1[128],
                  open_2[128],
                  open_3[128],
                  open_4[128],
                  open_5[128],
                  open_6[128],
                  open_7[128],
                  open_8[128],
                  open_9[128],
                  open_10[128],
                  open_11[128],
                  open_12[128];

extern const int anim_1,
                 anim_2,
                 anim_3,
                 anim_4,
                 anim_5,
                 anim_6,
                 anim_7,
                 anim_8,
                 anim_9,
                 anim_10,
                 anim_11,
                 anim_12;     

extern const int pc1,
                 pc2,
                 pc3,
                 pc4,
                 pc5,
                 pc6,
                 pc7,
                 pc8,
                 pc9,
                 pc10,
                 pc11,
                 pc12;  

extern const int p1,
                 p2,
                 p3,
                 p4,
                 p5,
                 p6,
                 p7,
                 p8,
                 p9,
                 p10,
                 p11,
                 p12;  

extern const int podn, 
                 smesh_x, 
                 smesh_y, 
                 smesh_pic_x, 
                 smesh_pic_y,
                 anim_on, 
                 anim_invert, 
                 anip, 
                 TIME, 
                 select_nom, 
                 possave, 
                 Menu_style; 

extern const char color1[4], color2[4];

extern const unsigned int ypos;
extern const char fontENcol[4],fontPAcol[4];
extern const RECT pos1,pos2;

extern const int prun;
extern const int prov;
extern const char passw[9];
extern const int glob_ani;

extern const int IDLE_BUTTON;
extern const int LongPress;
extern const int Cash;

extern const char BM1NAME[32],BM2NAME[32],BM3NAME[32],BM4NAME[32],BM5NAME[32],BM6NAME[32],BM7NAME[32],BM8NAME[32],BM9NAME[32];
extern const char BM1FILE[128],BM2FILE[128],BM3FILE[128],BM4FILE[128],BM5FILE[128],BM6FILE[128],BM7FILE[128],BM8FILE[128],BM9FILE[128];
////////////////////////////////////////////////////////////////////////////////
extern int UNI_YDISP;
extern int pic_n;
extern int m;                                      // constant defines a platform
extern const int minus11;

static const char per_t[]="%t";

extern unsigned int MAINCSM_ID;

extern IPC_REQ gipc;
extern int pos;
extern int ret;
extern GBSTMR tmr;

extern char pass[9];
extern int pass_pos;
extern int window_menu;
extern int start;
extern const char PicFon[128];
extern const int usePicFon;



