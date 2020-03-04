#include "..\inc\swilib.h"
#include "..\inc\cfg_items.h"
#include "conf_loader.h"
#include "code.h"
#include "bookmark.h"

/* 待完成
  -页码与边界控制
  -字符集控制
  -书签载入/生成
  -定位控制
  -搜索
  -编辑
*/
/*
===============================
         窗口调整
===============================
*/
#pragma inline
void patch_header(HEADER_DESC* head) {
  head->rc.x = 0;
  head->rc.y = YDISP;
  head->rc.x2 = ScreenW() - 1;
  head->rc.y2 = HeaderH() + YDISP;
}

#pragma inline
void patch_input(INPUTDIA_DESC* inp) {
  inp->rc.x = 0;
  inp->rc.y = HeaderH() + 1 + YDISP;
  inp->rc.x2 = ScreenW() - 1;
  inp->rc.y2 = ScreenH() - SoftkeyH() - 1;
}

#pragma inline
void patch_rect(RECT* rc, int x, int y, int x2, int y2) {
  rc->x = x;
  rc->y = y;
  rc->x2 = x2;
  rc->y2 = y2;
}

/*
===============================
           end
===============================
*/


/*
===============================
          参数定义
===============================
*/
int Quit_Required = 0;
/*---------文件----------*/
#define BUF_LEN 32768			//缓存长度

typedef struct DataIndex {
  struct DataIndex *next;
  struct DataIndex *prev;
  int offset;
  int uc16len, datalen;
} DataIndex;

DataIndex *fileindex;			//文件索引
DataIndex *curblock;		  //当前块
char* buff;	        			//缓存
int codetype = 0;
char procfile[128];
int buffloaded = 0;
int fd = 0;	        			//文件号
unsigned int ul = 0;	
int foffset = 0;					//文件偏移

int rlen = 0, llen = 0;

int bmkfd = 0;          	//书签文件号
int* bookmark;          	//书签
int bmknum;              	//数量
/*---------end-----------*/

/*---------屏幕----------*/
int winx = 0, winy = 0, winw = 0, winh = 0;
int viewlen = 0;                    //一次性转换的字节数
int bytelen = 0;										//显示缓存字节数
int viewpos = 0;                    //显示起始缓存偏移
int viewrow = 0;                		//
short showinfo = 0;
WSHDR *sbw;													//屏幕显存

extern const int charset;				//编码
extern const int space;					//行间距
extern const int font;					//字体
extern const int status;				//显示状态条
extern const int acbm;					//自动生成书签
extern const int statfont;			//状态条字体
extern const int fileicon;			//文件图标
extern const int foldicon;			//文件夹图标
extern const char fontcolor[4];	//字体颜色
extern const char bgcolor[4];		//背景颜色
extern const char statbgcolor[4]; 	//状态栏背景颜色
extern const char statfontcolor[4]; //状态栏字体颜色
extern const char LAST[];				//上次打开的文件
/*---------end-----------*/
/*
===============================
           end
===============================
*/


/*
===============================
           文件
===============================
*/
int getStartOffset(void) {
  int offset = 0;
  if (charset) codetype = charset;
  else {
		switch (codetype) {
  	case 2: case 3: offset += 2; break;
  	case 4: offset += 3; break;
  	}
  }
  return offset;
}

int createIndex(void) {
	
	int readlen = BUF_LEN - (BUF_LEN >> 2);

  char *tb = NULL;
  DataIndex *p;
  foffset = getStartOffset();
  
  //链表头，保存整个文件的信息
  fileindex = malloc(sizeof (DataIndex));
  fileindex->prev = NULL;
  fileindex->next = NULL;
  fileindex->offset = foffset;
  fileindex->uc16len = 0;
  fileindex->datalen = 0;
  p = fileindex;
  
  if (lseek(fd, foffset, S_SET, &ul, &ul) != foffset) return 0;
  
  while ((rlen = llen = fread(fd, buff, readlen, &ul)) > 0) {
  	fileindex->datalen += llen;
  	switch (codetype) {
  		case 1: //ansi
  		tb = gb2unicode(buff, rlen, &rlen, &llen);
  		break;
		case 2:   //unicode le
			rlen = rlen >> 1 << 1;
			llen = rlen;
			tb = malloc(1);
    	break;
		case 3:   //unicode be
			tb = unicodeSwitch(buff, rlen, &rlen, &llen);
			break;
		case 4:   //utf8
			tb = utf82unicode(buff, rlen, &rlen, &llen);
			break;
		default:
			break;
  	}
  	if (!tb) return 0;
  	mfree(tb);
  	p->next = malloc(sizeof (DataIndex));
  	p->next->offset = foffset;
  	p->next->uc16len = rlen;
  	p->next->datalen = llen;
  	p->next->prev = p;
  	p->next->next = NULL;
  	fileindex->uc16len += rlen;
  	foffset += llen;
  	p = p->next;
  }
  
  if (!fileindex->next) { //0字节文件
  	p = malloc(sizeof (DataIndex));
  	p->uc16len = p->datalen = 0;
  	p->offset = 0;
  	p->prev = fileindex;
  	p->next= NULL;
  	fileindex->next = p;
	}
	fileindex->prev = p;
  return 1;
}

int WriteLog(char *text, unsigned int len) {			//调试
  if (!text) return 0;
  int f = fopen("0:\\test.log", A_ReadWrite + A_Create + A_BIN, P_READ + P_WRITE, &ul);
  if (f != -1) {
    fwrite(f, text, len, &ul);
    fclose(f, &ul);
    return 1;
  }
  return 0;
}


int openFile(char *filename) {
	if (fd > 0) fclose(fd, &ul);
	buffloaded = 0;
	if ((fd = fopen(filename, A_ReadOnly + A_BIN, P_READ, &ul)) != -1) {
    if (fread(fd, buff, 3, &ul) > 0) {
    	codetype = getCodeType(buff);
    }
    if (!createIndex()) return 0;
    foffset = fileindex->offset;
    curblock = fileindex->next;
    return 1;
  }
  return 0;
}

