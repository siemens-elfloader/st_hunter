//java launch patch
            
#include "config\config.h" 
            
            RSEG    PATCH_JAVA_BEARER:CODE
            
            CODE16
#ifndef NOTALIGN            
            NOP
#endif
            
            BLX     java_control
            
            RSEG    CODE:CODE
          
         
            CODE32
            
java_control:
            
            PUSH    {R12,LR}
#ifndef NOTALIGN
            ADR     R1, rap_1    //Reset Register R1
#endif
            LDR     R12, =RAMFORPATCHADDR
            LDRB    R12, [R12, #0x09]
            
            CMP     R12, #0x00
            BEQ     end_jc
            LDR     R12, =MAINBEARER
            BLX     R12
            
    end_jc:
    
            POP     {R12,PC}   
#ifndef NOTALIGN             
            DATA
          
     rap_1: DC8 "RAP_BEARER_1"
#endif     
 
            END