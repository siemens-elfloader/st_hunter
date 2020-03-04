//#define NEWSGOLD

#include "swilib.h"
#include "cfg_items.h"
#include "conf_loader.h"

extern unsigned long  strtoul (const char *nptr,char **endptr,int base);

//это все идут настройки читаемые из конфига
extern const unsigned int cfgShowIn;//когда показывать
extern const unsigned int cfgX;//планировал сделать но потом понял что ненадо
extern const unsigned int cfgY;//положение по Y
extern const char spic1[128];//1я маленькая иконка(путь)
extern const char bpic1[128];//1я большая иконка
extern const char spic2[128];
extern const char bpic2[128];
extern const char spic3[128];
extern const char bpic3[128];
extern const char spic4[128];
extern const char bpic4[128];//предыдущее соответсвенно для остальных иконок
extern const int type1;//тип 1й иконки (файл/шорткат/адрес ф-ции)
extern const int type2;
extern const int type3;
extern const int type4;
extern const char file1[128];//файл/шорткат/адрес ф-ции 1 иконки в виде строки
extern const char file2[128];//тоже но для других иконок
extern const char file3[128];
extern const char file4[128];

CSM_DESC icsmd;
void (*old_icsm_onClose)(CSM_RAM*);
int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
CSM_RAM *idlegui;

int IndexX = -1; //индекс выделеной иконки, -1 - нет выделения
int IconWidth = 0, SplitterWidth = 0, IconDelta = 0;
//длинна иконки, расстояние между иконками, разница между большой и маленькой иконкой

const char *Imgs(int index)
//ф-ция для облегчения выбора рисуемой иконки, тупо берем по индексу
//и возвращаем путь к ней
{
  switch(index){
  case 0: return spic1;
  case 1: return spic2;
  case 2: return spic3;
  case 3: return spic4;
  case 4: return bpic1;
  case 5: return bpic2;
  case 6: return bpic3;
  case 7: return bpic4;
  }
  return 0;
}

void RunIcon(int Index)
{
//запуск действия по индексу иконки
  int Type = 0;//(файл/шорткат/адрес ф-ции)
  char Filename[128];
  
//подготавливаемся к запуску
//узнаем тип и путь
  switch(Index){
  case 0:
    Type = type1;
    strcpy(Filename, file1);
    break;
  case 1:
    Type = type2;
    strcpy(Filename, file2);
    break;
  case 2:
    Type = type3;
    strcpy(Filename, file3);
    break;
  case 3:
    Type = type4;
    strcpy(Filename, file4);
    break;
  }
  

//в зависимости от типа запускаем файл/шорткат/адрес ф-ции  
  switch(Type){
  case 0:
    {
      WSHDR *elfname=AllocWS(256);
      wsprintf(elfname,Filename);
      ExecuteFile(elfname,NULL,NULL);
      FreeWS(elfname);
      break;
    }
  case 1:
    {
      unsigned int* addr = (unsigned int*)GetFunctionPointer(Filename);
            if (addr)
            {
              SUBPROC((void*)addr);
            }
      break;
    }
  case 2:
    {
      typedef void (*voidfunc)(); 
      unsigned int addr=strtoul(Filename, 0, 16 );
      voidfunc pp=(voidfunc)addr;
      SUBPROC((void*)pp);
      break;
    }
  }
}

void Delay(unsigned int msec)
{
//хотел сделать задержку, но стало лень
}

