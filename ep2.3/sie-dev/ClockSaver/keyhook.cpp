#include "..\inc\swilib.h"
#include "cfg_param.h"
#include "csgui.h"

extern int isGUI;

int GetPresTypeCode()	//ïîëó÷èì òèï íàæàòèÿ
{
	extern const unsigned int BCFG_CALL_TYPE;
	if (BCFG_CALL_TYPE)
	{
		return LONG_PRESS;
	}
	else
	{
		return KEY_DOWN;
	}
}

extern csGUI* gui;
void timer_proc ();

void OpenClockWindow()
{
	extern GBSTMR timer;								//òàéìåð
	gui = new csGUI();									//Ñäåëàëè êëàññ ãóÿ
	gui->Create();										//Òåïåðü ñîçäàëè ãóé
	gui->redraw();										//Ïåðåðèñóåì íà âñÿêèé
	SetIllumination(0, 1, light, 0);					//âêëþ÷èëè ïðîæåêòîðà
	GBS_StartTimerProc(&timer, TMR_SECOND, timer_proc);	//çàâåëè òàéìåð
}

int hook_proc(int key, int msg)	//êåéõóê
{

	extern const unsigned int BCFG_CALL_BTN;	//êíîïêà âûçîâà

	if (!isGUI && (IsUnlocked() == BCFG_KBD_STATE || BCFG_KBD_STATE == 2) && msg == GetPresTypeCode() && IsScreenSaver())
		if (key == BCFG_CALL_BTN)
		{
				switch(msg)
				{
				case KEY_DOWN:
					if (GetPresTypeCode() == KEY_DOWN)
				  {
				    OpenClockWindow();
					  return KEYHOOK_BREAK;
					}
          else
					  return KEYHOOK_BREAK;
				case LONG_PRESS:
          if (GetPresTypeCode() == LONG_PRESS)
					{
            OpenClockWindow(); //äåëàåì, òî ÷òî íàì íàäî
					  return KEYHOOK_BREAK;
          }
          else
            return KEYHOOK_BREAK;
				}
			}
		}

	return 0;
}

void SetHook()
{
	AddKeybMsgHook((void*)hook_proc);
}

void DestroyHook()
{
	RemoveKeybMsgHook((void*)hook_proc);
}


