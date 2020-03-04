
#include <swilib.h>
#include "LangPack.h"


/* где раньше был еп3 и гцц? Тут нужно заюзать все прелести C++ */
/* коешо правда уже переделал... */


int _memcmp(const char *s, const char *d)
{
    if(!s) return 0;
    int p=0;
    int l = strlen(d);
    while( *s++ == *d++ && p<l)p++;
    if(p==l)
        return 0;
    else
        return 1;
}


char * LangPack :: ParseLangId(char *s, int id)
{
    //char *to = *too;
    char *st = 0;
    while(*s++ != '=');
    while(*s++ != '"');
    st = s;
    while(*s++ != '"');
    s--;
    lang[id].assign(st, s-st);

    return s;
}


LangPack :: LangPack(char *langpatch)
{
    //parse

    FSTATS fstat;
    GetFileStats(langpatch, &fstat, 0);
    int fsize = fstat.size;
    int fp = _open(langpatch, A_ReadOnly+A_BIN, P_READ, 0);
    if(fp==-1)
    {
        SetDefaultLang();
        return;
    }

    char *buffer = new char [fsize+2];
    _read(fp, buffer, fsize, 0);
    _close(fp, 0);
    char *s = buffer;


    /* нихуя-се я дал, AStyle ещё добавил о_О */
    do
    {
        if(!_memcmp(s, "TranslateLang"))
        {
            s = ParseLangId(s, TranslateLang);
            //debug("TranslateLang: %s", lang[TranslateLang]);
        }
        else

            if(!_memcmp(s, "EnterText"))
            {
                s = ParseLangId(s, EnterText);
                //debug("EnterText: %s", lang[EnterText]);
            }
            else

                if(!_memcmp(s, "Translate"))
                {
                    s = ParseLangId(s, Translate);
                    //debug("Translate: %s", lang[Translate]);
                }
                else

                    if(!_memcmp(s, "Options"))
                    {
                        s = ParseLangId(s, Options);
                        //debug("Options: %s", lang[Options]);
                    }
                    else

                        if(!_memcmp(s, "Clean"))
                        {
                            s = ParseLangId(s, Clean);
                            //debug("Clean: %s", lang[Clean]);
                        }
                        else

                            if(!_memcmp(s, "Empty"))
                            {
                                s = ParseLangId(s, Empty);
                                //debug("Empty: %s", lang[Empty]);
                            }
                            else

                                if(!_memcmp(s, "DoTranslate"))
                                {
                                    s = ParseLangId(s, DoTranslate);
                                    //debug("DoTranslate: %s", lang[DoTranslate]);
                                }
                                else

                                    if(!_memcmp(s, "OriginalLang"))
                                    {
                                        s = ParseLangId(s, OriginalLang);
                                        //debug("OriginalLang: %s", lang[OriginalLang]);
                                    }
                                    else


                                        if(!_memcmp(s, "Trafic"))
                                        {
                                            s = ParseLangId(s, Trafic);
                                            //debug("Trafic: %s", lang[Trafic]);
                                        }
                                        else

                                            if(!_memcmp(s, "AccountingTraffic"))
                                            {
                                                s = ParseLangId(s, AccountingTraffic);
                                                //debug("AccountingTraffic: %s", lang[AccountingTraffic]);
                                            }
                                            else

                                                if(!_memcmp(s, "About"))
                                                {
                                                    s = ParseLangId(s, Aboute);
                                                    //debug("About: %s", lang[Aboute]);
                                                }
                                                else

                                                    if(!_memcmp(s, "Exit"))
                                                    {
                                                        s = ParseLangId(s, Exit);
                                                        //debug("Exit: %s", lang[Exit]);
                                                    }
                                                    else

                                                        if(!_memcmp(s, "Completion"))
                                                        {
                                                            s = ParseLangId(s, Completion);
                                                            //debug("Completion: %s", lang[Completion]);
                                                        }
                                                        else

                                                            if(!_memcmp(s, "PleaseEnterText"))
                                                            {
                                                                s = ParseLangId(s, PleaseEnterText);
                                                                //debug("PleaseEnterText: %s", lang[PleaseEnterText]);
                                                            }
                                                            else

                                                                if(!_memcmp(s, "Translating"))
                                                                {
                                                                    s = ParseLangId(s, Translating);
                                                                    //debug("Translating: %s", lang[Translating]);
                                                                }
                                                                else

                                                                    if(!_memcmp(s, "Clear"))
                                                                    {
                                                                        s = ParseLangId(s, Clear);
                                                                        //debug("Clear: %s", lang[Clear]);
                                                                    }
                                                                    else

                                                                        if(!_memcmp(s, "Back"))
                                                                        {
                                                                            s = ParseLangId(s, Back);
                                                                            //debug("Back: %s", lang[Back]);
                                                                        }
                                                                        else

                                                                            if(!_memcmp(s, "FirstEnableGPRS"))
                                                                            {
                                                                                s = ParseLangId(s, FirstEnableGPRS);
                                                                                //debug("FirstEnableGPRS: %s", lang[FirstEnableGPRS]);
                                                                            }
                                                                            else

                                                                                if(!_memcmp(s, "ErrorConnecting"))
                                                                                {
                                                                                    s = ParseLangId(s, ErrorConnecting);
                                                                                    //debug("ErrorConnecting: %s", lang[ErrorConnecting]);
                                                                                }
                                                                                else

                                                                                    if(!_memcmp(s, "Error"))
                                                                                    {
                                                                                        s = ParseLangId(s, Error);
                                                                                        //debug("Error: %s", lang[Error]);
                                                                                    }
                                                                                    else

                                                                                        if(!_memcmp(s, "Send"))
                                                                                        {
                                                                                            s = ParseLangId(s, Send);
                                                                                            //debug("Send: %s", lang[Send]);
                                                                                        }
                                                                                        else

                                                                                            if(!_memcmp(s, "Received"))
                                                                                            {
                                                                                                s = ParseLangId(s, Received);
                                                                                                //debug("Received: %s", lang[Received]);
                                                                                            }
                                                                                            else

                                                                                                if(!_memcmp(s, "AllTrafic"))
                                                                                                {
                                                                                                    s = ParseLangId(s, AllTrafic);
                                                                                                    //debug("AllTrafic: %s", lang[AllTrafic]);
                                                                                                }
                                                                                                else

                                                                                                    if(!_memcmp(s, "SelectOriginalLang"))
                                                                                                    {
                                                                                                        s = ParseLangId(s, SelectOriginalLang);
                                                                                                        //debug("SelectOriginalLang: %s", lang[SelectOriginalLang]);
                                                                                                    }
                                                                                                    else

                                                                                                        if(!_memcmp(s, "SelectTranslateLang"))
                                                                                                        {
                                                                                                            s = ParseLangId(s, SelectTranslateLang);
                                                                                                            //debug("SelectTranslateLang: %s", lang[SelectTranslateLang]);
                                                                                                        }

    }
    while(*s++);


    delete buffer;
}

