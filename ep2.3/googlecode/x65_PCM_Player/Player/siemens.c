

const int minus11 = -11; // стремная константа =)
extern int onstart( char * exename, char * fname );
extern void onclose();
unsigned short maincsm_name_body[140];
extern unsigned short speed;
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;

char IsLocked = 0, LockCount;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}
MAIN_CSM;

typedef struct
{
  GUI gui;
  WSHDR * ws1;
  WSHDR * ws2;
  int i1;
}
MAIN_GUI;


void OnRedraw( MAIN_GUI * data ) // OnRedraw
{

  char temptime[5];

  if ( IsLocked )
  {
    DrawRoundedFrame( 0, 0, ScreenW() - 1, ScreenH() - 1, 0, 0, 0, GetPaletteAdrByColorIndex( 20 ),
         GetPaletteAdrByColorIndex( 20 ) );
    WSHDR * ws = AllocWS( 256 );

    str_2ws(ws,playlist[plcurrent].filename2,strlen(playlist[plcurrent].filename2));
    DrawString( ws, 1, 4, 131, 68, 3, 2, GetPaletteAdrByColorIndex( 0 ), GetPaletteAdrByColorIndex( 23 ) );

    unsigned char min = GetPlayingTime() / 60;
    unsigned char sec = GetPlayingTime() - min * 60;
    if ( min > 9 )
    {
      if ( sec > 9 )
      {
        sprintf( temptime, "%d:%d", min, sec );
      }
      else
      {
        sprintf( temptime, "%d:0%d", min, sec );
      }
    }
    else
    {
      if ( sec > 9 )
      {
        sprintf( temptime, "0%d:%d", min, sec );
      }
      else
      {
        sprintf( temptime, "0%d:0%d", min, sec );
      }
    }

    wsprintf( ws, temptime );
    DrawString( ws, 0, 70, 132, 900, 1, 2, GetPaletteAdrByColorIndex( 0 ), GetPaletteAdrByColorIndex( 23 ) );

    wsprintf( ws, "Keyboard locked." );
    DrawString( ws, 0, 100, 130, 120, 3, 2, GetPaletteAdrByColorIndex( 0 ), GetPaletteAdrByColorIndex( 23 ) );

    wsprintf( ws, "Hold '#' for unlock." );
    DrawString( ws, 0, 120, 130, 150, 3, 2, GetPaletteAdrByColorIndex( 0 ), GetPaletteAdrByColorIndex( 23 ) );

    FreeWS( ws );
    return;
  }

  if ( PlayListActive )
  {
    DrawPlayList();
    return;
  }

  if ( MenuActive )
  {
    DrawMenu();
    return;
  }

  DrawMainScreen();

  /* WSHDR * ws = AllocWS( 256 ); wsprintf( ws, "%d", GetPlayingTime()); c1.r = 0; c1.g = 0; c1.b = 0; c1.a = 255; c2.r = 0;
  c2.g = 0; c2.b = 255; c2.a = 128; DrawString( ws, 1, 1, 130, 130, 7, 2, ( char * ) & c1, ( char * ) & c2 ); FreeWS( ws ); */
}

void onCreate( MAIN_GUI * data, void * ( * malloc_adr ) ( int ) ) //Create
{
  // Тут можно создать переменные
  data->gui.state = 1;
}

void onClose( MAIN_GUI * data, void( * mfree_adr ) ( void * ) ) //Close
{
  // Тут можно освободить выделяемую память
  onclose();
  data->gui.state = 0;
}

void onFocus( MAIN_GUI * data, void * ( * malloc_adr ) ( int ), void( * mfree_adr ) ( void * ) ) //Focus
{
  DisableIDLETMR();
  data->gui.state = 2;
  CsmActive = 1;
}

void onUnfocus( MAIN_GUI * data, void( * mfree_adr ) ( void * ) ) //Unfocus
{
  CsmActive = 0;
  if ( data->gui.state != 2 ) return;
  data->gui.state = 1;
}


/* Обработчик нажатий клавиш. Сюда передаются нажатия клавиш в виде сообщения GUI_MSG, пример декодирования ниже. */
int OnKey( MAIN_GUI * data, GUI_MSG * msg ) //OnKey
{
  if ( ( msg->gbsmsg->msg == LONG_PRESS ) & ( msg->gbsmsg->submess == '#' ) )
  {
    LockCount++;
  }

  if ( LockCount > 1 )
  {
    IsLocked = IsLocked ^ 1;
    LockCount = 0;
    REDRAW();
  }

  if ( IsLocked ) return ( 0 );

  if ( ( msg->gbsmsg->msg == KEY_DOWN ) || ( msg->gbsmsg->msg == LONG_PRESS ) )
  {
    switch ( msg->gbsmsg->submess )
    {
      case VOL_UP_BUTTON:
        SetVolume( Volume + 1 );
      break;
      case VOL_DOWN_BUTTON:
        SetVolume( Volume - 1 );
      break;
    }

    if ( PlayListActive )
    {
      PLOnKey( msg->gbsmsg->submess );
      REDRAW();
      return ( 0 );
    }

    if ( MenuActive )
    {
      return ( MenuOnKey( msg->gbsmsg->submess ) );

    }

    return ( MainScreenOnKey( msg->gbsmsg->submess ) );
  }

  return ( 0 );
}

