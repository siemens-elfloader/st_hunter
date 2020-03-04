#include <alib/ui.h>

int KeyPressMode=0;
void OnKeyPress (int mess, int key, int IsLong, int DreamKey, void (*f)()){

  if (key==DreamKey){

    if (mess==KEY_DOWN) KeyPressMode=2;

    if (mess==LONG_PRESS){
      if (KeyPressMode==2){
        KeyPressMode=1;
        if (IsLong) f ();
      }
    }

    if (mess==KEY_UP){
      if (KeyPressMode==2){
        KeyPressMode=0;
        if (!IsLong) f ();
      }
    }

  }
}

void OnKeyPress2 (int mess, int key, int DreamKey, void (*f1)(), void (*f2)()){

  if (key==DreamKey){

    if (mess==KEY_DOWN) KeyPressMode=2;

    if (mess==LONG_PRESS){
      if (KeyPressMode==2){
        KeyPressMode=1;
        f2 ();
      }
    }

    if (mess==KEY_UP){
      if (KeyPressMode==2){
        KeyPressMode=0;
        f1 ();
      }
    }

  }
}
