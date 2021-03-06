
// Durak

#include "..\swilib.h"
#include "..\random.h"

extern short *screen; // ����� 132*176*2
extern void DrawScreen(); // ������� ����������� ������
void onredraw();
typedef unsigned char byte;
extern int width, height, screensize;
extern unsigned char SGOLD2, C65, S75, EL71;

char *Title="Durak 0.3 (c)bn";

#define forg for(i=0;i<strlen(girl);i++)

const char *errors[]={
  0,
  "Mess.bmp �� ������!\0",
  "Cards.bmp �� ������!\0",
  "Font.bmp ����� �� ������!\0",
//  "���� ������ ��� ������\0",
  "unknown error"
};

//������� ����
#define cardx 19
#define cardy 27

char filename[128], *ext;
char *mess=0, *bmpcards=0, *fon=0, *font=0; //free
char *msg[256];
int maxmsg;
int razdel[5];  //������ ������� ������� 0=���������, 1=...
enum{ msgvictory, msgloss, msgkozir};
enum{ razsluj, razstart, razend, razloss, razvictory};

#define maxcards 37
char coloda[maxcards], player[maxcards], girl[maxcards], current[maxcards]; // �����... ��������
char tmp1[maxcards], tmp2[maxcards];
byte fastfon=0, kozir=0; //������

const char girlbmp[]="Girl00.bmp";

int mode=0;
char level=0;
int curx=0;
int showmsg=0;
//int xod=0; //��� ���: ������=�


void randmsg(int raz){
  showmsg=razdel[raz]+randint(razdel[raz+1]-razdel[raz]-1);
}

inline void centerbmp(char *bmp){
  if(bmp){
  int x=(width-*(short*)bmp)>>1, y=(height-*(short*)(bmp+2))>>1;
    bitblt(screen, bmp, x<0?0:x, y<0?0:y, 0,0,0,0,0,0);
  }
}

inline void cls(){ memsetd(screen,0,screensize>>2);}

inline void showfon(){ 
  if(fastfon) memcpy(screen,fon+4,screensize);
  else{
    cls();
    centerbmp(fon);
  }
//  showstart();
}

void drawcard(int x, int y, int card){ // ���������� �����. 0=�������
  if(card){
    card=((card-6)&0xf)+(card>>4)*9;
  }else card=36;
  bitblt(screen,bmpcards, x,y, cardx,cardy, (card%6)*cardx, (card/6)*cardy, 0,0);
}

void showcards(char *who, int y, int deltax){
  int i,len,x=0, dx;
  if(len=strlen(who)){
    dx=width/(len+deltax);
    for(i=0;i<len;i++){
      drawcard(x,y,y?who[i]:0);
      x+=dx;
    }
  }
}

inline void showcursor(){
  int len=strlen(player);
  if(curx>=len) curx=0;
  if(len){
    bitblt(screen,bmpcards, width/len*curx,height-cardy, cardx,cardy, (38%6)*cardx, (38/6)*cardy, 0,0x40);
  }
}

inline void showcurrent(){
  char *s=current, *s1=tmp1, *s2=tmp2;
  while(*s){
//    if(!xod) 
      *s1++=*s++;
//    else xod=0;
    if(*s) *s2++=*s++;
  }
  *s1=*s2=0;
  showcards(tmp1,(height>>1)-cardy-8,0);
  showcards(tmp2,height>>1,strlen(tmp1)-strlen(tmp2));
//  showcards(current,(height-cardy)>>1);
}

void onredraw(){ // ����������� ������ screen[132*176]
  short *scr;
  byte *bb, b;
  char *tmsg;
  int i,j, dx, dy;
  //
  showfon();
  showcards(girl,0,0);
  //showcurrent();
  showcards(current,height>>1,0);
  showcards(player,height-cardy,0);
  showcursor();
  if(showmsg){/////////������� ���������
    tmsg=msg[showmsg]-1;
    dx=dy=0;
    while(*++tmsg){
      if(dx>=width-8){
        dx=0; dy+=10;
      }
      scr=screen+dy*width+dx;
      bb=(byte*)font+*(byte*)tmsg * 8;
      for(j=0;j<8;j++){
        b=*bb++;
        for(i=0;i<8;i++){
          *scr++=(b&0x80)?0:0xffff;
          //if(b&0x80) *scr=0xffff;
          //scr++;
          b<<=1;
        }
        scr+=width-8;
      }
      dx+=8;
    }
    showmsg=0;
  }
}

