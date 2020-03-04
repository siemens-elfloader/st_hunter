
extern char COLOUR[4], COLOUR_FRING[4];

extern char CURSOR_COLOUR[4],CURSOR_COLOUR_FRING[4], hostname[128], logmsg[256];;

extern int DoneConnected,
           FONT,
           Font_H,
           connect_state,
           RXstate,
           TYPE_DRAW;

extern volatile int sendq_l;
// Unsigned int:

extern volatile unsigned long ALLTOTALRECEIVED,ALLTOTALSENDED;
// Visual parametres
extern unsigned int ScrW, ScrH;

// Void
extern void kill_data(void *p,void (*func_p)(void *));

// Const char
extern const char percent_t[],
                  percent_d[],
                  percent_s[];

//===============================================================================================
#pragma inline
void patch_rect(RECT*rc,int x,int y, int x2, int y2)
{
  rc->x=x;
  rc->y=y;
  rc->x2=x2;
  rc->y2=y2;
}

#pragma inline
void patch_header(const HEADER_DESC* head)
{
  ((HEADER_DESC*)head)->rc.x=0;
  ((HEADER_DESC*)head)->rc.y=YDISP;
  ((HEADER_DESC*)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC*)head)->rc.y2=HeaderH()+YDISP-1;
}
#pragma inline
void patch_input(const INPUTDIA_DESC* inp)
{
  ((INPUTDIA_DESC*)inp)->rc.x=0;
  ((INPUTDIA_DESC*)inp)->rc.y=HeaderH()+1+YDISP;
  ((INPUTDIA_DESC*)inp)->rc.x2=ScreenW()-1;
  ((INPUTDIA_DESC*)inp)->rc.y2=ScreenH()-SoftkeyH()-1;
}
//===============================================================================================


//"obexcopy.exe" $EXE_DIR$\NatICQ.elf \MMCard\ZBin\NatICQ\NatICQ.elf
