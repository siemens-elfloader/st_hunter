#include "config\config.h" 

func	MACRO	a,b
	PUBLIC	a
a	EQU	b
	ENDM

        RSEG	CODE:CODE


        func	zeromem,ZEROMEM
        
        END