int loadBuffer(void) {
	
	if (curblock && lseek(fd, curblock->offset, S_SET, &ul, &ul) == curblock->offset) {
		char *ms = malloc (50);
		char *tb = NULL;
		if ((rlen = llen = fread(fd, buff, BUF_LEN, &ul)) > 0) { 
  		switch (codetype) {
  			case 1: //ansi
  			tb = gb2unicode(buff, rlen, &rlen, &llen);
  			if (rlen > BUF_LEN) {
  				mfree(buff);
  				buff = malloc(rlen);
  			}
  			memcpy(buff, tb, rlen);
  			break;
			case 2:   //unicode le
    		break;
			case 3:   //unicode be
				tb = unicodeSwitch(buff, rlen, &rlen, &llen);
				if (rlen > BUF_LEN) {
  				mfree(buff);
  				buff = malloc(rlen);
  			}
				memcpy(buff, tb, rlen);
  			break;
			case 4:   //utf8
				tb = utf82unicode(buff, rlen, &rlen, &llen);
				if (rlen > BUF_LEN) {
  				mfree(buff);
  				buff = malloc(rlen);
  			}
  			memcpy(buff, tb, rlen);
  			break;
			default:
				ShowMSG(1, (int) "Unkonwn code type!");
				break;
  		}
  		if (tb) mfree(tb);
  	}
  	
		buffloaded = 1;
  	foffset = curblock->offset;
  	return 1;	
  }
  else
		return 0;
}

int gotoPos(int offset) {
	DataIndex *p = curblock;
	curblock = fileindex->next;
	while (curblock && offset > curblock->offset + curblock->uc16len) {
		curblock = curblock->next;
	}
	if (curblock) {
		viewpos = (offset - curblock->offset) >> 1 << 1;
		if (viewpos < 0) {
			viewpos = 0;
			offset = curblock->offset;
		}
		foffset = offset;
		return loadBuffer();	
	}
	else {
		curblock = p;
		ShowMSG(1, (int) "no buff?");
		return 0;
	}
}

int InitSettings();
void KillIndex();
int loadFile(char *filename) {
	LockSched();
	InitSettings();
	viewpos = 0;
	KillIndex();
  if (openFile(filename)) {
		//载入书签
		if (bookmark) {
			mfree(bookmark);
			bookmark = NULL;
		}
    bookmark = readAddrInt(filename, &bmkfd, &ul, &bmknum, acbm);
    if (bookmark && bmknum > 0)
    	gotoPos(*bookmark);
    else
    	foffset = 0;
    if (!buffloaded) {
    	curblock = fileindex->next;
    	loadBuffer();
    }
    strcpy(procfile, filename);
 		UnlockSched();
 		return 1;
  }
  UnlockSched();
	return 0;
}
/*
===============================
           end
===============================
*/

/*
===============================
          初始化
===============================
*/
int div(int a, int b) {
  if (b == 0) return 0;
  int symbol = 1;
  if (a < 0) symbol *= -1;
  if (b < 0) symbol *= -1;
  int result = 0;
  while (result * b < a) result++;
  return result * symbol;
}

void adjustWindowsSize() {
	winx = 0;
  winy = YDISP;
  winw = ScreenW() - winx;
  winh = ScreenH() - winy;
}

int InitSettings() {
  //初始化各项设置
  adjustWindowsSize();
  InitConfig();
  viewrow = div(winh, GetFontYSIZE(font) + space);
  viewlen = div(winw, GetSymbolWidth(' ', font)) * viewrow;
  viewlen = viewlen >> 1 << 1;
  if (sbw) {
  	FreeWS(sbw);
  	sbw = NULL;
  }
  sbw = AllocWS(viewlen);
  if (sbw) return 1; else return 0;
}

//销毁索引
void KillIndex() {
	while (fileindex) {
		curblock = fileindex;
		fileindex = fileindex->next;
		mfree(curblock);
	}
	fileindex = NULL;
}

//销毁
extern void kill_data(void *p, void (*func_p)(void *));
void ElfKiller(void) {
	KillIndex();
	mfree(buff);
  mfree(bookmark);
  FreeWS(sbw);
  fclose(fd, &ul);
  fclose(bmkfd, &ul);
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN, (void (*)(void *))mfree_adr());
}
/*
===============================
            end
===============================
*/


/*
===============================
          屏幕控制
===============================
*/

void rowFluctuate(int param) {
	
	if (!buffloaded || param == 0) return;
	int i = 0, curwidth = 0, offset = viewlen;
	if (param < 0) {
		if (viewpos - viewlen < 0) {
			if (curblock->prev != fileindex) { //载入上一块
				curblock = curblock->prev;
				loadBuffer();
				viewpos = viewpos - viewlen + rlen;
				if (viewpos <= 0) {
					viewpos = 0;
					return;
				}
			}
			else if (viewpos < 0) {
				viewpos = 0;
				return;
			}
			else {
				offset = viewpos;
			}
		}
		str2ws_unicode(sbw, buff + viewpos - offset, offset);
    for (i = sbw->wsbody[0]; i > 0; i--) {
      if (sbw->wsbody[i] == 0x0a && i > 1 && sbw->wsbody[i - 1] == 0x0d) {
      	if (param + 1 == 0) {
      		if (i != sbw->wsbody[0]) {
      			param++;
      			i++;
      			break;
      		}
      		else {
      			param--;
      		}
      	}
				i--;
      	goto L_CALROW1;
      }
     	else if (sbw->wsbody[i] == 0x0d) {
     		goto L_CALROW1;
     	}
      else {
        curwidth += GetSymbolWidth(sbw->wsbody[i], font);
        if (curwidth > winw) {
        	i++;
        L_CALROW1:
          curwidth = 0;
          param++;
         	if (param == 0) break;
        }
      }
    }
    viewpos -= (sbw->wsbody[0] - i + 1) << 1;
  }
	else if (param > 0) {
		str2ws_unicode(sbw, buff + viewpos, viewlen > rlen ? rlen : viewlen);
    for (i = 1; i <= sbw->wsbody[0]; i++) {
      if (sbw->wsbody[i] == 0x0a || sbw->wsbody[i] == 0x0d) {
        if (i < sbw->wsbody[0] && sbw->wsbody[i + 1] == 0x0a) i++;
        goto L_CALROW2;
      }
      else {
        curwidth += GetSymbolWidth(sbw->wsbody[i], font);
        if (curwidth > winw) {
        	i--;
        L_CALROW2:
          curwidth = 0;
          param--;
          if (param == 0) break;
        }
      }
    }
    viewpos += i << 1;
    if (rlen < viewlen) {
    	viewpos -= i << 1;
    	return;
    }
    if (viewpos > curblock->uc16len) {
    	if (curblock->next) {
    		viewpos -= curblock->uc16len;
    		curblock = curblock->next;
    		loadBuffer();
    		if (viewpos > rlen) {
    			viewpos = 0;
    			return;
    		}
    	}
    	else {
				viewpos -= i << 1;
				return;
			}
    }
  }
  rowFluctuate(param);
}

