#ifndef _ICONPACK_H_
#define _ICONPACK_H_

enum ICON_ID
{
  // Status ;
  IMG_ONLINE,
  IMG_FFC,
  IMG_AWAY,
  IMG_DND,
  IMG_OCCUPIED,
  IMG_NA,
  IMG_INVISIBLE,
  IMG_EVIL,		
  IMG_DEPRESSION,	   
  IMG_HOME,		   
  IMG_WORK,	
  IMG_LUNCH,
  IMG_OFFLINE,
  IMG_GROUP_ON,
  IMG_GROUP_OFF,
  IMG_MESSAGE,
  IMG_TYPING,
  
  // MainMenu ; 
  IMG_TOCONNECT,
  IMG_CHANGE_S,
  IMG_CHANGE_XS,
  IMG_CLIST,
  IMG_SETTINGS,
  IMG_ABOUT,
  IMG_EXIT,
  
  // Interface
  IMG_LOGO,
  IMG_BG,
  
  // img client
  IMG_UNKNOW,
  IMG_SIEICQ,
  IMG_QUTIM,
  IMG_PIDGIN,
  IMG_NATICQ,
  IMG_MIRANDA,
  IMG_JIMM,
  IMG_QIP,
  IMG_QIP_INFINUM,
  IMG_ICQ,
  IMG_RAMBLER_ICQ,
  IMG_TRILLIAN,
  IMG_SIM,
  IMG_KOPETE,
  
  
  TOTAL_ICONS_NUM //
    
  // XStatus их много поэтому грузим по-другому


};
#define IMG_XStatus0 TOTAL_ICONS_NUM
#define XIMGCount 38


void LoadIcon();
void FreeIcon();
  
extern int IconPack[TOTAL_ICONS_NUM + XIMGCount];

#endif
