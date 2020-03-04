/* :P */

@ Дизасм из натаськи))

		.code 32
		.align 4
		.global ExecuteIMB
ExecuteIMB:

var_4		= -4
arg_4		=  4

		STR	LR, [SP,#var_4]!
		MRS	R2, CPSR
		SVC	4
		MRS	R1, CPSR
		ORR	R1, R1,	#0xC0
		MSR	CPSR_c,	R1

loc_7C4:
		MRC	p15, 0,	PC,c7,c10, 3
		BNE	loc_7C4
		MOV	R0, #0
		MCR	p15, 0,	R0,c7,c10, 4
		MCR	p15, 0,	R0,c7,c5
		MSR	CPSR_c,	R2
		LDR	PC, [SP],#arg_4
@ End of function ExecuteIMB


		.code 32
		.align 4
		.global aa
aa:
		STMFD	SP!, {R4,R5,LR}
		SUB	SP, SP,	#0x5C
		MOV	R4, R0
		LDRH	R0, [R4,#4]
		SUBS	R0, R0,	#0x23
		BEQ	loc_7C
		SUBS	R0, R0,	#1
		BEQ	loc_110
		B	loc_128
@ ---------------------------------------------------------------------------

loc_7C:
		MOV	R0, #0x18
		SVC	0x14
		MOV	R5, R0
		LDR	R0, [R4]
		STR	R0, [R5]
		MOV	R0, #0x23
		STRH	R0, [R5,#4]
		MOV	R0, #0x10
		STRH	R0, [R5,#6]
		MOV	R0, SP
		SVC	0x23D
		LDRH	R2, [R4,#6]
		ADD	R1, R4,	#8
		LDR	R4, =PASS
		MOV	R0, SP
		SVC	0x23E
		MOV	R0, R4
		SVC	0x1B
		MOV	R1, R4
		LDR	R4, =0
		MOV	R2, R0
		MOV	R0, SP
		SVC	0x23E
		MOV	R0, R4
		SVC	0x1B
		MOV	R2, R0
		MOV	R1, R4
		MOV	R0, SP
		SVC	0x23E
		MOV	R1, SP
		ADD	R0, R5,	#8
		SVC	0x23F
		MOV	R1, R5

loc_100:
		MOV	R0, #0
		BL	SendAnswer
		ADD	SP, SP,	#0x5C
		LDMFD	SP!, {R4,R5,PC}
@ ---------------------------------------------------------------------------

loc_110:
        BL	ExecuteIMB
		MOV	R0, R4
		ADD	R1, R4,	#8
		BLX	R1
		MOV	R1, R0
		B	loc_100
@ ---------------------------------------------------------------------------

loc_128:
		LDRH	R0, [R4,#6]
		MOV	R1, #0
		ADD	R0, R0,	R4
		STRB	R1, [R0,#8]
		ADD	SP, SP,	#0x5C
		LDMFD	SP!, {R4,R5,PC}
@ End of function aa


@ =============== S U B	R O U T	I N E =======================================

		.code 32
		.align 4
		.global ab
ab:

		LDR	R0, =connect_state
		LDR	R2, =dword_20C
		STMFD	SP!, {R4,R5,R6,LR}
		SUB	SP, SP,	#0x10
		MOV	R1, #2
		STR	R1, [R0]
		@MOVL	R3, 0xE09

		mov r3, #0xE10
		sub r3, r3, #0x1
		//LDR r3, =word_E09

		MOV	R1, #9
		MOV	R0, SP
		SVC	0x11B
		MOV	R0, #0x10
		SVC	0x14
		MOV	R4, R0
		LDR	R0, =UIN
		MOV	R2, #8
		LDR	R5, [R0]
		MOV	R0, #0x20 @ ' '
		STR	R5, [R4]
		STRH	R0, [R4,#4]
		MOV	R0, #8
		STRH	R0, [R4,#6]
		MOV	R1, SP
		ADD	R0, R4,	#8
		SVC	0x11E
		MOV	R1, R4
		MOV	R0, #0
		BL	SendAnswer
		MOV	R0, #8
		SVC	0x14
		STR	R5, [R0]
		MOV	R1, #1
		STRH	R1, [R0,#4]
		MOV	R1, #0
		STRH	R1, [R0,#6]
		MOV	R1, R0
		MOV	R0, #0
		BL	SendAnswer
		LDR	R0, =RXstate
		MOV r1, #0xFFFFFFF8
		STR	R1, [R0]
		LDMFD	SP!, {R0-R6,PC}
@ End of function ab

@ ---------------------------------------------------------------------------


dword_20C:	.4byte 0x5F656953
            .4byte 0x64343025
            .byte 0

word_E09:   .2byte 0xE09
            .byte  0

@ =============== S U B	R O U T	I N E =======================================


		.code 32
		.align 4
		.global ad
ad:
		BX	LR
@ End of function ad


@ =============== S U B	R O U T	I N E =======================================


		.code 32
		.align 4
		.global ae
ae:
		B	ProcessPacket
@ End of function ae



