#include "../inc/swilib.h"
#include "gui.h"

inline int IsIdleOnTop()
{
	return IsGuiOnTop(((int *)FindCSMbyID(CSM_root()->idle_id))[DISPLACE_OF_IDLEGUI_ID/4]);
}

int KeyHook(int key, int style)
{
    extern const int cfg_key_activation1, cfg_key_activation2;
	extern int gui_csm_id;
	if(IsIdleOnTop() && IsUnlocked() && GetPlayStatus()==0)
	{
		if(style==KEY_DOWN)
		{
			if(key==cfg_key_activation1 || key==cfg_key_activation2)
			{
				CreateOpenGUI();
				return KEYHOOK_BREAK;
			}
		}
	}
	return KEYHOOK_NEXT;
}

//#pragma inline=forced
void AddKeyHook(void)
{
	AddKeybMsgHook ((void*)KeyHook);
}

//#pragma inline=forced
void RemoveKeyHook(void)
{
	RemoveKeybMsgHook((void*)KeyHook);
}
