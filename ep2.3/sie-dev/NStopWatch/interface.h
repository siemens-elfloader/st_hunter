#ifndef _INTERFACE_H_
  #define _INTERFACE_H_

extern WSHDR *ws_cur_time;

void DrawIMG(IMGHDR *img, int x, int y, int x2, int y2);
void DrawSplitTimes(const int type_redraw,const int cur_list);
void DrawButtons(int cur);
void DrawLoadingScreen(void);
void DrawTimeOnIdle(void);
void DrawPicOnIdle(void);
void DrawCurTime(const int mode);

#endif
