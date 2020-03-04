#include <swilib.h>
#include "rect_patcher.h"
#include "menu.h"
#include "bmplib.h"
#include "lang.h"

extern unsigned int pic_id;
const char *d="%d";
const char *t="%t";

int str2int(const char *str)
{
    int num=0;
    for(int i=0; str[i]!=0; i++)
        num=num*10+(str[i]-'0');
    return num;
}

int ed1_onkey(GUI *data, GUI_MSG *msg)
{
    if (msg->keys==0xFFF) //select
    {
        EDITCONTROL ec;
        char *str=malloc(5);
        ExtractEditControl(data, 1,&ec);
        ws_2str(ec.pWS, str, wstrlen(ec.pWS));
        int id=str2int(str);
        if(GetPITaddr(id)) pic_id=id;
        mfree(str);
        return 0xFF;
    }
    return 0;
}

void ed1_ghook(GUI *data, int cmd)
{
    SOFTKEY_DESC sk= {0x0FFF, 0x0000, (int)lgp[lgp_view]};
    if(cmd==TI_CMD_FOCUS)
        DisableIDLETMR();
    if(cmd==TI_CMD_CREATE)
    {
        EDIT_SetFocus(data,2);
        EDIT_SetCursorPos(data,2);
    }
    if(cmd==TI_CMD_REDRAW)
        SetSoftKey(data,&sk,SET_SOFT_KEY_N);
}

void ed1_locret(void) {}