/*
//卷动条
void DrawIndication(void) {
	
	int progress =  winh * ((viewpos / curblock->uc16len) * (curblock->offset / fileindex->datalen));
	progress = progress * , ;
	
	DrawRectangle(winw + winx - 3,
                winy,
                winw + winx - 1,
                winy + winh,
                1,     //border-type 0=outline 1=dotted 2=inline
                GetPaletteAdrByColorIndex(3),
                GetPaletteAdrByColorIndex(3));
  int indh = div(winh, div(fileindex->uc16len, viewlen));
  if (indh < 3) indh = 3;
  int indy = div(winh, progress);
  DrawRectangle(winw + winx - 3,
                winy + indy,
                //winy + 70,
                winw + winx - 1,
                winy + indy + indh,
                //winy + 100,
                0,     //border-type 0=outline 1=dotted 2=inline
                GetPaletteAdrByColorIndex(5),
                GetPaletteAdrByColorIndex(5));
}
*/
//状态栏
void DrawStateBar(void) {
    DrawRectangle(winx,
                winy + winh - GetFontYSIZE(FONT_SMALL_ITALIC_BOLD),
                winx + winw,
                winy + winh,
                0,
                //GetPaletteAdrByColorIndex(bgcolor),
                //GetPaletteAdrByColorIndex(bgcolor)
                statbgcolor,
                statbgcolor
                );
	WSHDR* ws = AllocWS(50);
	TDate d;
  TTime t;
  GetDateTime(&d, &t);
  int progress = div((viewpos + viewlen) * 1000, curblock->uc16len);
  if (progress <= 0) progress = 1;
  int factor = div((curblock->offset + curblock->datalen) * 1000, fileindex->datalen);
  if (factor <= 0) factor = 1;
	progress = progress * factor;
	progress /= 1000;
  if (progress > 1000) progress = 1000;
  wsprintf(ws, "%d%d%d.%d%c %d%d:%d%d", progress / 1000, progress / 100 % 10, progress / 10 % 10, progress % 10, '%',
  																			t.hour / 10, t.hour % 10, t.min / 10, t.min % 10);
	DrawString(ws,
							winx, winy + winh - GetFontYSIZE(FONT_SMALL_ITALIC_BOLD), winx + winw - 1, winy + winh - 1,
              statfont,
              TEXT_ALIGNRIGHT,
              statfontcolor,
              statbgcolor
              //GetPaletteAdrByColorIndex(fontcolor),
              //GetPaletteAdrByColorIndex(bgcolor)
             );
	FreeWS(ws);
}

//信息框
void DrawInfoDialog(void) {
	  char *s = malloc(100);
	  ShowMSG(1, (int)s);
	  mfree(s);
}

//主窗口
void DrawScreen(void) {     //显示位置bug
	
  DrawRectangle(winx,
                winy,
                winx + winw,
                winy + winh,
                0,                               //border-type 0=outline 1=dotted 2=inline
                bgcolor,
                bgcolor
                );
  if (!buffloaded) return;
  bytelen = rlen - viewpos < viewlen ? rlen - viewpos : viewlen;
  str2ws_unicode(sbw, buff + viewpos, bytelen);
  WSHDR *tws = AllocWS(sbw->wsbody[0]);      //实际输出字符串，自动换行和行间距
  int curwidth = 0, drawrow = 0;
  int i = 1, j = 1;
  for (i = 1; i <= sbw->wsbody[0]; i++) {
    if (sbw->wsbody[i] == 0x0a || sbw->wsbody[i] == 0x0d) {
      if (i < sbw->wsbody[0] && sbw->wsbody[i + 1] == 0x0a) i++;
      goto L_DRAWCHAR;
    }
    else {
      curwidth += GetSymbolWidth(sbw->wsbody[i], font);
      if (curwidth > winw) {
      	i--;
      L_DRAWCHAR:
        DrawString(tws,
                    1, 1 + winy + (space + GetFontYSIZE(font)) * drawrow++, (curwidth << 1), winy + winh - 1,
                    font,
                    TEXT_ALIGNLEFT,
                    fontcolor,
                    bgcolor
                  );
        curwidth = 0;
        j = 1;
      }
      else {
      	tws->wsbody[j] = sbw->wsbody[i];
      	tws->wsbody[0] = j++; 
      }
    }
  }
  if (curwidth > 0 && i > sbw->wsbody[0]) {
    i = sbw->wsbody[0];
    goto L_DRAWCHAR;
  }
  FreeWS(tws);
  
  //if (scroll) DrawIndication();
  if (status) DrawStateBar();
  //if (showinfo) DrawInfoDialog();
}
/*
===============================
           end
===============================
*/


/*
===============================
           菜单
===============================
*/
/*---------软键----------*/
int menusoftkeys[] = {0, 1, 2};
SOFTKEY_DESC menu_sk[] = {
  {0x0018, 0x0000, (int) "Select"},
  {0x0001, 0x0000, (int) "Back"},
  {0x003D, 0x0000, (int) LGP_DOIT_PIC}
};

SOFTKEYSTAB menu_skt = {
  menu_sk, 0
};


/*--------主菜单---------*/
//int create_menu_folder(void);
void open_select_file_gui(int);
int create_menu_goto(void);
int create_menu_bookmark(void);

void mainmenu_open(GUI *data) {
  GeneralFuncF1(1);
  //create_menu_folder();
  open_select_file_gui(1);
}

void mainmenu_quit(GUI *data) {
  GeneralFuncF1(1);
  Quit_Required = 1;
}

int add2bmk(void) {
  
	if (!buffloaded) return 0;
	if (bookmark && bmknum == 0) {
		bookmark[bmknum++] = curblock->offset + viewpos;
		bookmark[bmknum++] = curblock->offset + viewpos;
		writeAddrInt(procfile, &bmkfd, &ul, bookmark, bmknum, acbm);
	}
	
  if (bookmark && bmknum < BOOK_NUM) {
  	int bmkaddr = curblock->offset + viewpos;
  	int i;
  	for (i = 2; i < bmknum; i++) {
  		if (bmkaddr == bookmark[i]) break;
  	}
  	if (i == bmknum) {
    	bookmark[bmknum++] = bmkaddr;
    	writeAddrInt(procfile, &bmkfd, &ul, bookmark, bmknum, acbm);
    	return 1;
    }
   }
   return 0;
}

