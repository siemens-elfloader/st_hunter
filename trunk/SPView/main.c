#include <swilib.h>
#include "menu.h"
#include "rect_patcher.h"
#include "lang.h"
#include "png_cache.h"

const char *s="%s";

unsigned int pic_id;
char main_folder[128];

typedef struct
{
    CSM_RAM csm;
    int gui_id;
} MAIN_CSM;

HEADER_DESC gui_hdr= {0,0,0,0, NULL, 0, LGP_NULL};

void update_header(GUI* gui)
{
    static char gui_hdr_text[64];
    IMGHDR *img=GetPITaddr(pic_id);
    sprintf(gui_hdr_text, "Pic ID: %d %dx%d", pic_id, img->w, img->h);
    gui_hdr.lgp_id=(int)gui_hdr_text;
}

unsigned int get_ws_width(WSHDR *ws, int font)
{
    return Get_WS_width(ws, font)+1;
}

void gui_redraw(GUI *data)
{
    int x1=0, y1=0, y_bg=
#ifdef ELKA
                     24;
#else
                     0;
#endif
    if(GetPITaddr(pic_id))
    {
        x1=(ScreenW()-GetImgWidth(pic_id))/2;
        y1=(ScreenH()-GetImgHeight(pic_id)+y_bg+HeaderH()-SoftkeyH())/2;
        DrawRectangle(x1-1, y1-1, x1+GetImgWidth(pic_id), y1+GetImgHeight(pic_id), 1,GetPaletteAdrByColorIndex(0),
                      GetPaletteAdrByColorIndex(23));
        DrawImg(x1, y1, pic_id);
    }
}

int guisoftkeys[]= {0,1,2};

SOFTKEY_DESC gui_sk[]=
{
    {0x0018,0x0000,(int)"Options"},
    {0x0001,0x0000,(int)"Exit"},
    {0x003D,0x0000,(int)"LGP_DOIT_PIC"}
};

SOFTKEYSTAB gui_skt=
{
    gui_sk,0
};

void gui_locret(void) {}


#ifdef ELKA
#define MAX_PIT 1698
#else
#define MAX_PIT 1755
#endif

void SetPicID(const int step, const int next)
{
    for(int i=step; i!=0; i--)
    {
        if(GetPITaddr(pic_id+i*next))
        {
            pic_id+=i*next;
            return;
        }
    }
    pic_id=(next==-1) ? MAX_PIT : 0;
}

int gui_onkey(GUI *data, GUI_MSG *msg)
{
    if(msg->keys==0xFFF)
        CreateOptionsMenu();
    if(msg->keys==0xFF0)
        return 1;

    int style=msg->gbsmsg->msg;
    int key=msg->gbsmsg->submess;
    char s[128];
    if(style==KEY_DOWN || style==LONG_PRESS)
    {
        switch(key)
        {
        case LEFT_BUTTON:
            SetPicID(10, -1);
            break;
        case DOWN_BUTTON:
            SetPicID(1, -1);
            break;
        case RIGHT_BUTTON:
            SetPicID(10, 1);
            break;
        case UP_BUTTON:
            SetPicID(1, 1);
            break;
        case '1':
            clear_bitmap();
            clear_cache();
            break;
                /*case '0':
                    sprintf(s, "%d", GetMaxDynIcon());
                    ShowMSG(1, (int)s);*/
            }
    }
    update_header(data);
    RefreshGUI();
    return 0;
}

void gui_ghook(GUI *data, int cmd)
{
    SOFTKEY_DESC sk= {0x0FFF,0x0000,(int)lgp[lgp_options]};
    SOFTKEY_DESC sk_exit= {0x0FF0,0x0000,(int)lgp[lgp_exit]};
    if (cmd==TI_CMD_CREATE)
    {
        static void *methods[16];
        void **m=GetDataOfItemByID(data,4);
        memcpy(methods, m[1], sizeof(methods));
        methods[0]=(void *)gui_redraw;
        m[1]=methods;
    }
    if(cmd==TI_CMD_FOCUS)
    {
        DisableIDLETMR();
        update_header(data);
    }
    if(cmd==TI_CMD_REDRAW)
    {
        SetSoftKey(data,&sk,SET_SOFT_KEY_N);
        SetSoftKey(data, &sk_exit, (SET_SOFT_KEY_N==0) ? 1 : 0);
    }
}


INPUTDIA_DESC gui_desc =
{
    1,
    gui_onkey,
    gui_ghook,
    (void *)gui_locret,
    0,
    &gui_skt,
    {0,0,0,0},
    FONT_SMALL,
    100,
    101,
    0,
    //  0x00000001 - Выровнять по правому краю
    //  0x00000002 - Выровнять по центру
    //  0x00000004 - Инверсия знакомест
    //  0x00000008 - UnderLine
    //  0x00000020 - Не переносить слова
    //  0x00000200 - bold
    0,
    //  0x00000002 - ReadOnly
    //  0x00000004 - Не двигается курсор
    //  0x40000000 - Поменять местами софт-кнопки
    0x40000000
};

int create_gui_ed(void)
{
    void *ma=malloc_adr();
    void *eq;

    EDITCONTROL ec;
    PrepareEditControl(&ec);
    eq=AllocEQueue(ma,mfree_adr());

    ConstructEditControl(&ec, ECT_READ_ONLY, ECF_INVISIBLE_CURSOR, '\0', 1);
    AddEditControlToEditQend(eq,&ec,ma);

    patch_header(&gui_hdr);
    patch_input(&gui_desc);

    return CreateInputTextDialog(&gui_desc, &gui_hdr,eq,1,0);
}

void maincsm_oncreate(CSM_RAM *data)
{
    MAIN_CSM*csm=(MAIN_CSM*)data;
    csm->csm.state=0;
    csm->csm.unk1=0;
    csm->gui_id=create_gui_ed();
}

void maincsm_onclose(CSM_RAM *csm)
{
    FreeLangPack();
    kill_elf();
}


int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
    MAIN_CSM *csm=(MAIN_CSM*)data;
    if (msg->msg==MSG_GUI_DESTROYED)
    {
        if ((int)msg->data0==csm->gui_id)
        {
            csm->csm.state=-3;
        }
    }
    return(1);
}


const int minus11=-11;
unsigned short maincsm_name_body[140];

const struct
{
    CSM_DESC maincsm;
    WSHDR maincsm_name;
} MAINCSM =
{
    {
        maincsm_onmessage,
        maincsm_oncreate,
#ifdef NEWSGOLD
        0,
        0,
        0,
        0,
#endif
        maincsm_onclose,
        sizeof(MAIN_CSM),
        1,
        &minus11
    },
    {
        maincsm_name_body,
        NAMECSM_MAGIC1,
        NAMECSM_MAGIC2,
        0x0,
        139,
        0
    }
};

void UpdateCSMname(void)
{
    wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"SPView");
}

int main(const char *exename, const char *fname)
{
    char dummy[sizeof(MAIN_CSM)];
    char *path=strrchr(exename, '\\');
    path++;
    memcpy(main_folder, exename, path-exename);

    InitLangpack();
    UpdateCSMname();
    LockSched();
    CreateCSM(&MAINCSM.maincsm,dummy,0);
    UnlockSched();
}
