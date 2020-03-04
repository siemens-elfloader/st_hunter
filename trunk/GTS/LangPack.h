

#ifndef __LANG_PACK_H__
#define __LANG_PACK_H__

#include <string>
using namespace std;

enum
{
   EnterText=0,
   Translate,
   Options,
   Clean,
   Empty,
   DoTranslate,
   OriginalLang,
   TranslateLang,
   Trafic,
   AccountingTraffic,
   Aboute,
   Exit,
   Completion,
   PleaseEnterText,
   Translating,
   Clear,
   Back,
   FirstEnableGPRS,
   ErrorConnecting,
   Error,
   Send,
   Received,
   AllTrafic,
   SelectOriginalLang,
   SelectTranslateLang
};

class LangPack
{
public:
   LangPack(char *langpatch);
   //~LangPack();

   void SetDefaultLang();
   char * ParseLangId(char *s, int id);

   string lang[30];
};



#endif
