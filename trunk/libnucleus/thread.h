
#ifndef __THREAD_H__
#define __THREAD_H__

//#warning This addreses has been found for S75sw52 by Z.Vova. You must change addreses for you phone
#ifdef S75V52

#define Create_Task          0xA00A2420
#define Delete_Task          0xA00A253C
#define Reset_Task           0xA00A2598
#define Terminate_Task       0xA00A26B4
#define Resume_Task          0xA00A26D8
#define Suspend_Task         0xA00A26FC
#define Relinquish           0xA00A2738
#define Task_Sleep           0xA00A275C
#define Change_Priority      0xA00A3AA4
#define Change_Preemption    0xA00A3ABC
#define Change_Time_Slice    0xA00A3AD8
#define Check_Stack          0xA00A3AF0
#define Current_Task_Pointer 0xA00A2030
#define Established_Tasks    0xA00A2CD0
#define Task_Pointers        0xA00A2CE4
#define Task_Information     0xA00A6900

#endif
#ifdef E71V45

#define Create_Task			 0xA009CA14
#define Delete_Task			 0xA009CB30
#define Reset_Task			 0xA009CB8C
#define Terminate_Task		 0xA009CBAC
#define Resume_Task			 0xA009CCA8
#define Suspend_Task		 0xA009CCF0
#define Task_Sleep			 0xA009CE44
#define Relinquish			 0xA009CEC4
#define Change_Priority		 0xA009E098
#define Change_Preemption	 0xA009E0B0
#define Change_Time_Slice	 0xA009E0CC
#define Check_Stack			 0xA00A2B18
#define Current_Task_Pointer 0xA009C624
#define Established_Tasks	 0xA00A0B54
#define Task_Pointers		 0xA00A0B7C
#define Task_Information	 0xA00A0CC4

#endif


#endif
