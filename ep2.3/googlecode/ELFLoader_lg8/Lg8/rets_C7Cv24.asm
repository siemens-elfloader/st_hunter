	RSEG	DATA_N
	RSEG	SWILIB_FUNC1B8_1BB:CODE
	EXTERN	EXT2_AREA
	EXTERN	pngtop
	EXTERN	pLIB_TOP
	DCD	EXT2_AREA
	DCD	pngtop
	DCD	pLIB_TOP
	DCD	SFE(DATA_N)

defadr	MACRO	a,b
	PUBLIC	a
a	EQU	b
	ENDM

        RSEG	CODE:CODE

	PUBLIC	OldOnClose
OldOnClose:
	DCD	0xA02C3C42+1 
	
	PUBLIC	OldOnCreate
OldOnCreate:
	DCD	0xA02C39E8+1
        
	PUBLIC	ESIOld
ESIOld:
	DCD	0xA07383AE+1
        
        PUBLIC	PITgetN
PITgetN:
	DCD	0xA00DF772+1 
	
	PUBLIC	PITret
PITret:
	DCD	0xA00DF788+1
        
        PUBLIC	KEYBret
KEYBret:
	DCD	0xA00F3D82+1 
        
        PUBLIC	KEYBfunc
KEYBfunc:
	DCD	0xA00E4A7C+1          
        
	PUBLIC	FReadOld
FReadOld:
	DCD	0xA09BBC48 
        
        PUBLIC	FWriteOld
FWriteOld:
	DCD	0xA09BBD30       
        
        defadr	StoreErrInfoAndAbort,0xA073FF78  
	defadr	StoreErrString,0xA073FD9C 

	END