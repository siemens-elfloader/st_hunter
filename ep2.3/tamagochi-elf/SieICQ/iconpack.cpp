#include "include.h"
#include "iconpack.h"
#include "file_works.h"

IconPack * IconPack::Active = NULL;

static const char * const img_names[TOTAL_ICONS_NUM]=
{
  "logo.png",
  "UNKNOWN.png",
  "OFFLINE.png",
  "ONLINE.png",
  "AWAY.png",
  "DND.png",
  "NA.png", 
  "OCCUPIED.png",
  "FFC.png",
  "INVISIBLE.png"
  
};

IconPack::IconPack()
{
  for(int i=0; i < TOTAL_ICONS_NUM; i++)
    data[i] = NULL;
  Setup();
}

IconPack::~IconPack()
{
  Free();
}

void IconPack::Setup()
{
  Free();
  
  char * icons_path = getSymbolicPath("$img\\");
  
  Active = this;
  for(int i=0; i<TOTAL_ICONS_NUM; i++)
  {
    char * img = new char[strlen(icons_path) + strlen(img_names[i])+1];
    strcpy(img, icons_path);
    strcat(img, img_names[i]);
    data[i] = (int)img;
  }
  delete icons_path;
}

void IconPack::Free()
{
  for(int i=0; i < TOTAL_ICONS_NUM; i++)
  {
    if(data[i])
      delete (void *)data[i];
    data[i] = NULL;
  }
}
