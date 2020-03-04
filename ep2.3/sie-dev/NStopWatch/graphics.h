#ifndef _GRAPHICS_H_
  #define _GRAPHICS_H_

void Graphics_Init(int *load_status);
void Graphics_Free(void);

enum 
{
	IMG_BG,
	IMG_START_PRESSED,
	IMG_START_UNPRESSED,
	IMG_RESET_PRESSED,
	IMG_RESET_UNPRESSED,
	IMG_SPLIT_PRESSED,
	IMG_SPLIT_UNPRESSED,
	IMG_STOP_PRESSED,
	IMG_STOP_UNPRESSED,
	IMG_TOTAL
};

extern IMGHDR *IMG[IMG_TOTAL];

#endif
