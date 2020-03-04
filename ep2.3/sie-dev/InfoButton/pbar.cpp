#include "../inc/swilib.h"
#include "pbar.h"

extern const char CFG_COL_PR_BG[4], CFG_COL_PR_BORDER[4], CFG_COL_PR_FILL1[4], CFG_COL_PR_FILL2[4],
                CFG_COL_PR_FILL3[4], CFG_COL_FNT[4];

extern const RECT CFG_RAM_POS, CFG_FLEX0_POS, CFG_FLEX1_POS, CFG_FLEX2_POS, CFG_FLEX4_POS,
                  CFG_CAP_POS, CFG_CPU_POS;

extern const int CFG_RAM_ENA, CFG_RAM_X_TEXT, CFG_RAM_Y_TEXT, CFG_RAM_MEASURE, CFG_RAM_TOTAL,
                 CFG_RAM_FNT, CFG_FLEX0_X_TEXT, CFG_FLEX0_Y_TEXT, CFG_FLEX0_ENA, CFG_FLEX0_FNT,
                 CFG_FLEX0_MEASURE, CFG_FLEX1_X_TEXT, CFG_FLEX1_Y_TEXT, CFG_FLEX1_ENA, CFG_FLEX1_FNT,
                 CFG_FLEX1_MEASURE, CFG_FLEX2_X_TEXT, CFG_FLEX2_Y_TEXT, CFG_FLEX2_ENA, CFG_FLEX2_FNT,
                 CFG_FLEX2_MEASURE, CFG_FLEX4_X_TEXT, CFG_FLEX4_Y_TEXT, CFG_FLEX4_ENA, CFG_FLEX4_FNT,
                 CFG_FLEX4_MEASURE, CFG_CAP_ENA, CFG_CAP_X_TEXT, CFG_CAP_Y_TEXT, CFG_CAP_FNT,
                 CFG_CPU_ENA, CFG_CPU_X_TEXT, CFG_CPU_Y_TEXT, CFG_CPU_FNT;
extern const char CFG_RAM_TEXT[128], CFG_FLEX0_TEXT[128], CFG_FLEX1_TEXT[128], CFG_FLEX2_TEXT[128],
                  CFG_FLEX4_TEXT[128], CFG_CPU_COL_FILL1[4], CFG_CPU_COL_FILL2[4],
                  CFG_CPU_COL_FILL3[4], CFG_CPU_TEXT[128], CFG_CAP_TEXT[128];


void Rectangle :: Draw(const int x, const int y, const int x2, const int y2,
                       const char *col_bg, const char *col_border)
{
    DrawRectangle(x, y, x2, y2, 0, col_border, col_bg);
}

PBar :: PBar(const int id)
{
    InitVar(id);
    //count++;
}

void PBar :: InitVar(const int id)
{
    unsigned int err;
    current_id=id;
    col_fill1=CFG_COL_PR_FILL1;
    col_fill2=CFG_COL_PR_FILL2;
    col_fill3=CFG_COL_PR_FILL3;
    switch(id)
    {
        case 0: //ram
            enable=CFG_RAM_ENA;
            pos=&CFG_RAM_POS;
            x_text=CFG_RAM_X_TEXT;
            y_text=CFG_RAM_Y_TEXT;
            measure=CFG_RAM_MEASURE;
            cfg_font=CFG_RAM_FNT;
            total=CFG_RAM_TOTAL*1024;
            text=CFG_RAM_TEXT;
        break;
        case 1: //flexspace0
            enable=CFG_FLEX0_ENA;
            pos=&CFG_FLEX0_POS;
            x_text=CFG_FLEX0_X_TEXT;
            y_text=CFG_FLEX0_Y_TEXT;
            measure=CFG_FLEX0_MEASURE;
            cfg_font=CFG_FLEX0_FNT;
            total=GetTotalFlexSpace(0, &err);
            text=CFG_FLEX0_TEXT;
        break;
        case 2:
            enable=CFG_FLEX1_ENA;
            pos=&CFG_FLEX1_POS;
            x_text=CFG_FLEX1_X_TEXT;
            y_text=CFG_FLEX1_Y_TEXT;
            measure=CFG_FLEX1_MEASURE;
            cfg_font=CFG_FLEX1_FNT;
            total=GetTotalFlexSpace(1, &err);
            text=CFG_FLEX1_TEXT;
        break;
        case 3:
            enable=CFG_FLEX2_ENA;
            pos=&CFG_FLEX2_POS;
            x_text=CFG_FLEX2_X_TEXT;
            y_text=CFG_FLEX2_Y_TEXT;
            measure=CFG_FLEX2_MEASURE;
            cfg_font=CFG_FLEX2_FNT;
            total=GetTotalFlexSpace(2, &err);
            text=CFG_FLEX2_TEXT;
        break;
        case 4:
            enable=CFG_FLEX4_ENA;
            pos=&CFG_FLEX4_POS;
            x_text=CFG_FLEX4_X_TEXT;
            y_text=CFG_FLEX4_Y_TEXT;
            measure=CFG_FLEX4_MEASURE;
            cfg_font=CFG_FLEX4_FNT;
            total=GetTotalFlexSpace(4, &err);
            text=CFG_FLEX4_TEXT;
        break;
        case 5:
            enable=CFG_CAP_ENA;
            pos=&CFG_CAP_POS;
            x_text=CFG_CAP_X_TEXT;
            y_text=CFG_CAP_Y_TEXT;
            measure=0;
            cfg_font=CFG_CAP_FNT;
            total=100;
            text=CFG_CAP_TEXT;
        break;
        case 6:
            enable=CFG_CPU_ENA;
            pos=&CFG_CPU_POS;
            x_text=CFG_CPU_X_TEXT;
            y_text=CFG_CPU_Y_TEXT;
            measure=0;
            cfg_font=CFG_CPU_FNT;
            total=100;
            text=CFG_CPU_TEXT;
            col_fill1=CFG_CPU_COL_FILL1;
            col_fill2=CFG_CPU_COL_FILL2;
            col_fill3=CFG_CPU_COL_FILL3;
        break;
    }
}

