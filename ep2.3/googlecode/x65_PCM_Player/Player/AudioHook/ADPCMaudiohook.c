/*    Интерфейс для аудио хука.
Немного о принципе работы:
Функцией PlayMelodyInMem запускается проигрывание буффера
с заголовком wave файла. PlayMelodyInMem создает свой
буффер, копирует туда данные без заголовка wave и уже
оттуда посылает
данные в DSP. Условно этот буффер разделяется на два.
Мы находим адрес этого буффера и установив хук на
зацикливание заполняем одну его половину, пока другая
воспроизводится. Все просто :)
Из за каких-то особенностей воспроизведения
буффер меньше ~2 секунд приводит к сбоям в проигрывании.
ILYA_ZX.
mailto: rusman2005@mail.ru
*/

#include "IMA_ADPCM.C"

extern signed char Volume;
/* Частота дискретизации */
extern int samplerate;
/* Адрес в ОЗУ(из библиотеки функций) */
long * WaveParameters;
/* Буффер, создаваемый функцией PlayMelodyInMem */
char * buffer;
/* Номер следующего буффера для заполнения. Один буффер условно делится на два */
extern unsigned char nextbuff;
/* Буффер, используемый для запуска проигрывания */
unsigned char * imabuf=0;
/* Буффер для PCM данных */
signed short * pcmbuff=0;
/* Переменные управления воспроизведением. Для остановки: Playing=0; */
extern signed char Playing;
/* Время проигрывания в 1/16000 секунды */
extern signed long PlayingTime;
/* Процедура управления проигрыванием */
void AdpcmPlayingProc();
/* Размер буффера. Должен быть кратен 256(размер блока IMA ADPCM) */
static const int AD_BUFFERSIZE = 32768;

int writebufpos;


/* Заголовок wave IMA ADPCM */
unsigned char Ad_WavHdr[60] =
{
  0x52, 0x49, 0x46, 0x46, 0x34, 0x4D, 0x4B, 0x00, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20, 0x14, 0x00, 0x00, 0x00, 0x11,
       0x00, 0x01, 0x00, 0x80, 0x3E, 0x00, 0x00, 0xAE, 0x1F, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x02, 0x00, 0xF9, 0x01, 0x66,
       0x61, 0x63, 0x74, 0x04, 0x00, 0x00, 0x00, 0x00, 0x89, 0x94, 0x00, 0x64, 0x61, 0x74, 0x61, 0x00, 0x4D, 0x4B, 0x00
};


/* Функция включения и отключения зацикливания буффера, не 0 - зацикливание включено */
void Ad_SetHookState( unsigned char hookstate )
{
  WaveParameters[-0x1C] = hookstate;
}

/* Возвращает адрес конца буффера, созданного функцией PlayMelodyInMem */
long GetBuffEnd()
{
  return ( WaveParameters[0x08] );
}

/* Возвращает адрес текущей позиции в буфере */
long GetAd_BuffPosAddr()
{
  return ( WaveParameters[0x07] );
}

/* Возвращает позицию в буффере */
unsigned long Ad_BuffPos()
{
  return ( GetAd_BuffPosAddr() - ( long )buffer );
}

/* Функция заполнения первого буффера в imabuf */
void Ad_FillBuffer()
{
  unsigned int i;

  writebufpos = 60;

  while ( writebufpos < ( 60+4096) )
  {
    i = 0;
    while ( i < 1012 )
    {
      pcmbuff[i] = 0;
      i++;
    }
    if ( Playing !=1 )
    {
      writebufpos=-1;
      return;
    }
    ImaCodeBlock( pcmbuff, & imabuf[writebufpos] );
    writebufpos = writebufpos + 256;
    PlayingTime += 506;
  }
  

  while ( writebufpos < ( AD_BUFFERSIZE/2 + 60 ) )
  {
    i = 0;
    GetSound(pcmbuff,1012);
    if ( Playing !=1 )
    {
      writebufpos=-1;
      return;
    }
    ImaCodeBlock( pcmbuff, & imabuf[writebufpos] );
    writebufpos = writebufpos + 256;
    PlayingTime += 506;
  }
  writebufpos=-1;
}

int Ad_PlayStatus(){
return(WaveParameters[0x16]);  
};
/* Функция заполнения первого буффера */
void Ad_FillBuffer1()
{
  unsigned int i; 
  int add=0;
  writebufpos = 0;
  while ( writebufpos < ( AD_BUFFERSIZE / 2 ) )
  {
    if ( Paused )
    {
      zeromem( pcmbuff, 1012 );
    }
    else
    {
      i = 0;
      GetSound(pcmbuff,1012);
      PlayingTime += 506;
    }
    if ( Playing !=1 )
    {
      writebufpos=-1;
      return;
    }
    ImaCodeBlock( pcmbuff, & buffer[writebufpos] );
    writebufpos = writebufpos + 256;
  }
  writebufpos=-1;
}

