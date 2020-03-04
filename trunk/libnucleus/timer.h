

#ifndef __TIMER_H__
#define __TIMER_H__

//#warning This addreses has been found for S75sw52 by Z.Vova. You must change addreses for you phone
#ifdef S75V52

#define Create_Timer        0xA00A4274
#define Delete_Timer        0xA00A42E8
#define Reset_Timer         0xA00A4308
#define Control_Timer       0xA00A436C
#define Established_Timers  0xFFFFFFFF
#define Set_Clock           0xA00A4D0C // Возможно это не она, я не уверен

#endif
#ifdef E71V45

#define Create_Timer		0xA009E868
#define Delete_Timer		0xA009E8DC
#define Reset_Timer			0xA009E8FC
#define Control_Timer		0xA009E960
#define Established_Timers	0xFFFFFFFF
//#define Set_Clock

#endif

#endif