float PBar :: GetMeasure(void)
{
    switch(measure)
    {
        case 0: return current;
        case 1: return current/1024;
        case 2: return current/1024/1024;
        default: return current;
    }
}

unsigned int PBar :: GetFont(void)
{
    switch(cfg_font)
    {
        case 0: return FONT_SMALL_ITALIC;
        case 1: return FONT_SMALL_ITALIC_BOLD;
        case 2: return FONT_SMALL;
        case 3: return FONT_SMALL_BOLD;
        case 4: return FONT_MEDIUM;
        case 5: return FONT_MEDIUM_BOLD;
        case 6: return FONT_LARGE;
        case 7: return FONT_LARGE_BOLD;
        default: return FONT_SMALL;
    }
}

void PBar :: Draw(WSHDR *ws)
{
    //прогрессбар
    unsigned int y, x2;
    const int width=pos->x2-pos->x;
    const int height=pos->y2-pos->y;
    char *col=(char*)malloc(4);
    if(width>=height)   //горизонтально
    {
        y=pos->y;
        x2=pos->x+(current/total)*width;
    }
    else    //вертикально
    {
        y=pos->y2-(current/total)*height;
        x2=pos->x2;
    }
    if(current<=total*0.3)
        memcpy(col, col_fill1, 4);
    else if(current<=total*0.7)
        memcpy(col, col_fill2, 4);
    else
        memcpy(col, col_fill3, 4);
    Rectangle::Draw(pos->x, pos->y, pos->x2, pos->y2, CFG_COL_PR_BG, CFG_COL_PR_BORDER);
    Rectangle::Draw(pos->x, y, x2, pos->y2, col, CFG_COL_PR_BORDER);
    delete col;

    //текст
    unsigned int font=GetFont();
    char *str=(char*)malloc(128);
    sprintf(str, text, GetMeasure());
    str_2ws(ws, str, strlen(str));
    x2=x_text+Get_WS_width(ws, font)+1;
    DrawString(ws, x_text, y_text, x2, y_text+GetFontYSIZE(font), font,
               TEXT_ALIGNLEFT, CFG_COL_FNT, GetPaletteAdrByColorIndex(23));
    delete str;
}

bool PBar :: GetState(void)
{
    return (enable) ? 1 : 0;
}

void PBar :: SetCurrent(void)
{
    unsigned int err;
    switch(current_id)
    {
        case 0: current=GetFreeRamAvail(); break;
        case 1: current=GetFreeFlexSpace(0, &err); break;
        case 2: current=GetFreeFlexSpace(1, &err); break;
        case 3: current=GetFreeFlexSpace(2, &err); break;
        case 4: current=GetFreeFlexSpace(4, &err); break;
        case 5: current=*RamCap(); break;
        case 6: current=GetCPULoad(); break;
    }
}
