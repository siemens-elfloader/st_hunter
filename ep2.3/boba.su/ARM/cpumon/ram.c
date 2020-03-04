#include "..\inc\swilib.h"
#include "cpumon.h"
#include "conf_loader.h"
#include "render.h"

extern int const cfgRamType;
extern int const cfgRamDiv;
extern const char cfgFreeRAM[4];
extern int uiWidth, uiHeight;

static void init(int rewidth);
static void deinit();
static void tick();
static int getValue(const int x,const int h);
static int getColor(const int x,const int h);
TSensor ram_sensor={0,0,0,0,init,deinit,tick,getValue,0,getColor,0};

static char* values;
static int color,maxRAM;

static void init(int rewidth){
  color = Color2ColorByRenderBit(&cfgFreeRAM[0]);
  ram_sensor.type=cfgRamType;
  ram_sensor.div=cfgRamDiv;
  if ((rewidth)||(values==0)){
    deinit();
    values = malloc(uiWidth);
    zeromem(values, uiWidth);
  }
}

static void deinit(){
  mfree(values);
  values=0;
}

static void tick(){
  int r = GetFreeRamAvail();
  if (r>maxRAM) maxRAM=r;
  int h=ram_sensor.hhh;
  values[h] = uiHeight * r / maxRAM;
  if(++h>=uiWidth) h=0;
  ram_sensor.hhh=h;
}

static int getValue(const int x,const int h){
  return values[h];
}

static int getColor(const int x,const int h){
  return color;
}
