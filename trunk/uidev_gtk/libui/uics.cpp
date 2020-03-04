
#include <alib/ui.h>
#include <alib/io.h>
#include <alib/io_ui.h>

const char *theme[]={
  "background",
  "panel",
  "panel2",

  "header",
  "header2",

  "scrollbg",
  "scrollbg2",

  "scroll",
  "scroll2",

  "cursor",
  "cursor2",

  "font-header",
  "font-menu",
  "font-act-menu",
  "font-soft",

  "size_panel_up",
  "size_panel_down",
  "size_header"
};
/*
void Log (const char *txt)
{
  unsigned int ul;
  int f=_open("0:\\uitesting.txt",A_ReadWrite+A_Create+A_Append+A_BIN,P_READ+P_WRITE,&ul);
  if (f!=-1)
  {
    _write(f,txt,strlen(txt),&ul);
    _close(f,&ul);
  }
}
*/

int hex_ch2int (char ch){
    if (ch>='A' && ch<='F') return ch-'A'+10;
	else return ch-'0';
}

unsigned int hex_str2int (char const *str){
	int val=0;
	for (int i=0; str[i]!='\0'; i++){
		val=val*16+hex_ch2int (str[i]);
	}
	return val;
}


#define MAX_THEME_ELEMENTS 64
struct ThemeList{
	char name[64];
	char value[64];
};

int ThemeItemsCount=0;
ThemeList tlist[MAX_THEME_ELEMENTS];

int ParseTheme (char *fname){
    SimpleParser file;
    file.Open ((char*)fname);

    if (file.GetCount ()>0){
        while (file.GetCur ()<file.GetCount ()){
            if (file.GoToSymb ('.')==-1) return 1;
            if (file.CopyToSymb ((char*)tlist[ThemeItemsCount].name, ':')==-1) return -1;

            if (file.GoToSymb ('#')==-1) return -1;
            if (file.CopyToSymb ((char*)tlist[ThemeItemsCount].value, ';')==-1) return -1;

            ThemeItemsCount++;
        }
        file.Close ();
    }

    return 0;
}

char *GetValueByName (const char *name){
    for (int i=0; i<ThemeItemsCount; i++){
        if (!strcmp (name, tlist[i].name)) return tlist[i].value;
    }
    return NULL;
}

void LoadTheme (char const *path){
    if (GetFileSize ((char*)path)){
        ParseTheme ((char*)path);

        //for (int i=0; i<ThemeItemsCount; i++) set_color_to_set (i, hex_str2int (GetValueByName (theme[i])));

        set_color_rgb32_to_set (CLR_BACKGROUND, hex_str2int (GetValueByName (theme[CLR_BACKGROUND])));

        set_color_rgb32_to_set (CLR_PANEL_BEG, hex_str2int (GetValueByName (theme[CLR_PANEL_BEG])));
        set_color_rgb32_to_set (CLR_PANEL_END, hex_str2int (GetValueByName (theme[CLR_PANEL_END])));

        set_color_rgb32_to_set (CLR_HEADER_BEG, hex_str2int (GetValueByName (theme[CLR_HEADER_BEG])));
        set_color_rgb32_to_set (CLR_HEADER_END, hex_str2int (GetValueByName (theme[CLR_HEADER_END])));

        set_color_rgb32_to_set (CLR_SCROLL_BG_BEG, hex_str2int (GetValueByName (theme[CLR_SCROLL_BG_BEG])));
        set_color_rgb32_to_set (CLR_SCROLL_BG_END, hex_str2int (GetValueByName (theme[CLR_SCROLL_BG_END])));

        set_color_rgb32_to_set (CLR_SCROLL_BEG, hex_str2int (GetValueByName (theme[CLR_SCROLL_BEG])));
        set_color_rgb32_to_set (CLR_SCROLL_END, hex_str2int (GetValueByName (theme[CLR_SCROLL_END])));

        set_color_rgb32_to_set (CLR_CURSOR_BEG, hex_str2int (GetValueByName (theme[CLR_CURSOR_BEG])));
        set_color_rgb32_to_set (CLR_CURSOR_END, hex_str2int (GetValueByName (theme[CLR_CURSOR_END])));

        set_color_rgb32_to_set (CLR_FONT_HEADER, hex_str2int (GetValueByName (theme[CLR_FONT_HEADER])));
        set_color_rgb32_to_set (CLR_FONT_MENU, hex_str2int (GetValueByName (theme[CLR_FONT_MENU])));
        set_color_rgb32_to_set (CLR_FONT_ACT_MENU, hex_str2int (GetValueByName (theme[CLR_FONT_ACT_MENU])));
        set_color_rgb32_to_set (CLR_FONT_SOFT, hex_str2int (GetValueByName (theme[CLR_FONT_SOFT])));

        set_color_to_set (PANEL_UP_SIZE, str2int (GetValueByName (theme[PANEL_UP_SIZE])));
        set_color_to_set (PANEL_DOWN_SIZE, str2int (GetValueByName (theme[PANEL_DOWN_SIZE])));
        set_color_to_set (PANEL_HEADER_SIZE, str2int (GetValueByName (theme[PANEL_HEADER_SIZE])));

    }

}



