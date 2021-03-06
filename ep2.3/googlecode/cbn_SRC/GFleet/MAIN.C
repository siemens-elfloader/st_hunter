
// GFleet Demo Clips

//#define HIGHCOLOR
//#define GRAPH
//#define UNPACK // ���������� �������
//#define NOZIP  // ����� zip
//#define NORAR  // ����� rar
//#define NOLOAD 
#include "..\swilib.h"

extern short *screen; // ����� 132*176*2
extern void DrawScreen(); // ������� ����������� ������
void onredraw();

extern int un7zinit(char *buf);
extern char *un7znext();
extern void un7zexit();

char *Title="GFleet 0.3 (c)bn";
const char *errors[]={
  0
};

extern unsigned char SGOLD2, C65, S75, EL71;
extern int width, height, screensize;

//#define RGB8(R,G,B) (B+(G<<2)+(R<<5))
#define RGB16(R,G,B) ((B>31?31:B)+((G>63?63:G)<<5)+((R>31?31:R)<<11))

typedef unsigned char byte;

// ���� ����� ������ 7z �����!!!! ����� ��������� ��� � ���� �������!
#define datasize 374858
char data[datasize]="INSERT 7Z HERE!!!";
char *buf=0; 
volatile int running=0; // ������� ��������� ���� � ������� ���� =0
int maxframe=0, frame=0;

GBSTMR timer;

void timer_proc(void){ // ������� ����������� �� ������� 10 ��� � �������
  if(running){
  short *s, *ss, *b;
  int i,j;
reopen: //������ ������� 7z ����
  if(loadfilesize==-1){
    un7zexit();
    loadfilesize=datasize;
    if(!un7zinit(data)) return;
    frame=maxframe=0;
  }
  if(++frame>=maxframe){
    if(buf) mfree(buf);
    if(!(buf=un7znext())){
      if(loadfilesize!=-1) return;
       goto reopen;
    }
    if(!(buf=loadgraph(buf))) return;
    maxframe=*(short*)(buf+2)/80;
    frame=0;
  }
  if(EL71){//EL
  s=ss=screen+15*240+240-1;
  b=(short*)buf;
  b+=2+frame*80*179;
  for(j=0;j<80;j++){
    for(i=0;i<179/2;i++){
      s[0]=s[-1]=s[-2]=
      s[240]=s[240-1]=s[240-2]=*b++;
      s[480]=s[480-1]=s[480-2]=*b++;
      s+=720;
    }
    b+=1;
    s=(ss-=3);
  }
  }else{ //132*176
  ss=screen+132-26;
  b=(short*)buf;
  b+=2+frame*80*179;
  for(j=0;j<80;j++){
    s=--ss;
    for(i=0;i<176;i++,s+=132){
      *s=*b++;
    }
    b+=3;
  }
  }
  REDRAW();
  GBS_StartTimerProc(&timer,262/4,timer_proc); // �������� ������ � �������� 10 ��� � �������
}}


void onredraw(){ // ����������� ������ screen[132*176]
}

// keycode - ��� �������, pressed - ������/��������/������������
int onkey(unsigned char keycode, int pressed){ //  ���������� ������. ������� 0 ��� 1 ��� ������
  switch(pressed){      
  case KEY_UP: break; // ������� ��������
  case LONG_PRESS: // ������� ������������ ����� (������)
  case KEY_DOWN: // ������� ������
    switch(keycode){ // ��� �������
    case RED_BUTTON: 
    case GREEN_BUTTON:
    case LEFT_SOFT:
    case RIGHT_SOFT:
      return 1; // �����
    }
  }
  return 0;
}

void onclose(){ // �������� ����
  running=0; // ������ �� ��������� �������� ��������
  GBS_DelTimer(&timer); // ������������� ������  pickoff!!
  un7zexit();
}

void onexit(){ // �����
  if(buf) mfree(buf); // ����������� ������
}

void oncreate(){ // �������� ����
//  SUBPROC((void *)execute); // ��������� ������� �������
  running=1;
  GBS_StartTimerProc(&timer,262/20,timer_proc); // �������� ������ � �������� 2 ��� � �������
}

int onstart(char *exename, char *fname){ // ����� ����������. ���������� 0 ��� 1 ��� ������.
  loadfilesize=-1;
  return 0;
}