/*
LangPack :: ~LangPack()
{

}
*/

void LangPack :: SetDefaultLang()
{
    lang[EnterText] = "Введите текст";

    lang[Translate] = "Перевод";

    lang[Options] = "Опции";

    lang[Clean] = "Очистить";

    lang[Empty] = "Пусто";

    lang[DoTranslate] = "Перевести";

    lang[OriginalLang] = "Язык оригинала";

    lang[TranslateLang] = "Язык перевода";

    lang[Trafic] = "Трафик";

    lang[AccountingTraffic] = "Учет трафика";

    lang[Aboute] = "О программе";

    lang[Exit] = "Выход";

    lang[Completion] = "Завершение";

    lang[PleaseEnterText] = "Пожалуйста введите текст!";

    lang[Translating] = "Перевод...";

    lang[Clear] = "Понятно";

    lang[Back] = "Назад";

    lang[FirstEnableGPRS] = "Сначало включите GPRS";

    lang[ErrorConnecting] = "Ошибка подключения";

    lang[Error] = "Ошибка";

    lang[Send] = "Отправлено";

    lang[Received] = "Принято";

    lang[AllTrafic] = "Всего";

    lang[SelectOriginalLang] = "Выберите язык оригинала";

    lang[SelectTranslateLang] = "Выберите язык перевода";

}