int deletebmk(int n) {
	if (!buffloaded || !bookmark || n > bmknum || bmknum < 3) return 0;
	bmknum--;
  int i;
	for (i = n; i < bmknum; i++)
		bookmark[i] = bookmark[i + 1];
	writeAddrInt(procfile, &bmkfd, &ul, bookmark, bmknum, acbm);
	return 1;
}

void mainmenu_add2bmk(GUI *data){
	GeneralFuncF1(1);
	add2bmk();
	create_menu_bookmark();
}

void mainmenu_search(GUI *data){

}

void mainmenu_goto(GUI *data){
  GeneralFuncF1(1);
  create_menu_goto();
}

int LaunchEditor(void);
void mainmenu_edit(GUI *data){
	GeneralFuncF1(1);
	LaunchEditor();
}

void mainmenu_sendsms(GUI *data) {

}

void mainmenu_set(GUI *data){
  WSHDR *ws = AllocWS(150);
  int fd1;
  if ((fd1 = fopen("4:\\ZBin\\etc\\SieTxtView.bcfg", A_ReadOnly + A_BIN, P_READ, &ul)) != -1)
    str_2ws(ws, "4:\\ZBin\\etc\\SieTxtView.bcfg", 128);
  else
    str_2ws(ws, "0:\\ZBin\\etc\\SieTxtView.bcfg", 128);
  fclose(fd1, &ul);
  ExecuteFile(ws, 0, 0);
  FreeWS(ws);
}

void mainmenu_help(GUI *data){
  ShowMSG(1, (int)"Siemens Text Viewer v0.3f\nby HanikLZ\n2007");
}

const MENUPROCS_DESC mainmenu_HNDLS[9] = {
  mainmenu_open,
  mainmenu_add2bmk,
  mainmenu_search,
  mainmenu_goto,
  mainmenu_edit,
  mainmenu_sendsms,
  mainmenu_set,
  mainmenu_help,
  mainmenu_quit
};

MENUITEM_DESC mainmenu_ITEMS[9] = {
  {NULL, (int)"Open...",  		LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL, (int)"bookmark...",  LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL, (int)"Search...",  	LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL, (int)"Go to...",  		LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL, (int)"Edit...",  		LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL, (int)"Send SMS...",  LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL, (int)"Set...",  			LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL, (int)"Help",  				LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL, (int)"Quit",     		LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2}
};

HEADER_DESC mainmenu_HDR = {0, 0, 0, 0, 0 //icon
                            ,(int)"Main Menu", LGP_NULL};

MENU_DESC mainmenu_STRUCT = {
  8, NULL, NULL, NULL,
  menusoftkeys,
  &menu_skt,
  0,
  NULL,
  mainmenu_ITEMS,
  mainmenu_HNDLS,
  9
};

int create_menu_main(void) {
  patch_header(&mainmenu_HDR);
  return CreateMenu(0, 0, &mainmenu_STRUCT, &mainmenu_HDR, 0, 9, 0, 0);
}
/*----------end--------------*/

/*--------GOTO---------*/
void gotomenu_start(GUI *data) {
	GeneralFuncF1(1);
	gotoPos(0);
}

void gotomenu_readed(GUI *data) {
  
	GeneralFuncF1(1);
	if (!bookmark || bmknum < 2 || !gotoPos(bookmark[1]))
		ShowMSG(1, (int) "Load bookmark failed!");
}

void gotomenu_end(GUI *data) {
	GeneralFuncF1(1);
	if (!buffloaded) return;
	gotoPos(fileindex->prev->offset + 
  				((fileindex->prev->uc16len - viewlen) > 0 ? (fileindex->prev->uc16len - viewlen) : 0));
}

void gotomenu_random(GUI *data) {
	
	GeneralFuncF1(1);
	if (!buffloaded) return;
	char *s = malloc(10);
//	sprintf(s, "%d", Rand());
  ShowMSG(1, (int) s);
  mfree(s);
}

void gotomenu_bookmark(GUI *data) {
	GeneralFuncF1(1);
  create_menu_bookmark();
}

const MENUPROCS_DESC gotomenu_HNDLS[5] = {
	gotomenu_start,
  gotomenu_readed,
  gotomenu_end,
  gotomenu_random,
  gotomenu_bookmark
};

