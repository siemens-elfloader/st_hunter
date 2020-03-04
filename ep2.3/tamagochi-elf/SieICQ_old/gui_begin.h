#define _GUI_BEGIN_H_
 #ifdef _GUI_BEGIN_H_

// Запустить начальный гуй

void OnRedraw_Begin();
int MoveCursor_Begin(int pressed_mode, int key_kode);
void Init_Begin();

void SMART_REDRAW(void);

void BeginStep(int max);
void NextStep(char * msg);
void EndStep();

#endif
