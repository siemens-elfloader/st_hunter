//////////////////// Обьявляем переменные /////////////////////////

#define idlegui_id (((int *)data)[DISPLACE_OF_IDLEGUI_ID/4])
#define SCREENSIZE ScreenH()*ScreenW()
#define HSIZE YDISP*ScreenW()
const int minus11=-11;

const char ipc_my_name[]=IPC_FASTRUN_NAME;

const IPC_REQ my_ipc={
  ipc_my_name,
  ipc_my_name,
  NULL
};

int FASTRUNMENU_GUI_ID;// ИД гуя
int CONFMenuActive=0;
extern const char *successed_config_filename;
extern const char FILE_EMENU[128];
extern const int IDLE_GORIZO;
int IDLE_GORIZ;
extern unsigned int SOUND_11;
extern const int K1;
extern const int L1;
extern const int M1;
extern const int N1;
extern const int P1;
extern const int R1;
 int K;
 int L;
 int M;
 int N;
 int P;
 int R;



extern void kill_data(void *p, void (*func_p)(void *));

//CSM_DESC icsmd;

//GUI *igui;
#define TMR_SECOND 216// секунда
GBSTMR idle_timer;
extern const int FR_CALL_BUTTON;// Кнопка активации панели FastRun
extern const int FR_EXIT_BUTTON;// Кнопка закрытия панели FastRun
//int Run;
extern const int IDLE_CALL_BUTTON;// Кнопка активации панели FastRun
extern const int IDLE_EXIT_BUTTON;// Кнопка закрытия панели FastRun
int mode_enter;
int idle_pic_size;
int fr_pic_size;
int ConfigUpdated;
extern const int IDLE_PIC_SIZE;
extern const int FR_PIC_SIZE;
extern const int ENA_FR_LONGPRESS;
extern const int ENA_IDLE_LONGPRESS;

extern const unsigned int LEN_ID;
extern const unsigned int LEN;
extern const unsigned int BEGIN;
extern const int DRAW_MODE;
extern const int IDLE_ENA;
extern const int FASTRUN_ENA;
extern const unsigned int TEXT_FONT;
int FR_KEYHOOK_ENA;
int IDLE_KEYHOOK_ENA;

char* percent_t="%t";

int NumOfItems_idle;// Число итемов меню 
int Selected_idle;// Позиция курсора
int NumOfItems_fr;
int Selected_fr;
int FastRunMenuActive;// Флаг активности меню
int IdleMenuActive;
extern const unsigned int xx1;
extern const unsigned int yy1;
unsigned int x1;
unsigned int y1;


unsigned char scrH, scrW;
int x1_fr;
int x2_fr;
int y1_fr;
int y2_fr;


void EditConfig()
{
  extern const char *successed_config_filename;
  WSHDR *ws;
  ws=AllocWS(150);
  str_2ws(ws,successed_config_filename,128);
  ExecuteFile(ws,0,0);
  FreeWS(ws);
}

int lenta;
extern const char cfgPanBorderCol[4];
extern const char cfgPanBGCol[4];
extern const char cfgBookBorderCol[4];
extern const char cfgBookBGCol[4];
extern const char cfgPanBGCol_ID_ena[4];
extern const char cfgPanBGCol_ID_dis[4];
extern const char cfgBookBorderCol_ID[4];
extern const char cfgBookBGCol_ID[4];
extern const char cfgFontCol_ID[4];

extern const char BM1PIC_ID[128];
extern const char BM2PIC_ID[128];
extern const char BM3PIC_ID[128];
extern const char BM4PIC_ID[128];
extern const char BM5PIC_ID[128];
extern const char BM6PIC_ID[128];
extern const char BM7PIC_ID[128];
extern const char BM8PIC_ID[128];
extern const char BM9PIC_ID[128];
extern const char BM10PIC_ID[128];
extern const char BM11PIC_ID[128];
extern const char BM12PIC_ID[128];
extern const char BM13PIC_ID[128];
extern const char BM14PIC_ID[128];
extern const char BM15PIC_ID[128];