#ifdef NEWSGOLD
void onDestroy( MAIN_GUI * data, void( * mfree_adr ) ( void * ) )
{
  // Это оставить как есть ;)
  mfree_adr( data );
}

#else
extern void kill_data( void * p, void( * func_p ) ( void * ) );

void method7( MAIN_GUI * data, void( * mfree_adr ) ( void * ) )
{
  kill_data( data, mfree_adr );
  mfree_adr( data );
}

#endif

// Оставить как есть
int method8( void )
{
  return ( 0 );
}

// Оставить как есть
int method9( void )
{
  return ( 0 );
}

// Массив с методами, чтобы дать ОС информацию, какие когда вызывать
const void * const gui_methods[11] =
{
  ( void * ) OnRedraw, //Redraw
  ( void * ) onCreate, //Create
  ( void * ) onClose, //Close
  ( void * ) onFocus, //Focus
  ( void * ) onUnfocus, //Unfocus
  ( void * ) OnKey, //OnKey
  0,
#ifdef NEWSGOLD
  ( void * ) onDestroy, //Destroy
#else
  ( void * ) kill_data, //Destroy
#endif
  ( void * ) method8, ( void * ) method9, 0
};

// Канвас для основного GUI
const RECT Canvas =
{
  0, 0, 129, 129
};

// Вызывается при создании главного CSM. В данном примере
// создаётся GUI, его ID записывается в MAINGUI_ID
// на всякий случай - вдруг понадобится ;)
void maincsm_oncreate( CSM_RAM * data )
{
  MAIN_GUI * main_gui = malloc( sizeof( MAIN_GUI ) );
  MAIN_CSM * csm = ( MAIN_CSM * ) data;
  zeromem( main_gui, sizeof( MAIN_GUI ) );
  main_gui->gui.canvas = ( void * ) ( & Canvas );
  main_gui->gui.flag30 = 2;
  main_gui->gui.methods = ( void * ) gui_methods;
  main_gui->gui.item_ll.data_mfree = ( void( * ) ( void * ) ) mfree_adr();
  csm->csm.state = 0;
  csm->csm.unk1 = 0;
  csm->gui_id = CreateGUI( main_gui );
  MAINGUI_ID = csm->gui_id;
}

// Всё ясно из названия ;) оставить как есть
void ElfKiller( void )
{
  extern void kill_data( void * p, void( * func_p ) ( void * ) );
  extern void * ELF_BEGIN;
  kill_data( & ELF_BEGIN, ( void( * ) ( void * ) ) mfree_adr() );
}

// Вызывается при закрытии главного CSM. Тут и вызывается киллер
void maincsm_onclose( CSM_RAM * csm )
{
  SUBPROC( ( void * ) ElfKiller );
}

// Обработчик событий главного CSM
int maincsm_onmessage( CSM_RAM * data, GBS_MSG * msg )
{
  MAIN_CSM * csm = ( MAIN_CSM * ) data;
  if ( ( msg->msg == MSG_GUI_DESTROYED ) && ( ( int )msg->data0 == csm->gui_id ) )
  {
    csm->csm.state = -3;
  }
  return ( 1 );
}

// Инициализация структуры MAINCSM
const struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
}
MAINCSM =
{
  {
    maincsm_onmessage, maincsm_oncreate,
#ifdef NEWSGOLD
    0, 0, 0, 0,
#endif
    maincsm_onclose, sizeof( MAIN_CSM ), 1, & minus11
  },
  {
    maincsm_name_body, NAMECSM_MAGIC1, NAMECSM_MAGIC2, 0x0, 139
  }
};

// Функция, которая устанавливает название этого CSM для X-Task.
void UpdateCSMname( void )
{
  WSHDR * ws = AllocWS( 256 );
  wsprintf( ( WSHDR * ) ( & MAINCSM.maincsm_name ), "MediaPlayer 1.0" );
  FreeWS( ws );
}


// Основная процедура. Она первой получает управление при старте эльфа.
int main( char * exename, char * fname )
{
  char dummy[sizeof( MAIN_CSM )];
  MAINCSM_ID = CreateCSM( & MAINCSM.maincsm, dummy, 0 );
  onstart( exename, fname );
  UpdateCSMname();
  return 0;
}