/* Функция заполнения второго буффера */
void Ad_FillBuffer2()
{
  unsigned int i, writebufpos;
  writebufpos = AD_BUFFERSIZE / 2;
  while ( writebufpos < AD_BUFFERSIZE )
  {
    if ( Paused )
    {
      zeromem( pcmbuff, 1012 );
    }
    else
    {
      GetSound(pcmbuff,1012);
      PlayingTime += 506;
    }
    if ( Playing !=1 )
    {
      writebufpos=-1;
      return;
    }
    ImaCodeBlock( pcmbuff, & buffer[writebufpos] );
    writebufpos = writebufpos + 256;
  }
  if(!AudioActive){
    Playing=0;
    PlayerCmd=CMD_OPEN | CMD_NEXT | CMD_STOP | CMD_PLAY;
  }
}

void Ad_AudioInit()
{
  Volume = 1;
  /* Адрес параметров wave из библиотеки функций,
  другого способа его получить я не нашел */
  //WaveParameters = ( void * ) 0xA0FD426C;  
 // WaveParameters = ( void * ) WaveParameters[0]; 
  WaveParameters= AudioParamsAdr();
  /* +60 - на заголовок wave */
  imabuf = malloc( AD_BUFFERSIZE + 60 );
  pcmbuff = malloc( 1014*2 );
  /*Копируем заголовок wave */
  memcpy( imabuf, ( void * ) ( Ad_WavHdr ), 60 );
}

void Ad_AudioStart()
{ 
  /* Воспроизведение активно */
  Playing = 1;
  /*0xFF - для установки громкости и получения адреса буффера*/
  nextbuff = 0xFF;
  /*Подготавливаем буффер*/
  Ad_FillBuffer();
  /*Устанавливаем зацикливание*/
  Ad_SetHookState( 1 );
  
  /* Устанавливаем частоту дискретизации */
  memcpy(imabuf+24,&samplerate,4);
  *(( (char *) PcmWaveParams)+0x85) = ( 48 - ( Volume * 2 ) ) + 1;
  /*Запускаем буффер на проигрывание*/
  PlayMelodyInMem( 0x11, imabuf, AD_BUFFERSIZE + 60, 0xFFFF, 0, 0 );
  /*Запускаем цикл обслуживания проигрывания*/  
  GBS_StartTimerProc( & ahtimer, 64, AdpcmPlayingProc );
}

/*Остановка воспроизведения.
Фенкция вызывается если Playing=0 */
void Ad_AudioStop()
{
  /* Отключаем хук */
  Ad_SetHookState( 0 );
  AudioActive=0;
  /* Проигрываем короткий звук для остановки проигрывания основного буффера */
  //PlayMelodyInMem( 0x11, imabuf, 256 + 60, 0xFFFF, 0, 0 );
}


void Ad_AudioTerminate()
{
  Playing=0;
  Ad_AudioStop();
  if(imabuf!=0){
  mfree( imabuf );
  imabuf=0;
  }
  if(pcmbuff!=0){
  mfree( pcmbuff );
  pcmbuff=0;
  }
}

void AdpcmPlayingProc()
{
  int BUFPOS;
  
  if(writebufpos==-1)
  {
  switch (Playing)
  {
  case 0:
    Ad_AudioStop();    
    return;
  case -1:
    Ad_AudioTerminate();
    return;
  }

  }
  
  if ( nextbuff == 0xFF )
  {
    nextbuff = 2;
    buffer = ( void * ) ( GetBuffEnd() - AD_BUFFERSIZE );
  //  ChangeVolume( Volume );
  }

  BUFPOS = Ad_BuffPos();

  if ( ( nextbuff == 1 ) && ( BUFPOS > ( AD_BUFFERSIZE / 2 ) ) )
  {
    /* Сбрасываем какой-то счетчик. Если этого не делать
    то примерно через 10 минут проигрывания телефон зависает */
    WaveParameters[0x0B] = 0;
    /* Заполняем первый буффер */
    Ad_FillBuffer1();
    //SUBPROC((void*)Ad_FillBuffer1);
    nextbuff = 2;
  }

  if ( ( nextbuff == 2 ) && ( BUFPOS < ( AD_BUFFERSIZE / 2 ) ) )
  {
    Ad_FillBuffer2();
    //SUBPROC((void*)Ad_FillBuffer2);
    nextbuff = 1;  
  }

    GBS_StartTimerProc( & ahtimer, 8, AdpcmPlayingProc );
}
