#ifndef _LANGPACK_H_
#define _LANGPACK_H_

enum LGP_ID
{
/// menu
  LGP_Connect=0,
  LGP_SetStatus,
  LGP_CList,
  LGP_Option,
  LGP_About,
  LGP_Exit,
///

  LGP_DATA_NUM
};

  void LoadLanguage();
  void FreeLanguage();
  
  char * LangPack[LGP_DATA_NUM];

#endif
