#include <swilib.h>
#include <conf_loader.h>


typedef struct
{
    GUI gui;
    IMGHDR *screen;
    WSHDR *lgp;
    unsigned int lgp_width;
    unsigned int strn;
    unsigned int type_msg;
} MAIN_GUI;

int gui_id;
static GBSTMR tmr;

extern unsigned int CFG_X_BG, CFG_Y_BG, CFG_ANGLES_BG, CFG_ALIGN_BG, CFG_CLOSE_TIME;
extern const char CFG_COL_FRAME[4], CFG_COL_BORDER_FRAME[4], CFG_COL_FNT[4];
extern const char CFG_PATH_ERROR[128], CFG_PATH_INFO[128];
extern const RECT CFG_POS_BG;


void Scrot(IMGHDR *img)
{
    int w=ScreenW(), h=ScreenH();
    unsigned int bitsize=w*h*2;
    img->bitmap=malloc(bitsize);
    memcpy(img->bitmap, RamScreenBuffer(), bitsize);
    img->w=w;
    img->h=h;
    img->bpnum=0x08;
}

void DrawIMG(IMGHDR *img, int x, int y, int x2, int y2)
{
    RECT rc;
    DRWOBJ drwobj;
    if(!x2 && !y2)
    {
        StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
        SetPropTo_Obj5(&drwobj, &rc, 0, img);
    }
    else
    {
        StoreXYXYtoRECT(&rc, x, y, x2, y2);
        SetProp2ImageOrCanvas(&drwobj, &rc, 0, img, x, y);
    }
    SetColor(&drwobj, NULL, NULL);
    DrawObject(&drwobj);
}

static void OnRedraw(MAIN_GUI *data)
{
    DrawIMG(data->screen, 0, YDISP, ScreenW(), ScreenH());


    unsigned int x=CFG_POS_BG.x, x2=CFG_POS_BG.x2, y=CFG_POS_BG.y, y2;
    const char *path_icon=(data->type_msg) ? CFG_PATH_INFO : CFG_PATH_ERROR;
    const int indent=(path_icon) ? 6 : 4;
    const unsigned int pic_height=GetImgHeight((int)path_icon);
    const unsigned int pic_width=GetImgWidth((int)path_icon);
    const int total_width=pic_width+indent+data->lgp_width;
    const unsigned int lgp_height=data->strn*GetFontYSIZE(FONT_SMALL);

    if(CFG_POS_BG.x2-CFG_POS_BG.x<total_width)  //если ширина в конфиге, меньше ширины окна
    {
        if(CFG_ALIGN_BG)
        {
            x=CFG_POS_BG.x;
            x2=CFG_POS_BG.x+total_width;
        }
        else
        {
            x=CFG_POS_BG.x2-total_width;
            x2=CFG_POS_BG.x2;
        }

    }
    if(lgp_height<pic_height)
        y2=CFG_POS_BG.y+4+pic_height;
    else
        y2=CFG_POS_BG.y+4+lgp_height;

    DrawRoundedFrame(x, CFG_POS_BG.y, x2, y2, CFG_ANGLES_BG, CFG_ANGLES_BG, 0,
                     CFG_COL_BORDER_FRAME, CFG_COL_FRAME);
    x+=2;
    if(pic_height)
    {
        DrawImg(x, CFG_POS_BG.y+((y2-CFG_POS_BG.y)/2-pic_height/2), (int)path_icon);
        x+=2;
    }
    y+=2;
    y2-=2;
    x2-=2;
    if(data->strn<=1)
    {
        y+=(y2-y)/2-GetFontYSIZE(FONT_SMALL)/2;
        y2=y+GetFontYSIZE(FONT_SMALL);
    }
    DrawString(data->lgp, x+pic_width,
               y, x2, y2,FONT_SMALL,TEXT_ALIGNLEFT, CFG_COL_FNT, GetPaletteAdrByColorIndex(23));
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
    Scrot(data->screen=malloc(sizeof(IMGHDR)+1));
    data->gui.state=1;
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
    data->gui.state=0;
    mfree(data->screen->bitmap);
    mfree(data->screen);
    FreeWS(data->lgp);
}

static void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
    data->gui.state=2;
    DisableIDLETMR();
}

static void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
    if (data->gui.state!=2) return;
    data->gui.state=1;
}

static int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
    if ((msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS))
        return (1);
    return(0);
}

int method8(void)
{
    return 0;
}
int method9(void)
{
    return 0;
}

const void * const gui_methods[11]=
{
    (void *)OnRedraw,
    (void *)onCreate,
    (void *)onClose,
    (void *)onFocus,
    (void *)onUnfocus,
    (void *)OnKey,
    0,
    (void *)kill_data,
    (void *)method8,
    (void *)method9,
    0
};

void CloseGUI(void)
{
    if (gui_id)
        GeneralFunc_flag1(gui_id, 0);
    GBS_DelTimer(&tmr);
    REDRAW();
}

void CreateMyGUI(char *lgp_ptr, unsigned int type_msg)
{
    if(lgp_ptr)
    {
        static RECT Canvas= {0,0,0,0};
        MAIN_GUI *main_gui = malloc(sizeof(MAIN_GUI));
        zeromem(main_gui,sizeof(MAIN_GUI));
        Canvas.x2=ScreenW()-1;
        Canvas.y2=ScreenH()-1;
        main_gui->gui.canvas=(RECT *)(&Canvas);
        main_gui->gui.methods=(void *)gui_methods;
        main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();

        /*костыли, они и в африке костыли*/

        int mlen=0;     //длина текущей строки
        int len=0;      //длина максимальной строки
        int n=0;        //количество строк
        int i=0;        //количество символов
        char *ptr_max_width=0;       //указывает на начало самой длинной строки

        while(lgp_ptr[i]!='\0')
        {
            if(lgp_ptr[i]!='\n')
                mlen++;
            else
            {
                if(mlen>len)
                {
                    len=mlen;
                    ptr_max_width=lgp_ptr+i-len;
                }
                mlen=0;
                n++;
            }
            i++;
        }
        //конец массива
        if(mlen>len)
        {
            len=mlen;
            ptr_max_width=lgp_ptr+i-len;
        }
        n++;

        //копируем данные
        main_gui->lgp=AllocWS(i+1);
        wsprintf(main_gui->lgp, "%t", lgp_ptr);
        main_gui->type_msg=type_msg;

        //считаем нужные нам величины(высоту и ширину)
        WSHDR *tmp_ws=AllocWS(len+1);
        ptr_max_width[len]='\0';
        wsprintf(tmp_ws, "%t", ptr_max_width);
        main_gui->lgp_width=Get_WS_width(tmp_ws, FONT_SMALL);
        main_gui->strn=n;
        FreeWS(tmp_ws);

        //нахуй
        mfree(lgp_ptr);


        /*создание gui*/
        gui_id=CreateGUI(main_gui);
        GBS_StartTimerProc(&tmr, 1300/6/10*CFG_CLOSE_TIME, (void*)&CloseGUI);
    }
    else
        MsgBoxError(1, (int)"Error create GUI!");
}
