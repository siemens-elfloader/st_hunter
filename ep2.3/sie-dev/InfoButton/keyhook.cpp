#include "../inc/swilib.h"
#include "main.h"

extern const int CFG_ACTIVATION_STYLE;
extern const int CFG_KEY_OPEN;
extern int CSM_ID;

int Keyhook(int submess, int msg)
{
    if(!CSM_ID && submess==CFG_KEY_OPEN)
    {
        if(CFG_ACTIVATION_STYLE==0)  //коротокое нажатие
        {
            CreateOpenCSM(); //делаем то, что нам надо
            return KEYHOOK_BREAK;
        }
        else    //длинное нажатие
        {
            static int mode;
            switch(msg)
            {
                case KEY_DOWN:
                    if(mode==1)
                    {
                        mode=0;
                        return KEYHOOK_NEXT;
                    }
                    else
                    {
                        mode=0;
                        return KEYHOOK_BREAK;
                    }
                case LONG_PRESS:
                    mode=2;
                    CreateOpenCSM(); //делаем, то что нам надо
                return KEYHOOK_BREAK;
                case KEY_UP:
                    if(!mode)
                    {
                        mode=1;
                        GBS_SendMessage(MMI_CEPID, KEY_DOWN, CFG_KEY_OPEN);
                        return KEYHOOK_BREAK;
                    }
                break;
            }
        }
    }
    return KEYHOOK_NEXT;
}

void InitKeyhook()
{
    AddKeybMsgHook((void*)Keyhook);
}

void DestroyKeyhook()
{
    RemoveKeybMsgHook((void*)Keyhook);
}
