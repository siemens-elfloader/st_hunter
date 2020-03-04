	PUBLIC	ELF_BEGIN
	RSEG	ELFBEGIN:DATA
 
ELF_BEGIN

	RSEG	CODE:CODE
	
	#include "MP3\Polyphase.asm"
        #include "MOD\mix_asm.asm"
        #include "AAC\AACDEC\asm\armgcc\sbrcov.s"
        #include "AAC\AACDEC\asm\armgcc\sbrqmfak.s"
        #include "AAC\AACDEC\asm\armgcc\sbrqmfsk.s"
        
	PUBLIC	kill_data
	CODE32
kill_data
	BX	R1

	END
	