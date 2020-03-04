
#include <swilib.h>
#include "FormBuilder/LForm.h"
#include "main.h"
#include "lang.h"
#include "LangConfig.h"
#include "LangPack.h"



extern LangPack *lgp;
LForm *input_lang, *output_lang;
int inputlang=0, outputlang=1;
extern char dir[], temp[];

   int inpen=0;
   int outpen=0;

string lang[2][50];
string inlang[2][50];

/*
char *lang[2][5] =
{
   {"Русский", "Английский", "Украинский", "Немецкий", "Французкий"},
   {"ru",      "en",         "uk",         "de",       "fr"}

};

char *inlang[2][6] =
{
   {"Автоматически", "Русский", "Английский", "Украинский", "Немецкий", "Французкий"},
   {"auto",          "ru",      "en",         "uk",         "de",       "fr"}

};
*/


//  ========= выбор исходного языка ===========

int ShowMenuInput()
{
   input_lang->Show();
   return 1;
}

int MenuInputItemSelect()
{
   inputlang = input_lang->GetCurentMenuItem();
   myread->SetHeadString();
   return 1;
}


//  ============== выбор целевого языка =============

int ShowMenuOutput()
{
    output_lang->Show();
    return 1;
}

int MenuOutputItemSelect()
{
    outputlang = output_lang->GetCurentMenuItem();
    myread->SetHeadString();
    return 1;
}


void SetDefaultLang()
{
      inlang[0][0] = "Автоматически";
      inlang[1][0] = "auto";

      inlang[0][1] = "Русский";
      inlang[1][1] = "ru";

      inlang[0][2] = "Английский";
      inlang[1][2] = "en";

      inlang[0][3] = "Украинский";
      inlang[1][3] = "uk";

      inlang[0][4] = "Немецкий";
      inlang[1][4] = "de";

      inlang[0][5] = "Французкий";
      inlang[1][5] = "fr";

      /******/

      lang[0][0] = "Русский";
      lang[1][0] = "ru";

      lang[0][1] = "Английский";
      lang[1][1] = "en";

      lang[0][2] = "Украинский";
      lang[1][2] = "uk";

      lang[0][3] = "Немецкий";
      lang[1][3] = "de";

      lang[0][4] = "Французкий";
      lang[1][4] = "fr";

      inpen=6;
      outpen=5;

}


void Start()
{
   extern void AllocLangEditor();
   AllocLangEditor();

   char *buffer;
   zeromem(inlang, sizeof(inlang));
   zeromem(lang, sizeof(lang));

   sprintf(temp, "%slanguage.config", dir);

   int fp = _open(temp, A_ReadOnly+A_BIN, P_READ, 0);
   if(fp==-1)
   {
      SetDefaultLang();
   }
   else
   {
    int size = _lseek(fp, 0, S_END, 0, 0);
    _lseek(fp, 0, S_SET,0,0);

    buffer = new char[size+1];

   _read(fp, buffer, size, 0);
   buffer[size]=0;

   inpen = ParseToMssivInLang(buffer, inlang);
   outpen = ParseToMassivOutLang(buffer, lang);


   delete buffer;
   _close(fp,0);
   }

   input_lang = new LForm (LForm::Line);

   sprintf(temp, "%sRes\\google.png", dir);
   input_lang->SetHeader(temp, lgp->lang[SelectOriginalLang].c_str());

   for(int i=0; i<inpen; i++)
   {
      input_lang->AddItemElement(0, inlang[0][i].c_str(), MenuInputItemSelect);
   }

   output_lang = new LForm (LForm::Line);

   sprintf(temp, "%sRes\\google.png", dir);
   output_lang->SetHeader(temp, lgp->lang[SelectTranslateLang].c_str());

   for(int i=0; i<outpen; i++)
   {
      output_lang->AddItemElement(0, lang[0][i].c_str(), MenuOutputItemSelect);
   }

   extern void AllocTrafic();
   AllocTrafic();
}


void Close()
{
   extern void FreeLangEditor();
   FreeLangEditor();

   extern void FreeTrafic();
   FreeTrafic();
   delete input_lang;
   delete output_lang;
}
