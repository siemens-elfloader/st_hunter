#ifndef _PBAR_H_
#define _PBAR_H_

#include "../inc/swilib.h"

#define MAX_BAR 7

class Rectangle //класс создан для последующих градиентов
{
public:
    void Draw(const int x, const int y, const int x2, const int y2,
              const char *col_bg, const char *col_border);
};

class PBar : public Rectangle
{
private:
    int enable;
    const RECT *pos;
    int x_text, y_text;
    int measure;
    int cfg_font;
    float current, total;
    const char *text;
    const char *col_fill1;
    const char *col_fill2;
    const char *col_fill3;

    int current_id; //for SetCurrent()
    float GetMeasure(void);
    unsigned int GetFont(void);
    //static int count;
public:
    PBar(const int id);
    //int GetCount(){return count;}
    void InitVar(const int id);
    bool GetState(void);
    void SetCurrent(void);
    void Draw(WSHDR *ws);
};

#endif