void delcard(char *who, int pos){ //������� �����
  who[pos]=who[strlen(who)-1];
  who[strlen(who)-1]=0;
}

int getcard(char *who){ // �������� �����. return 0=no card
  int rnd;
  if(!*coloda) return 0;
  rnd=random(strlen(coloda));
  who[strlen(who)]=coloda[rnd];
  delcard(coloda,rnd);
//  coloda[rnd]=coloda[strlen(coloda)-1];
//  coloda[strlen(coloda)-1]=0;
  return 1;
}


int putcard(char card){ // �������� �����. return 0=����
  char *cur=current+strlen(current)-1;
  char last=*cur;
  int i;
  if(strlen(current)&1){//����������
    if((card>>4)==kozir && (last>>4)!=kozir); //�����
    else{
      if((card>>4)==(last>>4) && (card&0xf)>(last&0xf));//�����
      else return 0; //����
    }
  }else{ //�������
    if(strlen(current)){
      for(i=0;i<strlen(current);i++)
        if((current[i]&0xf)==(card&0xf)) goto ata;
      return 0;
    }
  }
ata:  
  *++cur=card;
  *++cur=0;
  return 1;
}

inline void pickall(char *who){ //����� ��� �����
  strcat(who,current);
  *current=0;
}

void pickup(){ //����� �� ������ �� 6 ����
  while(strlen(player)<6)
    if(!getcard(player)) break;
  while(strlen(girl)<6)
    if(!getcard(girl)) break;
}

int findmin(char card){ //����� ��� ����� ������ ����� -1=���
  int pos=-1, i;
  forg{
    if((girl[i]>>4)==(card>>4) && girl[i]>card)
      if(pos<0) pos=i;
      else if(girl[i]<girl[pos]) pos=i;
  }
  return pos;//i error!
}

char pair[9];//������� ���

void findpair(int koz){ //����� ���� ��� �����, koz - �������� �����
  int i;
  memset(pair,0,9);
  forg{
    if(koz || (girl[i]>>4)!=kozir)
      ++pair[(girl[i]&0xf)-6];
  }
}

void AIdefence(){ //����������
  int i;
  int cnt=strlen(current);
  char cur=current[cnt-1];
  if((cur>>4)==kozir){ //���� ������
    if(cnt<4 && strlen(coloda)>6){ //���� ������ � ������ � ����� � ������ - �����
lget:      pickall(girl);  pickup(); return; 
    } 
    goto lkoz;
  }else{ //�� ������
    if((i=findmin(cur))<0){ //���� ��� ����� �����
lkoz:
      if((i=findmin((kozir<<4)+5))<0) goto lget; //���� ��� ������� �����
      if(strlen(coloda)<7 || cnt>8) ; //���� ���� �������� ��� ����� ���� �� ����� �����
      else{
        if((girl[i]&0xf)>9) goto lget; //�����   rnd()
      }
    }
  }
  putcard(girl[i]);
  delcard(girl,i);
}

void AIcont(){ //������� ���������� // ����� ����������� �� ���� (���������� �����)
}

void AIatack(){ //������� � ���� (���� ���� � ���� ��������)
  int i,k,flag=0;
lll:  
  findpair(flag); //���� ���������� ����
  for(k=0;k<6;k++)
    if(pair[k]>1) break;
  if(k>=6){ //��� ��� - ����� ����������� ���������� �����
    for(k=0;k<9;k++)
      if(pair[k]>0) break;
    if(k>=9){ //��� ������ - ����� � ������������
      if(flag) return;/////error randcard here!!!
      flag=1;
      goto lll;
    }
  }
  forg{
    if(((flag || girl[i]>>4)!=kozir) && (girl[i]&0xf)==k+6) break;
  }
  putcard(girl[i]);
  delcard(girl,i);
}