int IsIdleGui()//проверяем находимся мы на айдле или нет
{
  void *icsm=FindCSMbyID(CSM_root()->idle_id);
  if(IsGuiOnTop(((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4]))
    return 1;
  else
    return 0;
}

void DrawMainMenu()
//ф-ция рисования меню
{
	//рисуемся в зависимости от настроек
  switch(cfgShowIn){
  case 0:
    if(IsUnlocked())
      return;
    break;
  case 1:
    if(!IsUnlocked())
      return;
    break;
  }
  //если рисуемся, то смотрим на айдле ли мы
  if(IsIdleGui())
 {
  IconWidth = GetImgWidth((int)Imgs(0));//берем длинну малой иконки
  SplitterWidth = (ScreenW() - IconWidth * 4) / 5;//вычисляем длинну разделителя
  IconDelta = GetImgWidth((int)Imgs(4)) - GetImgWidth((int)Imgs(0));
  // -/-/- разность между большой и малой иконками
  for(int i = 0; i < 4; i++)//цикл рисования иконок, кто не знает то (x >> 1) = (x / 2)
  {
    if(IndexX != i)
    {
      DrawImg(SplitterWidth * (i + 1) + (IconWidth * i), cfgY, (int)Imgs(i));
    }
    else
    {
      DrawImg(SplitterWidth * (i + 1) + (IconWidth * i) - (IconDelta >> 1),
             cfgY - (IconDelta >> 1), (int)Imgs(i + 4));
    }
  }
 }
}

int my_keyhook(int submsg, int msg)//кейхук
{
  if(!IsIdleGui()||!IsUnlocked())
    return(0);//работаем только на айлде и при разблокированой клаве
 if(msg == LONG_PRESS)
   return(0);//не обрабатываем долгое нажатие клавиш
 
if(msg == KEY_UP)
// при отпускании клавиши запускаем действие
{
   switch(submsg){
      case UP_BUTTON:
        RunIcon(IndexX);
        break;
      case DOWN_BUTTON:
        RunIcon(IndexX);
        break;
      case LEFT_BUTTON:
        RunIcon(IndexX);
        break;
      case RIGHT_BUTTON:
        RunIcon(IndexX);
        break;
      }
   if(IndexX != -1)
   {
        IndexX = -1;//раз отпустили, то никакая иконка не выделена
        DirectRedrawGUI();//перерисовываемся
   }
   return(0);
}

if(msg == KEY_DOWN)
{
  if(IndexX != -1)
    DirectRedrawGUI();
    //если уже есть выделеная иконка, то перерисовываеся
  switch(submsg)//выбираем новую выделенную иконку
  {
  case UP_BUTTON:
    IndexX = 0;
    break;
  case DOWN_BUTTON:
    IndexX = 3;
    break;
  case LEFT_BUTTON:
    IndexX = 1;
    break;
  case RIGHT_BUTTON:
    IndexX = 2;
    break;
  default:
    return(0);
  }
  DrawMainMenu();// рисуем меню
}

return(2); //если мы что-то обработали, то не пускам событие дальше, поэтому и возвращаем 2
}

int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

#pragma inline
int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}

int MyIDLECSM_onMessage(CSM_RAM* data, GBS_MSG* msg)
{
  if(msg->msg == MSG_RECONFIGURE_REQ) //если настройки обновились
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
         ShowMSG(1,(int)"Настройки 4MotorIcons обновлены!");
         InitConfig();//читаем настройки заново
    }
  }

 int csm_result = old_icsm_onMessage(data, msg);//передаем сообщение старому обработчику
 DrawMainMenu();//рисуем меню
 return(csm_result);
}

 void MyIDLECSM_onClose(CSM_RAM *data)
{
  RemoveKeybMsgHook((void *)my_keyhook);//удаяем наш кейхук
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
  seqkill(data,old_icsm_onClose,&ELF_BEGIN,SEQKILLER_ADR());
}


int main(void)
{
 InitConfig();//загружаем настройки
 LockSched();
 CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
 idlegui = icsm;
 memcpy(&icsmd,icsm->constr,sizeof(icsmd));
 old_icsm_onMessage=icsmd.onMessage;
 icsmd.onMessage=MyIDLECSM_onMessage;
 old_icsm_onClose=icsmd.onClose;
 icsmd.onClose=MyIDLECSM_onClose;
 icsm->constr=&icsmd;
 AddKeybMsgHook((void *)my_keyhook);//прилепились к айдлу и ставим кейхук
 UnlockSched();
 return 0;
}
