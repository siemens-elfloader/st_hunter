#include "../inc/swilib.h"
#include "../inc/cfg_items.h"
#include "conf_loader.h"

typedef struct{
  unsigned char
  vibra_status:1,
  sound_status:1,
  off_contacts,
  show_groups;
#ifdef ELKA
  unsigned char sli_state:1;
#endif
  unsigned char smartcpu_state;
  unsigned char inc_state:1;
  unsigned char ilu_state:1;
  unsigned char remind_state;
  unsigned char ping_state;
  unsigned char uptime_state;
#ifdef NEWSGOLD
  unsigned char Id3tag_status;
#endif
  unsigned char isXTX;//show x-status or online/offline time
}DEF_SETTINGS2;

DEF_SETTINGS2 def_se[35];
  
int main()
{

  for(int i=0;i<35;i++)
  {
    def_se[i].vibra_status=i;//сохраняем текущее состояние флагов
    def_se[i].sound_status=i;
    def_se[i].off_contacts=i;
    def_se[i].show_groups=i;
    def_se[i].sli_state=i;
    def_se[i].inc_state=i;
    def_se[i].remind_state=i;
    def_se[i].ilu_state=i;
    def_se[i].uptime_state=i;
    def_se[i].isXTX=i;//Показывать хстатус или время он лайн  
  }
  int f;
  unsigned int err;
  if ((f=fopen("4:\\def_set",A_WriteOnly+A_BIN+A_Create+A_Append,P_WRITE,&err))!=-1)
  {
    for(int i=0;i<35;i++)
    {
     fwrite(f,&def_se[i],sizeof(DEF_SETTINGS2),&err);    
    }
    ShowMSG(1,(int)"ok");
  }
    
  return 0;
}