void newgame(){
  int i,j;
//  xod=0;//rand()&1;
  //��������� ������
  char *s=coloda;
  for(j=0;j<4;j++)
    for(i=0;i<9;i++) *s++=(i+6)+(j<<4);
  *s=*current=0; 
  curx=0;
  kozir=random(4);
  //������� �����
  memset(player,0,37);
  memset(girl,0,37);
  for(i=0;i<6;i++){ 
    getcard(player); 
    getcard(girl);
  }
  //��������
  *(char*)(girlbmp+5)=level+'0';
  strcpy(ext,girlbmp);
  if(fon) mfree(fon);
  if((fon=loadgraph(filename)) && *(short*)fon==width && *(short*)(fon+2)==height) fastfon=1;
  //
  showmsg=razdel[razsluj]+msgkozir+kozir;
}

void ai(){ //������������� ���������
  if(strlen(current)&1) AIdefence(); 
  else if(strlen(current)) AIcont(); 
  else AIatack();
//  xod=1;
  REDRAW();
  if(!strlen(player) || !strlen(girl)) newgame(); //�������!!! ����� ��� ��������.. ���������� ������ � �.�.
  //levelup
}

// keycode - ��� �������, pressed - ������/��������/������������
int onkey(unsigned char keycode, int pressed){ //  ���������� ������. ������� 0 ��� 1 ��� ������
  switch(pressed){      
  case KEY_UP: break; // ������� ��������
  case LONG_PRESS: // ������� ������������ ����� (������)
  case KEY_DOWN: // ������� ������
    //if(mode)//????
    switch(keycode){ // ��� �������
    case RED_BUTTON: return 1; // �����
    case LEFT_SOFT: case GREEN_BUTTON: newgame(); break;
  
    case LEFT_BUTTON: 
      if(curx>0) --curx; else curx=strlen(player)-1; break;
    case RIGHT_BUTTON: 
      if(curx<strlen(player)-1) ++curx; else curx=0; break;
      
    case UP_BUTTON: case DOWN_BUTTON: //����� �����
      pickall(player);
      randmsg(razstart);
      ai(); 
      break;

    case RIGHT_SOFT: case ENTER_BUTTON: // ��������
      if(putcard(player[curx])){
        delcard(player,curx);
        ai();
        break;
      }else return 0;
    default: return 0;
    }
//    onredraw(); DrawScreen(); // ������������ �����
    REDRAW();
  }
  return 0;
}


void onclose(){ // �������� ����
}

void onexit(){ // �����
  if(mess) mfree(mess); // ����������� ������
  if(bmpcards) mfree(bmpcards);
  if(fon) mfree(fon);
  if(font) mfree(font);
}


void oncreate(){ // �������� ����
  newgame();
}


void txt(){
  int i, raz;
  char *s=mess, **ptr=msg;
  mess[loadfilesize-2]=0;
  //�������������
//  for(i=0, s=data; i<datasize; i++,s++) *s=~*s; 
  //����������� � Daemons...
//  daemcopy(exename);
 //�������� �����
  do{
    while(*s==' ') ++s;
    *ptr++=s;
    --s;
    while(*++s && *s!=0xd);
    if(!*s) break;
    *s=0; s+=2;
  }while(1);
  *ptr=0;
  maxmsg=ptr-msg;
  //����������� �������
  i=raz=0;
  while(i<maxmsg){
    if(*msg[i]=='['){
      razdel[raz++]=i+1;
    }
    ++i;
  }
  razdel[raz]=maxmsg;
}

int onstart(char *exename, char *fname){ // ����� ����������. ���������� 0 ��� 1 ��� ������.
  strcpy(filename,exename);
  ext=strrchr(filename,'\\')+1;
  strcpy(ext,"mess.bmp");
  if(!(mess=loadfile(filename))) return 1;
  txt();
  strcpy(ext,"cards.bmp");
  if(!(bmpcards=loadgraph(filename))) return 2;
  strcpy(ext,"font.bmp");
  if(!(font=loadfile(filename))) return 3;
//  if(!(back=(char*)malloc(screensize))) return 4;
  randomize();
  return 0;
}