static SOFTKEY_DESC input_number_sk[]=
{
    {0x0018,0x0000,(int)"Options"},
    {0x0001,0x0000,(int)"Back"},
    {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

static SOFTKEYSTAB input_number_skt=
{
    input_number_sk,0
};

HEADER_DESC ed1_hdr= {0,0,0,0,NULL,(int)"Pic ID:",LGP_NULL};

INPUTDIA_DESC ed1_desc=
{
    1,
    ed1_onkey,
    ed1_ghook,
    (void *)ed1_locret,
    0,
    &input_number_skt,
    {0,0,0,0},
    FONT_MEDIUM,
    100,
    101,
    0,
    0,
    0x40000000
};

int CreateEdInputNumber()
{
    void *ma=malloc_adr();
    void *eq;

    EDITCONTROL ec;
    PrepareEditControl(&ec);
    eq=AllocEQueue(ma,mfree_adr());
    WSHDR *ews=AllocWS(128);

    ed1_hdr.lgp_id=(int)lgp[lgp_pic_id];
    wsprintf(ews, "%d", pic_id);
    ConstructEditControl(&ec, ECT_NORMAL_NUM, ECF_DISABLE_POINT,  ews, 4);
    AddEditControlToEditQend(eq,&ec,ma);

    patch_header(&ed1_hdr);
    patch_input(&ed1_desc);

    FreeWS(ews);
    return CreateInputTextDialog(&ed1_desc,&ed1_hdr,eq,1,0);
}
//=================================================================================
void SaveProc(int *id, int *id2)
{
    unsigned int total=0;
    while(*id<=*id2)
    {
        IMGHDR *img=GetPITaddr(*id);
        char *bitmap=NULL;
        char path[128];
        sprintf(path, "0:\\Pictures\\%d.bmp", *id);
        switch(img->bpnum)
        {
        case 0x85: //rle 8bit+pallete
            bitmap=SieRastrDecoder(img->bitmap, img->w*img->h, 8);
            if(SaveBMP(path, bitmap, img->w, img->h, 8, BI_RGB)!=-1) total++;
        break;
        case 0x88: //rle 16bit+pallete
            bitmap=SieRastrDecoder(img->bitmap, img->w*img->h, 16);
            if(SaveBMP(path, bitmap, img->w, img->h, 16, BI_BITFIELDS)!=-1) total++;
        break;
        case 0x8A: //32bit rle
            bitmap=SieRastrDecoder(img->bitmap, img->w*img->h, 32);
            if(SaveBMP(path, bitmap, img->w, img->h, 32, BI_RGB)!=-1)total++;
        break;
        case 0xA:
            if(SaveBMP(path, img->bitmap, img->w, img->h, 32, BI_RGB)!=-1)total++;
        break;
    /*default:
        str=malloc(32);
        sprintf(str, "bpnum: 0x%X", img->bpnum);
        ShowMSG(1, (int)str);
        mfree(str);*/
        }
        if(bitmap)  mfree(bitmap);
        *id+=1;
    }
    char *str=malloc(64);
    sprintf(str, "%d icons saved", total);
    ShowMSG(1, (int)str);
    mfree(str);
}


int ed2_onkey(GUI *data, GUI_MSG *msg)
{
    if (msg->keys==0xFFF) //select
    {
        static int id, id2;
        EDITCONTROL ec;
        char *str=malloc(5);
        ExtractEditControl(data, 2,&ec);
        ws_2str(ec.pWS, str, wstrlen(ec.pWS));
        id=str2int(str);
        ExtractEditControl(data, 4,&ec);
        ws_2str(ec.pWS, str, wstrlen(ec.pWS));
        id2=str2int(str);
        mfree(str);
        //char *str2=malloc(32);
        //sprintf(str, "%d\n%d", id, id2);
        //ShowMSG(1, (int)str);
        //mfree(str);


        if(id<=id2) SUBPROC((void*)SaveProc, &id, &id2);
        else    MsgBoxError(1, (int)"Error");

        return 0xFF;
    }
    return 0;
}

void ed2_ghook(GUI *data, int cmd)
{
    SOFTKEY_DESC sk= {0x0FFF, 0x0000, (int)lgp[lgp_save]};
    if(cmd==TI_CMD_FOCUS)
        DisableIDLETMR();
    if(cmd==TI_CMD_CREATE)
    {
        EDIT_SetFocus(data,2);
        EDIT_SetCursorPos(data,2);
    }
    if(cmd==TI_CMD_REDRAW)
        SetSoftKey(data,&sk,SET_SOFT_KEY_N);
}

void ed2_locret(void) {}

static SOFTKEY_DESC save_bmp_sk[]=
{
    {0x0018,0x0000,(int)"Options"},
    {0x0001,0x0000,(int)"Back"},
    {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

static SOFTKEYSTAB save_bmp_skt=
{
    save_bmp_sk,0
};

HEADER_DESC ed2_hdr= {0,0,0,0,NULL,(int)"Save...",LGP_NULL};

INPUTDIA_DESC ed2_desc=
{
    1,
    ed2_onkey,
    ed2_ghook,
    (void *)ed2_locret,
    0,
    &save_bmp_skt,
    {0,0,0,0},
    FONT_MEDIUM,
    100,
    101,
    0,
    0,
    0x40000000
};

int CreateEdSave()
{
    void *ma=malloc_adr();
    void *eq;

    EDITCONTROL ec;
    PrepareEditControl(&ec);
    eq=AllocEQueue(ma,mfree_adr());
    WSHDR *ews=AllocWS(128);

    ed2_hdr.lgp_id=(int)lgp[lgp_save_bmp];

    wsprintf(ews, "%t", lgp[lgp_first]);
    ConstructEditControl(&ec, ECT_HEADER, ECF_APPEND_EOL,  ews, wstrlen(ews));
    AddEditControlToEditQend(eq,&ec,ma);

    wsprintf(ews, d, pic_id);
    ConstructEditControl(&ec, ECT_NORMAL_NUM, ECF_APPEND_EOL,  ews, 4);
    AddEditControlToEditQend(eq,&ec,ma);

    wsprintf(ews, "%t", lgp[lgp_last]);
    ConstructEditControl(&ec, ECT_HEADER, ECF_APPEND_EOL,  ews, wstrlen(ews));
    AddEditControlToEditQend(eq,&ec,ma);

    wsprintf(ews, d, pic_id);
    ConstructEditControl(&ec, ECT_NORMAL_NUM, ECF_DISABLE_POINT,  ews, 4);
    AddEditControlToEditQend(eq,&ec,ma);

    patch_header(&ed2_hdr);
    patch_input(&ed2_desc);

    FreeWS(ews);
    return CreateInputTextDialog(&ed2_desc,&ed2_hdr,eq,1,0);
}
//=================================================================================

#define OPTIONS_ITEMS_N 3

int icon_array[1];

MENUITEM_DESC options_items[OPTIONS_ITEMS_N]=
{
    {icon_array,(int)"Enter number",          LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
    {icon_array,(int)"Save...",           LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
    {icon_array,(int)"About",                 LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2}
};

void options_create_number(GUI *gui)
{
    CreateEdInputNumber();
    GeneralFuncF1(2);
}

void options_save(GUI *gui)
{
    CreateEdSave();
    GeneralFuncF1(2);
}

void options_about(GUI *gui)
{
    char str[128];
    sprintf(str, LGP_COPYRIGHT, "0.3.1", __DATE__);
    ShowMSG(0, (int)str);
    GeneralFuncF1(2);
}

const MENUPROCS_DESC options_procs[OPTIONS_ITEMS_N]=
{
    options_create_number,
    options_save,
    options_about,
};

int options_softkeys[]= {0,1,2};

SOFTKEY_DESC options_sk[]=
{
    {0x0018,0x0000,(int)"Select"},
    {0x0001,0x0000,(int)"Back"},
    {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB options_skt=
{
    options_sk,0
};

MENU_DESC options_struct=
{
    8, NULL, NULL, NULL,
    options_softkeys,
    &options_skt,
    0x00000001,
    NULL,
    options_items,   //Items
    options_procs,   //Procs
    OPTIONS_ITEMS_N //n
};

HEADER_DESC options_header= {0,0,0,0,NULL,(int)"Options",LGP_NULL};

int CreateOptionsMenu(void)
{
    icon_array[0]=
#ifdef ELKA
        1336;
#else
        1380;
#endif
    options_sk[0].lgp_id=(int)lgp[lgp_select];
    options_sk[1].lgp_id=(int)lgp[lgp_back];
    options_header.lgp_id=(int)lgp[lgp_options];
    options_items[0].lgp_id_small=(int)lgp[lgp_enter_number];
    options_items[1].lgp_id_small=(int)lgp[lgp_save_bmp];
    options_items[2].lgp_id_small=(int)lgp[lgp_about];
    patch_header_small((HEADER_DESC*)(&options_header));
    return CreateMenu(1, 0, &options_struct, &options_header, 0, OPTIONS_ITEMS_N, 0, 0);
}

