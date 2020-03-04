#ifndef _RECT_PATCHER_H_
  #define _RECT_PATCHER_H_


void patch_rect(RECT*rc,int x,int y, int x2, int y2);
void patch_header(const HEADER_DESC* head);
void patch_input(const INPUTDIA_DESC* inp);
void patch_option_header(const HEADER_DESC* head);

#endif
