#include "addr.h"

	EXTERN	AppendInfoW
	EXTERN	UpdateLocaleToItem
	EXTERN	AddNewLine	
	
	EXTERN	GetProvAndCity         //2.1.1
	EXTERN	GetNumFromIncomingPDU  //2.1.1
	EXTERN	setMenuText            //2.1.1

	RSEG	CODE
	
	DATA
MENU_HDR_ICON
	DCD	0x12E
	DCD	0
		
	CODE16
RecoedWindow_:
	PUSH	{LR}
	LDRB	R0, [R5, #9]
	CMP	R0, #7
	BCS	EX_PBACK
	LDR	R1, [R5, #4] //ºÅÂë WS ,ÅÐ¶ÏÒþ²ØºÅÂë
	LDR	R0, [R1, #0] //wsbody
	LDRH	R0, [R0, #0] //wslen
	CMP	R0, #0
	BEQ	EX_PBACK
	MOV	R0, R4
	BL	AddNewLine
	MOV	R0, R4
	LDR	R1, [R5, #4]
	BL	AppendInfoW
EX_PBACK
	MOV	R0, R4
	BL	AddNewLine
	MOV	R2, #1
	POP	{R0}
	ADD	R0, #4
	BX	R0

Callinwindow_:
	PUSH	{LR}
	LDR	R0, =ADDR_IsCalling
	BLX	R0
	MOV	R1, R0
	MOV	R0, #1
	MOV	R2, R4
	BL	UpdateLocaleToItem
	LDR	R0, =ADDR_CallIN
	BLX	R0
	CMP	R0, #0	
	POP	{R0}
	BEQ	HOOK1_EQ
	ADD	R0, #4
	BX	R0
HOOK1_EQ
	ADD	R0, #0xC
	BX	R0

CallOutWindow_:
	BEQ	SHOW_EQ
	MOV	R1, SP
	PUSH	{LR}
	LDR	R2, =ADDR_CallOUT2
	BLX	R2
	POP	{R2}
	ADD	R2, #4
	BX	R2
SHOW_EQ
	PUSH	{LR}
	LDR	R0, =ADDR_IsCalling
	BLX	R0
	MOV	R1, R0
	MOV	R0, #4
	MOV	R2, R4
	BL	UpdateLocaleToItem
	POP	{R0}
	ADD	R0, #0x14
	BX	R0

AddrBookWindow_:
	MOV	R4, SP
	PUSH	{LR}
	LDR	R2, =ADDR_AllocWS
	BLX	R2
	MOV	R1, R4
	MOV	R4, R0
	LDR	R2, =ADDR_ADDRBook
	BLX	R2
	MOV	R0, R4
	BL	AddNewLine
	MOV	R0, R4
	MOV	R1, R4
	BL	AppendInfoW
	POP	{R0}
	ADD	R0, #4
	BX	R0
	

SIMBOOK:
	PUSH	{LR}
	ADD	R1, R4, #0
	ADD	R0, R6, #0
	LDR	R3, =ADDR_GetPhoneBookNum
	BLX	R3
	MOV	R0,R4
	BL	AddNewLine
	MOV	R0,R4
	MOV	R1,R4
	BL	AppendInfoW
	POP	{R3}
	ADD	R3, #4
	BX	R3

	RSEG	CODE_X
	EXTERN	GetProvAndCity
	EXTERN	GetNumFromIncomingPDU
	CODE16
SMS_IN
	LDR	R0, [SP, #8]
	LDR	R2, [SP, #4]
	PUSH	{R0-R7, LR}
	MOV	R7, R0
	SUB	SP, #0x20
	LDR	R6, =0xAC5 //LGP ÐÂÐÅÏ¢
	CMP	R6, R2
	BNE	GoBack
	ADD	R0, SP, #0
	BL	GetNumFromIncomingPDU
	CMP	R0, #0
	BEQ	GoBack
	MOV	R0, R7
	BL	AddNewLine
	LDR	R0, [R7, #0]
	ADD	R1, SP, #0
	BL	GetProvAndCity
GoBack
	ADD	SP, #0x20
	POP	{R0-R7}
	POP	{R2}
	LDR	R0, [R0, #0] //WSTRLEN
	LDRH	R0, [R0, #0]
	STR	R0, [SP,#0xD8]
	ADD	R2, R2, #4
	BX	R2

	EXTERN	setMenuText
NUM_SELECT_MENU_PSETTEXT: //R2, WS, 
	LDR	R0, [SP, #0x208] //GUI
	LDR	R3, [SP, #0x20C] //item_n
	LDR	R1, [SP, #0] //item
	MOV	R4, LR
	BL	setMenuText
	ADD	R4, #4
	BX	R4
/*
	EXTERN	AddDialEQ
	EXTERN	DialGHookRedraw
#ifdef  S68Cv51
	CODE16
EDIAL_CREATE
	//ADD	LR, LR, #4
	//STMFD	SP!,{LR}
	//LDR	R0, [SP, #0x64] //EDITQ
	//LDR	R1, [SP, #0x34] //ec
	//LDR	R2, [SP, #0x20] //ecop
	//MOV	R3, R7
	LDR	R0, [SP, #0x60] //EDITQ
	LDR	R1, [SP, #0x30] //ec
	LDR	R2, [SP, #0x1C] //ecop
	BL	AddDialEQ
	MOV	R3, R7 //gui
	MOV	R2, #1
	MOV	R0, R4
	//LDR	R1, [SP, #0x64] //editq
	//LDMFD	SP!,{PC}
	LDR	R1, [SP, #0x60] //editq
	LDR	R7, =0xA08CA5AC+1
	BLX	R7
	ADD	SP, #0x6C
	POP	{R4-R7,PC}
#else
	CODE32
EDIAL_CREATE
	ADD	LR, LR, #4
	STMFD	SP!,{LR}
	LDR	R0, [SP, #0x64] //EDITQ
	LDR	R1, [SP, #0x34] //ec
	LDR	R2, [SP, #0x20] //ecop
	MOV	R3, R7
	BLX	AddDialEQ
	MOV	R3, R7 //gui
	MOV	R2, #1
	MOV	R0, R4
	LDR	R1, [SP, #0x64] //editq
	LDMFD	SP!,{PC}
#endif

	CODE32
EDIAL_REDRAW
	ADD	LR, LR, #4
	STMFD	SP!,{LR}
	MOV	R0, R4
	BLX	DialGHookRedraw
	MOV	R2, #1
	MOV	R1, #1
	MOV	R0, R4
	BLX	ADDR_EDIT_UNK_VOID_INT_INT
	LDMFD	SP!,{PC}
	
	EXTERN	IsNum
	EXTERN	AddSpace
	CODE32
ADRBKLST_CATWS
	MOV	R4, LR
	LDR	R0, [R5, #0]
	MOV	R1, #1
	BLX	IsNum
	CMP	R0, #0
	MOVEQ	R0, R5
	BXEQ	R4
	MOV	R0, R5
	BLX	AddNewLine
	MOV	R0, R5
	MOV	R1, #4
	BLX	AddSpace
	LDR	R0, [R5, #0]
	ADD	R1, SP, #0x1C
	BLX	GetProvAndCity
	MOV	R0, R5
	BX	R4
	
	CODE32
ADRBKLST_SIMNUMWS
	STMFD	SP!,{LR}
	LDR	R0, [R8, #0]
	MOV	R1, #0
	BLX	IsNum
	CMP	R0, #0
	MOVEQ	R1, R10
	LDMEQFD	SP!,{PC}
	MOV	R0, R10
	BLX	AddNewLine
	MOV	R0, R10
	MOV	R1, R8
	BLX	AppendInfoW
	MOV	R1, R10
	LDMFD	SP!,{PC}
	
	CODE32
ADRBKLST_CATWS_2
	STMFD	SP!,{LR}
	LDR	R0, [R4, #0]
	MOV	R1, #1
	BLX	IsNum
	CMP	R0, #0
	MOVEQ	R0, R4
	LDMEQFD	SP!,{PC}
	MOV	R0, R4
	BLX	AddNewLine
	MOV	R0, R4
	MOV	R1, #4
	BLX	AddSpace
	LDR	R0, [R4, #0]
	ADD	R1, SP, #4
	BLX	GetProvAndCity
	MOV	R0, R4
	LDMFD	SP!,{PC}
	
	CODE32
ADRBKLST_CATWS_3
	MOV	R4, LR
	LDR	R0, [SP,#0x220]
	LDR	R0, [R0, #0]
	MOV	R1, #1
	BLX	IsNum
	CMP	R0, #0
	LDREQ	R0, [SP,#0x220]
	BXEQ	R4
	LDR	R0, [SP,#0x220]
	BLX	AddNewLine
	LDR	R0, [SP,#0x220]
	MOV	R1, #4
	BLX	AddSpace
	LDR	R0, [SP,#0x220]
	LDR	R0, [R0, #0]
	ADD	R1, SP, #0x1C
	BLX	GetProvAndCity
	LDR	R0, [SP,#0x220]
	BX	R4
*/

	RSEG	RecordWindow:CODE(2)
	CODE16
	LDR	R0, =RecoedWindow_
	BLX	R0
	
	RSEG	Callinwindow:CODE(2)
	CODE16
	LDR	R0, =Callinwindow_
	BLX	R0
	
	
	RSEG	CallOutWindow:CODE(2)
	CODE16
	LDR	R1, =CallOutWindow_
	BLX	R1

	RSEG	AddrBookWindow3:CODE(1)
	CODE16
	NOP
	
	RSEG	AddrBookWindow:CODE(2)
	CODE16
	LDR	R1, AddrBookWindow_P
	BLX	R1
	DATA
AddrBookWindow_P
	DCD	AddrBookWindow_
	CODE16
	NOP
	
	CODE16
	RSEG	PHONEBOOKHOOK:CODE(2)
	LDR	R1, =SIMBOOK
	BLX	R1

	
	RSEG	SMS_IN_HOOK
	CODE16
	LDR	R0, =SMS_IN
	BLX	R0
	
	RSEG	NUM_SELECT_MENU_HDR_ICON:DATA(2)
	DATA
	DCD	MENU_HDR_ICON
	
	RSEG	NUM_SELECT_MENU_LINES:DATA(2)
	DATA
	DCD	1
	
	RSEG	NUM_SELECT_MENU_ALITEM:CODE(2)
	CODE16
	BLX	AllocMLMenuItem_adr //void *AllocMLMenuItem(void *data)
	
	RSEG	NUM_SELECT_MENU_TEXT:CODE(2)
	CODE16
	LDR	R1, P_NUM_SELECT_MENU_PSETTEXT
	BLX	R1
	DATA
P_NUM_SELECT_MENU_PSETTEXT
	DCD	NUM_SELECT_MENU_PSETTEXT
	CODE16
	NOP
	
	RSEG	NUM_SELECT_MENU_GUI:CODE(1)
	CODE16
	BLX	GetMLMenuGUI_adr //ml menu gui
	
	RSEG	EDIAL_CREATE_HOOK
	CODE16
	LDR	R0, =EDIAL_CREATE
	BLX	R0

	RSEG	EDIAL_REDRAW_HOOK
	CODE16
	LDR	R0, =EDIAL_REDRAW
	BLX	R0

/////////////////////////////////////////////////////
	RSEG	EDIAL_ATTR_HOOK:CODE(1)
	CODE16
	STR	R1, [SP,#8]
	STR	R2, [SP,#4]
	MOV	R2, #1
	LSL	R2, R2, #0x13
	ADD	R2, #0x40
	
	RSEG	ADRBKLST_LINES_HOOK1
	DATA
	DCD	2
	
	RSEG	ADRBKLST_LINES_HOOK2
	DATA
	DCD	2
	
	RSEG	ADRBKLST_LINES_HOOK3
	DATA
	DCD	2
	
	RSEG	ADRBKLST_ALLOCWS_HOOK
	CODE32
	ADD	R0, R0, #0x20
	NOP
	
	RSEG	ADRBKLST_CATWS_HOOK
	CODE32
	BL	ADRBKLST_CATWS
	
	RSEG	ADRBKLST_SIMALLWS_HOOK
	CODE32
	ADD	R1, R0, #0x20
	
	RSEG	ADRBKLST_SIMNUMWS_HOOK
	CODE32
	BL	ADRBKLST_SIMNUMWS
	
	RSEG	ADRBKLST_ALLOCWS_HOOK2
	CODE32
	ADD	R0, R7, #0x20
	
	RSEG	ADRBKLST_CATWS_HOOK2
	CODE32
	BL	ADRBKLST_CATWS_2
	
	RSEG	ADRBKLST_ALLOCWS_HOOK3
	CODE32
	ADD	R0, R0, #0x20
	NOP
	
	RSEG	ADRBKLST_CATWS_HOOK3
	CODE32
	BL	ADRBKLST_CATWS_3

	END