MENUITEM_DESC gotomenu_ITEMS[5] = {
  {NULL, (int)"Start",  					LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{NULL, (int)"Readed",  					LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL, (int)"End",    					LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL, (int)"Random position",  LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
	{NULL, (int)"Bookmark...",  		LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2}
};

HEADER_DESC gotomenu_HDR = {0, 0, 0, 0, 0 //icon
                            ,(int)"Go to...", LGP_NULL};

MENU_DESC gotomenu_STRUCT = {
  8, NULL, NULL, NULL,
  menusoftkeys,
  &menu_skt,
  0,
  NULL,
  gotomenu_ITEMS,
  gotomenu_HNDLS,
  5
};

int create_menu_goto(void) {
  patch_header(&gotomenu_HDR);
  return CreateMenu(0, 0, &gotomenu_STRUCT, &gotomenu_HDR, 0, 5, 0, 0);
}
/*----------end--------------*/
/*
===============================
            end
===============================
*/


/*
===============================
         编辑框
===============================
*/

int saveText(void) {
	fclose(fd, &ul);
	fd = -1;
	char temp[128];
  strncpy(temp, procfile, strlen(procfile));
  strcat(temp, ".temp");
  
  unlink(temp, &ul);
  fmove(procfile, temp, &ul);
  int sfd = fopen(temp, A_ReadOnly, P_READ, &ul);
  int dfd = fopen(procfile, A_ReadWrite + A_Create + A_Truncate, P_READ + P_WRITE, &ul);
  char *tbuff = malloc(BUF_LEN);
  
  int maxlen = curblock->offset;
  int len = 0;
  while (maxlen > 0) {
  	if ((len = fread(sfd, tbuff, maxlen > BUF_LEN ? BUF_LEN : maxlen, &ul)) <= 0) break;
  	fwrite(dfd, tbuff, len, &ul);
  	maxlen -= BUF_LEN;
	}
	
	len = sbw->wsbody[0] << 1;
	char *s = malloc(len);
	ws2str_unicode(s, sbw, &len);
	
	maxlen = rlen - viewpos - bytelen;
	rlen = rlen - bytelen + len;
	if (rlen > BUF_LEN) {
		mfree(tbuff);
		tbuff = malloc(rlen);
	}
	
	memcpy(tbuff, buff, viewpos);
	memcpy(tbuff + viewpos, s, len);
	memcpy(tbuff + viewpos + len, buff + viewpos + bytelen, maxlen);
	mfree(s);
	
	maxlen = curblock->offset + llen;
	char *tb = NULL;
 	switch (codetype) {
	case 1: //ansi
  	tb = unicode2gb(tbuff, &rlen);
  	break;
	case 2:   //unicode le
		tb = malloc(rlen);
		memcpy(tbuff, tb, rlen);
   	break;
	case 3:   //unicode be
		tb = unicodeSwitch(tbuff, rlen, &rlen, &llen);
		break;
	case 4:   //utf8
		tb = unicode2utf8(tbuff, &rlen);
		break;
	default:
		tb = unicode2gb(tbuff, &rlen);
		break;
  }
  //WriteLog(tb, rlen);
  fwrite(dfd, tb, rlen, &ul);
	mfree(tb);
	if (lseek(sfd, maxlen, S_SET, &ul, &ul) == maxlen) {
		maxlen = fileindex->datalen - maxlen + 1;
  	while (maxlen > 0) {
  		if ((len = fread(sfd, tbuff, maxlen > BUF_LEN ? BUF_LEN : maxlen, &ul)) <= 0) break;
  		fwrite(dfd, tbuff, len, &ul);
  		maxlen -= BUF_LEN;
		}
	}
	
	int addr = 0;
	if (curblock) addr = curblock->offset + viewpos;
	
	/*
	sprintf(tbuff, "%d\0", addr);
  ShowMSG(1, (int) tbuff);
  */
  mfree(tbuff);
	fclose(dfd, &ul);
	fclose(sfd, &ul);
	unlink(temp, &ul);
	if (loadFile(procfile)) {
		gotoPos(addr);
	}
	else {
		rlen = llen = 0;
		ShowMSG(1, (int) "File failed!");
		return 0;
	}
	return 1;
}

int inp_onkey(GUI *gui, GUI_MSG *msg) {
	
	EDITCONTROL ec;
  
  if(msg->gbsmsg->submess == RED_BUTTON) {
    return 1;
  }
  
  if(msg->gbsmsg->submess == GREEN_BUTTON) {
  	
    
  }
  
  if (msg->keys == 0x18) {
  	
  	ExtractEditControl(gui, 1, &ec);
    if (sbw) FreeWS(sbw);
    sbw = AllocWS(viewlen);
    wstrcpy(sbw, ec.pWS);
/*    char *s = malloc(100);
    sprintf(s, "%d:%d", sbw->wsbody[0], ec.pWS->wsbody[0]);
   	ShowMSG(1, (int) s);
   	mfree(s);*/
    if(saveText()) {
      return 1;
    }
  	/*
    ExtractEditControl(gui, 1, &ec);    
    wstrcpy(editstr, ec.pWS);
    patch_header(&menuhdr);
    menu_gui_id = CreateMenu(0, 0, &tmenu, &menuhdr, 0, 4, editstr, 0);*/
    return -1; //do redraw
  }
  return 0; //Do standart keys
  //1: close
}


void inp_ghook(GUI *gui, int cmd) {
	
  static SOFTKEY_DESC sk = {0x0018, 0x0000, (int)"Save"};
  
  //WSHDR *editstr = EDIT_GetUserPointer(gui);
  
  if (cmd == 2) {
    //Called after onCreate
  }
  
  if (cmd == 3) {
  	//退出
    //FreeWS(tmostr);    
  }    
  if (cmd == 7) {
  	SetSoftKey(gui, &sk, SET_SOFT_KEY_N);    
  }
}

void inp_locret(void){}

INPUTDIA_DESC inp_desc = {
  1,
  inp_onkey,
  inp_ghook,
  (void *)inp_locret,
  0,
  &menu_skt,
  {0, 22, 131, 153},
  4,
  100,
  101,
  0,
  0,
  0x40000000
};


HEADER_DESC inp_hdr = {0, 0, 131, 21,/*icon*/NULL, (int)"Edit", LGP_NULL};

int LaunchEditor(void) {
	if (!buffloaded) return 0;
  void *ma = malloc_adr();
  void *eq;
  EDITCONTROL ec;
  PrepareEditControl(&ec);
  eq = AllocEQueue(ma, mfree_adr());
  ConstructEditControl(&ec, 4, 0x40, sbw, viewlen);
  AddEditControlToEditQend(eq, &ec, ma);
  patch_header(&inp_hdr);
  patch_input(&inp_desc);
  return CreateInputTextDialog(&inp_desc, &inp_hdr, eq, 1, sbw);
}

/*
===============================
           end
===============================
*/


/* need fix
===============================
         文件列表
===============================
*/
typedef struct {
  void *next;
  int is_folder;
  char *fullname;
  char *name;
} FLIST;

typedef struct {
  void *gui;
  int type;
} FVIEW;

volatile FLIST *fltop;
enum TYPES {IS_BACK, IS_FOLDER, IS_FILE}; 
const char back[] = "..";
char header[128];

void Free_FLIST(void) {
	
  LockSched();
  FLIST *fl = (FLIST *)fltop;
  fltop = 0;
  UnlockSched();
  while (fl) {
  	FLIST *fl_prev = fl;
    fl = fl->next;
    mfree(fl_prev);
  }
}

int strcmp_nocase(const char *s, const char *d) {
	
  int cs;
  int ds;
  do {
    cs = *s++;
    if (cs & 0x40) cs &= 0xDF;
    ds = *d++;
    if (ds & 0x40) ds &= 0xDF;
    cs -= ds;
    if (cs) break;
  }
  while(ds);
  return(cs);
}

FLIST *AddToFList(const char* full_name, const char *name, int is_folder) {
	
  int l_fname;
  FLIST *fl;
  FLIST *fn = malloc(sizeof (FLIST) + (l_fname = strlen(full_name)) + strlen(name) + 2);
  fn->fullname = (char *)fn + sizeof (FLIST);
  fn->name = (char *)fn + sizeof (FLIST) + l_fname + 1;
  strcpy(fn->fullname, full_name);
  strcpy(fn->name, name);
  fn->is_folder = is_folder;
  fn->next = 0;
  fl = (FLIST *)fltop;
  if (fl) {
  	
    FLIST *pr;
    pr = (FLIST *) &fltop;
    while (strcmp_nocase(fl->name, fn->name) < 0) {
      pr = fl;
      fl = fl->next;
      if (!fl) break;
    }
    fn->next = fl;
    pr->next = fn;
  }
  else {
    fltop=fn;
  }
  return (fn);
}

int FindFiles(char *str, int type) {// type == 0 SelectFolder, type == 1 SelectFile

  DIR_ENTRY de;
  unsigned int err;
  char *rev, *s, *d;
  int i, c, n = 0;
  char path[256];
  char name[128];
  
  strcpy(path, str);
  Free_FLIST();
  s = path;
  d = name;
  rev = 0;
  while ((c = *s++)) {
    *d++ = c;
    if (c == '\\' && *s != '\0') rev = d;
  }
  if(rev == 0)
    AddToFList("ROOT", back, IS_BACK);
  else {
    *rev=0;
    AddToFList(name, back, IS_BACK);
  }
  n++;
  
  i = strlen(path);
  path[i++] = '*';
  path[i] = '\0';
  if (FindFirstFile(&de, path, &err)) {
    do {
      i = strlen(de.folder_name);
      strcpy(path, de.folder_name);
      path[i++] = '\\';
      strcpy(path + i, de.file_name);
      if (de.file_attr & FA_DIRECTORY) {
        i = strlen(path);
        path[i++] = '\\';
        path[i] = 0;
        name[0] = '\\';
        strcpy(name + 1, de.file_name);
        AddToFList(path, name, IS_FOLDER);
        n++;
      }
      else {
        if (type != 0) {
          AddToFList(path,de.file_name, IS_FILE);
          n++;
        }
      }
    }
    while(FindNextFile(&de, &err));
  }
  FindClose(&de, &err);
  return n;
}

FLIST *FindFLISTtByNS(int *i, int si) {
  FLIST *fl;
  fl = (FLIST *)fltop;
  while (fl) {
    if (fl->is_folder == si) {
      if (!(*i)) return (fl);
      (*i)--;
    }    
    fl = fl->next;
  }
  return fl;
}
  
FLIST *FindFLISTtByN(int n) {
  
  FLIST *fl;
  fl = FindFLISTtByNS(&n, IS_BACK); if ((!n) && (fl)) return(fl);
  fl = FindFLISTtByNS(&n, IS_FOLDER); if ((!n) && (fl)) return(fl);
  fl = FindFLISTtByNS(&n, IS_FILE); if ((!n) && (fl)) return(fl);
  return fl;
}

void SavePath(FLIST *fl) {
	
	if (loadFile(fl->fullname)) {
   		//ShowMSG(1, (int)"File loaded!");
  	strcpy(procfile, fl->fullname);
  }
  else {
   	ShowMSG(1, (int)"File failed!");
	}
	/*
  EDITCONTROL ec;
  EDIT_ExtractFocusedControl(ed_gui,&ec);
  WSHDR *ws=AllocWS(ec.maxlen);
  str_2ws(ws,fl->fullname,ec.maxlen);
  EDIT_SetTextToFocused(ed_gui,ws);
  FreeWS(ws);*/
}

int filelist_menu_onkey(void *data, GUI_MSG *msg) {
	
  FLIST *fl;
  int i, n;
  i = GetCurMenuItem(data);
  fl = FindFLISTtByN(i);
  
  if (msg->keys == 0x3D || msg->keys == 0x18) {
  	
    if (fl) {
      if (fl->is_folder == IS_FOLDER || fl->is_folder == IS_BACK) {
        int len;
        if (strcmp(fl->fullname, "ROOT")) {
          strncpy(header, fl->fullname, sizeof (header) - 1);
          len = strlen(fl->fullname);
          header[len > sizeof(header) - 1 ? sizeof (header) - 1 : len] = 0;
          n = FindFiles(fl->fullname, 1);
        }
        else {
          int CreateRootMenu();
          n = CreateRootMenu();
        }         
        Menu_SetItemCountDyn(data, n);
        SetCursorToMenuItem(data, 0);
        RefreshGUI();
      }
      else {
      	SavePath(fl);        
        return 1;
      }
    }
    //    GeneralFunc_F1(1);
    return -1;
  }
  return 0;
}

void filelist_menu_ghook(void *data, int cmd) {
  if (cmd == 3) {
    Free_FLIST();
 	}
  if (cmd == 0x0A) {
    DisableIDLETMR();
  }
}

void filelist_menu_iconhndl(void *data, int curitem, void *user_pointer) {
	
  FLIST *fl;
  WSHDR *ws;
  void *item = AllocMenuItem(data);
  int len;
  fl = FindFLISTtByN(curitem);
  if (fl) {
    len = strlen(fl->name);
    ws = AllocMenuWS(data, len + 4);
    if (fl->is_folder == IS_BACK || fl->is_folder == IS_FOLDER) {
      str_2ws(ws, fl->name,len);
      wsInsertChar(ws, 0x0002, 1);
      wsInsertChar(ws, 0xE008, 1);
    }
    else {
      str_2ws(ws, fl->name, len);
    }
  }
  else {
    ws = AllocMenuWS(data, 10);
    wsprintf(ws, "?");
  }
  SetMenuItemText(data, item, ws, curitem);
}

HEADER_DESC filelist_HDR = {0, 0, 0, 0, NULL, (int)header, LGP_NULL};

MENU_DESC filelist_STRUCT = {
	
  8,filelist_menu_onkey,filelist_menu_ghook, NULL,
 	menusoftkeys,
  &menu_skt,
  0x10,
  filelist_menu_iconhndl,
  NULL,   //Items
  NULL,   //Procs
  0   //n
};

int CreateRootMenu() {
  char path[32];
  unsigned int err;
  int n = 0;
  Free_FLIST();
  for (int i = 0; i != 5; i++) {
    path[0] = i+'0';
    path[1] = ':';
    path[2] = '\\';
    path[3] = 0;
    if (isdir(path,&err)) {
      AddToFList(path, path, IS_FOLDER);
      n++;
    }
  }
  strcpy(header, "Root");
  return (n);
}


void open_select_file_gui(int type) {
  char *s, *rev=0;
  int n, c, len;
  
	s = procfile;
  while ((c = *s++)) {
    if (c == '\\' && *s != '\0') rev = s;
  }
  if (!rev)
    n = CreateRootMenu();
  else {
    *rev = 0;
    strncpy(header, procfile, sizeof (header) - 1);
    len = strlen(procfile);
    header[len > sizeof (header) - 1 ? sizeof (header) - 1 : len] = 0;
    n = FindFiles(procfile, type);
  }    
  patch_header(&filelist_HDR);
  CreateMenu(0, 0, &filelist_STRUCT, &filelist_HDR, 0, n, 0, 0);
}
/*
===============================
           end
===============================
*/

/*
===============================
         书签列表
===============================
*/
typedef struct BMKLIST {
  struct BMKLIST *next;
  struct BMKLIST *prior;
  int address;
  WSHDR *preview;
} BMKLIST;

BMKLIST *bltop = NULL;
void Free_BLIST(void) {       //清空列表
  BMKLIST *bl;
 	while (bltop) {
  	bl = bltop;
  	bltop = bltop->next;
  	FreeWS(bl->preview);
    mfree(bl);
  }
}

void Add_Blist_Item(int offset) {
  
  int len = 100;
  WSHDR *ws; 
  BMKLIST* bl = bltop;
  if (!bl) {
  	bltop = bl = malloc(sizeof (BMKLIST));
  	bltop->prior = NULL;
  }
  else {
  	while (bl->next) bl = bl->next;
  	bl->next = malloc(sizeof (BMKLIST));
  	bl->next->prior = bl;
  	bl = bl->next;
	}
  ws = AllocWS(len);
  char *tbuff = malloc(len);
  DataIndex *block = curblock;
  int view = viewpos;
  gotoPos(offset);
  str2ws_unicode(ws, buff + viewpos, 20);
  bl->preview = ws;
  bl->next = NULL;
  bl->address = offset;
  mfree(tbuff);
  curblock = block;
  viewpos = view;
}

int Fill_BLIST() {
	for (int i = 2; i < bmknum; i++) {
	  Add_Blist_Item(bookmark[i]);
	}
	return bmknum;
}

BMKLIST *FindBLISTtByN(int n) {
  BMKLIST *bl = bltop;
  int i = 0;
  while(bl && (i != n)) {
    bl = bl->next;
    i++;
  }
  return bl;
}

int bmklist_menu_onkey(void *data, GUI_MSG *msg) {
  int keycode = msg->keys;
  int i = GetCurMenuItem(data);
  BMKLIST* bl;
  bl = FindBLISTtByN(i);
  if (keycode == 0x3D || keycode == 0x18) {
    gotoPos(bl->address);
    GeneralFuncF1(1);
  }
  else if (keycode == 0x14) { //'*' 添加
  	if (add2bmk()) {
  		Add_Blist_Item(curblock->offset + viewpos);
  		Menu_SetItemCountDyn(data, bmknum - 2);
      SetCursorToMenuItem(data, 0);
  	}
	}
	else if (keycode == 0x15) { //'#' 删除
		if (deletebmk(i + 2)) {
			if (bl->prior) bl->prior->next = bl->next;
			else bltop = bl->next;
			if (bl->next) bl->next->prior = bl->prior;
			mfree(bl);
			Menu_SetItemCountDyn(data, bmknum - 2);
      SetCursorToMenuItem(data, 0);
		}
	}
  return 0;
}

void bmklist_menu_ghook(void *data, int cmd) {
  if (cmd == 3) {
    Free_BLIST();
    //loadBuffer(curblock->offset, curblock->alen);
  }
}

void bmklist_menu_iconhndl(void *data, int curitem, void *unk) {
  BMKLIST *t;
  WSHDR *ws;
  void *item = AllocMenuItem(data);
  t = FindBLISTtByN(curitem);
  if (t) {
  	ws = AllocMenuWS(data, t->preview->wsbody[0]);
    wstrcpy(ws, t->preview);
  }
  else {
    ws = AllocMenuWS(data, 10);
    wsprintf(ws, "error");
  }
  //SetMenuItemIconArray(data, item, S_ICONS);
  SetMenuItemText(data, item, ws, curitem);
  SetMenuItemIcon(data, curitem, 0);
}

HEADER_DESC bmklist_HDR = {0, 0, 0, 0, 0, (int)header, LGP_NULL};

MENU_DESC bmklist_STRUCT = {
  8, bmklist_menu_onkey, bmklist_menu_ghook, NULL,
  menusoftkeys,
  &menu_skt,
  1 + 0x10,
  bmklist_menu_iconhndl,
  NULL,   //Items
  NULL,   //Procs
  0   //n
};

int create_menu_bookmark(void) {
  Free_BLIST();
  Fill_BLIST();
  strncpy(header, "Bookmarks", 10);
  patch_header(&bmklist_HDR);
  int n = bmknum - 2;
  if (n < 0) n = 0;
	return CreateMenu(0, 0, &bmklist_STRUCT, &bmklist_HDR, 0, n, 0, 0);
}
/*
===============================
            end
===============================
*/


/*
===============================
            GUI
===============================
*/
const int minus11 = -11;
const RECT Canvas = {0, 0, 0, 0};   //窗口

typedef struct {
  GUI gui;
  //  WSHDR *ws1;
  //  WSHDR *ws2;
  //  int i1;
} MAIN_GUI;

typedef struct {
  CSM_RAM csm;
  int gui_id;
} MAIN_CSM;

void method0(MAIN_GUI *data) {
  if (data->gui.state == 2) {
    DrawScreen();
  }
}

void method1(MAIN_GUI *data, void *(*malloc_adr)(int)) {
  data->gui.state = 1;
}

void method2(MAIN_GUI *data, void (*mfree_adr)(void *)) {
  data->gui.state = 0;
}

void method3(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *)) {
  DisableIDLETMR();
  data->gui.state = 2;
}

void method4(MAIN_GUI *data, void (*mfree_adr)(void *)) {
  if (data->gui.state != 2) return;
  data->gui.state = 1;
}

void method7(MAIN_GUI *data, void (*mfree_adr)(void *)) {
  kill_data(data, mfree_adr);
  //  mfree_adr(data);
}

int method8(void){return 0;}

int method9(void){return 0;}

int method5(MAIN_GUI *data, GUI_MSG *msg) {
  int result = 0;
  if (msg->gbsmsg->msg == KEY_DOWN || msg->gbsmsg->msg == LONG_PRESS) {
    switch(msg->gbsmsg->submess) {
    case RED_BUTTON:
      Quit_Required = 1;
      /*result = 1;*/
      break;
    case GREEN_BUTTON:
    	LaunchEditor();
    	break;
    case ENTER_BUTTON:
    	break;
    case LEFT_SOFT:
    	create_menu_goto();
    	break;
    case RIGHT_SOFT:
      create_menu_main();
      break;
    case '2': case UP_BUTTON:
    	rowFluctuate(-1);
    	if (bmknum > 1) {
    		int offset = viewpos + curblock->offset;
    		bookmark[0] = offset;
    		if (offset > bookmark[1]) bookmark[1] = offset;
    		writeAddrInt(procfile, &bmkfd, &ul, bookmark, bmknum, acbm);
    	}
    	REDRAW();
    	break;
    case VOL_UP_BUTTON:
    case '1':
    	break;
    case '3':
    case '4': case LEFT_BUTTON:
      rowFluctuate(1 - viewrow);
      if (bookmark && bmknum > 1) {
    		int offset = viewpos + curblock->offset;
    		bookmark[0] = offset;
    		if (offset > bookmark[1]) bookmark[1] = offset;
    		writeAddrInt(procfile, &bmkfd, &ul, bookmark, bmknum, acbm);
    	}
      REDRAW();
      break;
    case '5':
    	showinfo = !showinfo;
    	//DrawInfoDialog();
    	break;
    case '6': case RIGHT_BUTTON:
      rowFluctuate(viewrow - 1);
      if (bookmark && bmknum > 1) {
    		int offset = viewpos + curblock->offset;
    		bookmark[0] = offset;
    		if (offset > bookmark[1]) bookmark[1] = offset;
    		writeAddrInt(procfile, &bmkfd, &ul, bookmark, bmknum, acbm);
    	}
      REDRAW();
      break;
    case '7':
    case '8': case DOWN_BUTTON:
      rowFluctuate(1);
      if (bookmark && bmknum > 1) {
    		int offset = viewpos + curblock->offset;
    		bookmark[0] = offset;
    		if (offset > bookmark[1]) bookmark[1] = offset;
    		writeAddrInt(procfile, &bmkfd, &ul, bookmark, bmknum, acbm);
    	}
      REDRAW();
      break;
    case '9':
    case '0':      
    case '*':
      break;
    case '#':
      break;
    default:
      break;
    }
    return result;
  }
  return 0;
}

const void * const gui_methods[11] = {
  (void *)method0,	//Redraw
  (void *)method1,	//Create
  (void *)method2,	//Close
  (void *)method3,	//Focus
  (void *)method4,	//Unfocus
  (void *)method5,	//OnKey
  0,
  (void *)method7,	//Destroy
  (void *)method8,
  (void *)method9,
  0
};
/*
===============================
            end
===============================
*/


/*
===============================
            CSM
===============================
*/
unsigned short maincsm_name_body[140];
void maincsm_oncreate(CSM_RAM *data) {
  
  MAIN_GUI *main_gui = malloc(sizeof (MAIN_GUI));
  MAIN_CSM *csm = (MAIN_CSM *) data;
  zeromem(main_gui, sizeof (MAIN_GUI));
  patch_rect((RECT*)&Canvas, 0, 0, ScreenW() - 1, ScreenH() - 1);
  main_gui->gui.canvas = (void *)(&Canvas);
  main_gui->gui.flag30 = 2;
  main_gui->gui.methods = (void *)gui_methods;
  main_gui->gui.item_ll.data_mfree = (void (*)(void *))mfree_adr();
  csm->csm.state = 0;
  csm->csm.unk1 = 0;
  csm->gui_id = CreateGUI(main_gui);
}

void maincsm_onclose(CSM_RAM *csm) {
  SUBPROC((void *)ElfKiller);
}

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg) {
  MAIN_CSM *csm = (MAIN_CSM *) data;
  
  if (Quit_Required) {
    fclose(fd, &ul);
    GeneralFuncF1(1);
  }
  
  if (msg->msg == MSG_GUI_DESTROYED) {
    if ((int)msg->data0 == csm->gui_id) {
      csm->csm.state = -3;
    }
  }
  
  else if (msg->msg == MSG_RECONFIGURE_REQ) {
  	LockSched();
		InitSettings();
		if (buffloaded && charset != codetype) {
			KillIndex();
			if (createIndex()) {
				curblock = fileindex->next;
				loadBuffer();
				viewpos = 0;
			}
		}
		UnlockSched();
    RefreshGUI();
  }
  return 1;
}

