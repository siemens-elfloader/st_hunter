#ifndef _TAMAGOCHI_H_
  #define _TAMAGOCHI_H_

#define __VERSION__ "0.8"

#define IPC_TAMAGOCHI_NAME "Tamagochi"
#define IPC_CHECK_DOUBLERUN 1
#define IPC_UPDATE_STAT 8

extern const unsigned int REFRESH;
extern const int cfgShowIn;

extern const unsigned int POS_X;
extern const unsigned int POS_Y;
extern const int Effects_Ena;

extern const char PET_PATH[64];
extern const char PIC_PATH[64];
extern const char ROOM_PATH[64];
extern const char SOUND_PATH[64];
extern const char GAMELIST_PATH[64];


extern const int ACTIVE_KEY;
extern const int ACTIVE_KEY_STYLE;

extern const unsigned  int OP;
extern const unsigned  int MINSIZE;
extern const unsigned  int MAXSIZE;

extern const int VIBR_TYPE;


extern const int cfgLockOnly;
extern const unsigned int cfgPeriod;
extern const unsigned int cfgBright;
extern const unsigned int cfgMaxEv;
extern const int cfgKbd;
extern const int cfgDispl;
#ifndef NEWSGOLD
extern const int cfgDyn;
#else
extern const int cfgLighter;
#endif
#ifdef ELKA
extern const int cfgSLI;
#endif


static const char * const SoundName[18]=

{
  "global.wav", //tuk-tuk 0
  "hello.wav", //privet 1
  "birthday.wav", //den' rozhdeniya 2
  "alarm.wav", //toropis'! 3
  "comeon.wav", //nu davaj-davaj 4
  "illgetyou.wav", //nu pogodi 5
  "boring.wav", //skuchno 6
  "byebye.wav", //poka 7
  "fatality.wav", //ubit 8
  "firstblood.wav", //pervaya krov' 9
  "laugh.wav", //haha 10
  "leaveme.wav", //ostav' menya 11
  "nooo.wav", //neee 12
  "ohdeer.wav", //oj-e-e-e-ej 13
  "sleep.wav", //zevaet 14
  "ouch.wav", //oj bol'na 15
  "ow.wav", //au! 16
  "yessir.wav" //da ser 17
};



static const char * const icons_names[9]=
{
  "smile.png",
  "death.png",
  "dream.png",
  "hunger.png",
  "Health.png",
  "Happiness.png",
  "Dirtiness.png",
  "Boredom.png",
  "Behaviour.png"
};

static const char * const icons_status[4]=
{
  "null.png",
  "game.png",
  "moon.png",
  "music.png"
};

static const char * const icons_menu[15]=
{
  "game.png",
  "vaccinate.png",
  "discipline.png",
  "sleep.png",
  "status.png",
  "settings.png",
  "about.png",
  "sm_smile.png",
  "heart.png",
  "sm_hunger.png",
  "sm_Happiness.png",
  "sm_Dirtiness.png",
  "sm_Boredom.png",
  "sm_Behaviour.png",
  "sm_dream.png"  
};


typedef struct{
  int Age;          //возраст
  int TimeAge;      //минут после увеличения возраста
  int Health;       //здоровье
  int MaxHealth;    //максимальное здоровье
  int Hunger;       //голод
  int MaxHunger;    //максимальный голод
  int Happiness;    //счастье
  int MaxHappiness; //максимальное счастье
  int Dirtiness;    //грязь
  int MaxDirtiness; //максимальная грязь
  int Boredom;      //скука
  int MaxBoredom;   //максимальная скука
  int Behaviour;    //мораль
  int MaxBehaviour; //максимальная мораль
  int StatusDeath;  //статус смерти
  int ImageStatus;  //индекс статус-картинки на главный экран
} TStatusPet;


#endif