extern const char BM1FILE_ID[128];
extern const char BM2FILE_ID[128];
extern const char BM3FILE_ID[128];
extern const char BM4FILE_ID[128];
extern const char BM5FILE_ID[128];
extern const char BM6FILE_ID[128];
extern const char BM7FILE_ID[128];
extern const char BM8FILE_ID[128];
extern const char BM9FILE_ID[128];
extern const char BM10FILE_ID[128];
extern const char BM11FILE_ID[128];
extern const char BM12FILE_ID[128];
extern const char BM13FILE_ID[128];
extern const char BM14FILE_ID[128];
extern const char BM15FILE_ID[128];

extern const char BM1NAME[32];
extern const char BM2NAME[32];
extern const char BM3NAME[32];
extern const char BM4NAME[32];
extern const char BM5NAME[32];
extern const char BM6NAME[32];
extern const char BM7NAME[32];
extern const char BM8NAME[32];
extern const char BM9NAME[32];
extern const char BM10NAME[32];
extern const char BM11NAME[32];
extern const char BM12NAME[32];
extern const char BM13NAME[32];
extern const char BM14NAME[32];
extern const char BM15NAME[32];

extern const char BM1PIC_FR[128];
extern const char BM2PIC_FR[128];
extern const char BM3PIC_FR[128];
extern const char BM4PIC_FR[128];
extern const char BM5PIC_FR[128];
extern const char BM6PIC_FR[128];
extern const char BM7PIC_FR[128];
extern const char BM8PIC_FR[128];
extern const char BM9PIC_FR[128];
extern const char BM10PIC_FR[128];
extern const char BM11PIC_FR[128];
extern const char BM12PIC_FR[128];
extern const char BM13PIC_FR[128];
extern const char BM14PIC_FR[128];
extern const char BM15PIC_FR[128];

extern const char BM1FILE_FR[128];
extern const char BM2FILE_FR[128];
extern const char BM3FILE_FR[128];
extern const char BM4FILE_FR[128];
extern const char BM5FILE_FR[128];
extern const char BM6FILE_FR[128];
extern const char BM7FILE_FR[128];
extern const char BM8FILE_FR[128];
extern const char BM9FILE_FR[128];
extern const char BM10FILE_FR[128];
extern const char BM11FILE_FR[128];
extern const char BM12FILE_FR[128];
extern const char BM13FILE_FR[128];
extern const char BM14FILE_FR[128];
extern const char BM15FILE_FR[128];

//////////////////////////////////////////////////////////

int savefile(char *fname, char *buf, int size){
  int f,i;
  if((f=fopen(fname,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&err))==-1) return 0;
  i=fwrite32(f,buf,size,&err); 
  fclose(f,&err); 
  return i;
}

//------------------------------------------------------------------------------
unsigned int err=0;
int loadfilesize=0;
char *loadfile(char *filename){ // Загрузить файл, распак-ть
  int f; unsigned int err;  char *buf=0;
  if((f=fopen(filename,A_ReadOnly+A_BIN,P_READ,&err))!=-1){
    if(loadfilesize=lseek(f,0,2,&err,&err)){
      lenta=loadfilesize;
      buf=(char*)malloc(loadfilesize);
      lseek(f,0,0,&err,&err);
      if(fread32(f,buf,loadfilesize,&err)!=loadfilesize){ mfree(buf); buf=0;}
    } fclose(f,&err);
  } return buf;
}

char *intToChar(int data){ // Записать int
char *buf=0;    
buf="               ";
sprintf(buf,"%i ",data);
return buf;  
}

int saveconf(){
  int *iteg;
  char *strokata;
strokata = loadfile("4:\\ZBin\\etc\\FastRun_Night.bcfg");
if (x1<256) {strokata[7144]=x1; strokata[7145]=0;} 
if (y1<400) {if (y1<256) {strokata[7148]=y1; strokata[7149]=0;} else {strokata[7144]=y1-255; strokata[7145]=1;}}
strokata[14470]=255;strokata[14471]=255;
strokata[14518]=255;strokata[14519]=255;
strokata[14566]=255;strokata[14567]=255; //для совместимости изменяет старую версию конфига
iteg =(int *)&strokata[14472];
*iteg=K;
//char *mozg="";
//mozg=intToChar(M);
//ShowMSG(0,(int)mozg);

iteg=(int *)&strokata[14568];
*iteg=M;
strokata[14664]=P;
strokata[6708]=IDLE_GORIZ;


savefile("4:\\ZBin\\etc\\FastRun_Night.bcfg", strokata, lenta);

  return 0;
}

int no( int f){
if (!f) return 2; else return 0;
}