const struct {
  CSM_DESC maincsm;
  WSHDR maincsm_name;
} MAINCSM = {
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
    139
  }
};

//更新进程名称
void UpdateCSMname(void) {
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "SieTxtView");
  
  /*
  //中文名
  char csmname[] = {0};
  str2ws_unicode((WSHDR *)(&MAINCSM.maincsm_name), csmname);
  */
}
/*
===============================
           end
===============================
*/

/*
===============================
          程序入口
===============================
*/
int main(char *exename, char *fname) {

  TDate d;
  TTime t;
  GetDateTime(&d, &t);
  srand((unsigned int)(d.year + d.month + d.day + t.hour + t.min + t.sec));
  //打开文件
  buff = malloc(BUF_LEN);
  if (!buff) {
  	ShowMSG(1, (int)"Alloc memory error!");
  	return 0;
	}
	
  if (fname && strlen(fname) < 128)
    strcpy(procfile, fname);
  else
    strcpy(procfile, LAST);
  
	if (!loadFile(procfile)) adjustWindowsSize();
  char dummy[sizeof(MAIN_CSM)];
  LockSched();
  CreateCSM(&MAINCSM.maincsm, dummy, 0);
  UnlockSched();
  UpdateCSMname();

	//fclose(fd, &ul);}
  return 1;
 
}
/*
===============================
            end
===============================
*/
