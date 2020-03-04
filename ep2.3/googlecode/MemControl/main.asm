//Mem_Control Patch X65
//If not a company (not a word "Linking") or does not start postbuild.cmd helps Project -> Clean
  

#include "config\config.h" 
   
            EXTERN SeekBlock

            RSEG PATCH_MAINENTER:CODE:ROOT
 
 
            CODE32 //ARM mode
            
        
            PUBLIC start
             
     start:
        
            LDR     PC, =main
        
        
        
 
            RSEG    CODE:CODE
            

  
      main:
       
            PUSH   {LR}

#ifdef CUTHEAP
            LDR     R3, =0xA8000400
#else
            LDR     R3, =0xA8000000
#endif


            STR     R3, [R0, #0]
            
            BL      execute
        
            LDR     R0, =RAMFORPATCHADDR 
            LDR     R0, [R0, #0x00]      //Total Size = NonPermMem + PermMemory
        
            STR     R0, [R1, #0]
        
            LDR     R0, =RAMFORPATCHADDR 
            LDR     R0, [R0, #0x04]      //PermMemory Size = PermMem + JavaHeap
            
            STR     R0, [R2, #0]
        
            POP    {PC}
            
            
           
   execute:
           
            PUSH    {R0-R2,LR}
#ifdef CUTHEAP            
            MOV      R0, #0xA8000000
            MOV      R1, #0x400

            LDR      R2, =ZEROMEM
            BLX      R2
#endif   
            LDR      R2, =SeekBlock
            BLX      R2
            
            BLX      read_config
            
            
            RSEG     CODE:CODE
 
            CODE16   //THUMB mode
               
read_config:

            LDR      R2, =RAMFORPATCHADDR
            
            LDR      R0, [R2, #0x0C]
            LDRB     R0, [R0, #0x00]
            
            LDR      R1, =DEF_TOTALSIZE
            STR      R1, [R2, #0x00]
            
            LDR      R1, =DEF_PERMSIZE
            STR      R1, [R2, #0x04]
            
            MOV      R1, #0x01
            STRB     R1, [R2, #0x08]
            STRB     R1, [R2, #0x09]
            
            
            //R0 = LAUNCH PARAMETR
            //R2 = RAMFORPATCHADDR
            
            CMP      R0, #0x00
            BEQ      return         //Normal mode
            
            CMP      R0, #0x01
            BEQ      not_browser    //Murdered browser
            
            CMP      R0, #0x02
            BEQ      inc_nonperm    //Increased NonPermMem
            
            CMP      R0, #0x03
            BEQ      notbr_incnon   //Murdered browser + Increase NonPermMem
            
            CMP      R0, #0x04
            BEQ      optimize_1     //Optimal mode NonPerm-JavaHeap #1
            
            CMP      R0, #0x05
            BEQ      optimize_2     //Optimal mode NonPerm-JavaHeap #2
            
            CMP      R0, #0x06
            BEQ      max_nonperm    //Maximum NonPermMem
            
            CMP      R0, #0x07
            BEQ      max_javaheap   //Maximum JavaHeap
            

     return:
        
            POP     {R0-R2,PC}
        
        
            
not_browser:   
            
            
            MOV      R1, #0x00
            STRB     R1, [R2, #0x08]
            
            B        return
            
            
inc_nonperm:   
            
            LDR      R1, =MAX_TOTALSIZE
            STR      R1, [R2, #0x00]
            
            LDR      R1, =MIN_PERMSIZE
            STR      R1, [R2, #0x04]
            
            B        return
            
            
notbr_incnon:
            
            LDR      R1, =MAX_TOTALSIZE
            STR      R1, [R2, #0x00]
            
            LDR      R1, =MIN_PERMSIZE
            STR      R1, [R2, #0x04]
           
            MOV      R1, #0x00
            STRB     R1, [R2, #0x08]
            
            B        return
            
           
 optimize_1:   
            
            LDR      R1, =MAX_TOTALSIZE
            STR      R1, [R2, #0x00]
            
            LDR      R1, =OPT1_PERMSIZE
            STR      R1, [R2, #0x04]
            
            MOV      R1, #0x00
            STRB     R1, [R2, #0x08]
            
            LDR      R1, =OPT1_JAVAHEAP
            LDR      R2, =RAMJAVAHEAPSIZE
            STR      R1, [R2, #0x00]
            
            B        return
            
 optimize_2:   
            
            LDR      R1, =MAX_TOTALSIZE
            STR      R1, [R2, #0x00]
            
            LDR      R1, =OPT2_PERMSIZE
            STR      R1, [R2, #0x04]
            
            MOV      R1, #0x00
            STRB     R1, [R2, #0x08]
            
            LDR      R1, =OPT2_JAVAHEAP
            LDR      R2, =RAMJAVAHEAPSIZE
            STR      R1, [R2, #0x00]
            
            B        return


max_nonperm:   
            
            LDR      R1, =MAX_TOTALSIZE
            STR      R1, [R2, #0x00]
            
            LDR      R1, =MIN_PERMSIZE2
            STR      R1, [R2, #0x04]
            
            MOV      R1, #0x00
            STRB     R1, [R2, #0x08]
            STRB     R1, [R2, #0x09]
            
            LDR      R2, =RAMJAVAHEAPSIZE
            STR      R1, [R2, #0x00]
            
            B        return
            
max_javaheap:   
            
            LDR      R1, =MAX_TOTALSIZE
            STR      R1, [R2, #0x00]
            
            LDR      R1, =MAX_PERMSIZE
            STR      R1, [R2, #0x04]
            
            MOV      R1, #0x00
            STRB     R1, [R2, #0x08]

            LDR      R1, =MAX_JAVAHEAP
            LDR      R2, =RAMJAVAHEAPSIZE
            STR      R1, [R2, #0x00]
            
            B        return
            
            END

