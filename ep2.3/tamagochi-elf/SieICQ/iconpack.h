#ifndef _ICONPACK_H_
#define _ICONPACK_H_

#define LGP_SMILE_OK 0xE41F // :)
#define LGP_SMILE_NO 0xE43D // :(

enum ICON_ID
{
  IMG_Logo,
  IMG_UNKNOWN,
  IMG_OFFLINE,
  IMG_ONLINE,
  IMG_AWAY,
  IMG_DND,
  IMG_NA, 
  IMG_OCCUPIED,
  IMG_FFC,
  IMG_INVISIBLE,
  
  TOTAL_ICONS_NUM
};

class IconPack
{
public:
  IconPack();
  ~IconPack();
  
  void Setup();
  void Free();
  
  static IconPack * Active;
  
  int data[TOTAL_ICONS_NUM];
};

#endif
