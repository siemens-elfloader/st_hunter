//PCM Audio Stereo Hook (c)ILYA_ZX and fix by Dimadze for CX/M75

#include "config\config.h"

                        EXTERN  pcm_copy_frame_h


                        CODE32

#ifdef PCM_STEREO_AUDIOHOOK

                        
                        RSEG PATCH_SKIP_CHECK_SAMPLE_RATE1:CODE:ROOT
                        
                        CMP     LR, LR
#ifndef SGOLD_MP3 
                        LDREQH  R3, [R4,#0x2A]
                        LDREQH  R12,[R2,#8]
                        CMPEQ   R3, R3
                        LDREQH  R3, [R4,#0x1C]
#else
                        LDREQH  R3, [R4,#0x2E]
                        LDREQH  R12, [R2,#8]
                        CMPEQ   R3, R3
                        LDREQH  R3, [R4,#0x20]
#endif
                        
                        LDREQH  R2, [R2]
                        CMPEQ   R3, R3
                        
                        RSEG    PATCH_SKIP_CHECK_SAMPLE_RATE2:CODE:ROOT
                        
                        CMP     R0, R0

                        
                        RSEG   PATCH_DAC_SAMPLE_RATE_SET_HOOK:CODE:ROOT
                        
                        LDR     R2, =PCM_RAM_PLAYER
                        STMFD   SP!, {R3,LR}
                        LDRB    R1, [R2]
                        CMP     R1, #0
                        MOVNE   R0, R1
                        MOVNE   R1, #0
                        STRNEB  R1, [R1]
                        B       loc_jump
                        
                        
                        
                        
                        RSEG    PATCH_STEREOCONTROL:CODE:ROOT
                        
                        MOV     R4, #1
                        
                        RSEG    PATCH_PCM_COPY_FRAME_H:CODE:ROOT
                        
                        B       pcm_copy_frame_h
                        
       
                        RSEG    PATCH_FRAME_SIZE_FIX_HOOK:CODE:ROOT
                        
                        BL      GetFrameSize
                        
                        
                        
                        RSEG    PATCH_CLEAR_RAM_FRAME_FIX:CODE:ROOT
                        
                        MOV     R3, #0x140
                        
                        
                        RSEG    PATCH_CLEAR_DSP_FRAME_FIX:CODE:ROOT
                        
                        MOV     R3, #0x140
                        
                        
                        RSEG PATCH_JUMP:CODE:ROOT
                        
              loc_jump:
                      

        
#endif

#ifdef IMAADPCM_AUDIOHOOK
        
                        RSEG PATCH_OLD_AUDIOHOOK:CODE:ROOT
                        
                        B       audio_hook_body
                        
                        
#endif
                        
                     
#ifdef PCM_STEREO_AUDIOHOOK                 
                     
            
                        //PCM Stereo Audio Hook
                        
                        
                        RSEG PATCH_BODY:CODE:ROOT   
                        
                        PUBLIC  GetFrameSize
                        
         GetFrameSize:
                        
                        LDRH    R3, [R4,#0x24]
                        SUB     R1, R1, R3
                        BX      LR
     
#endif


                         
                         //ADPCM Audio Hook
                         
#ifdef IMAADPCM_AUDIOHOOK
                         
        audio_hook_body:
                         
                         LDR     R3, =ADPCM_RAM_ADDR
                         LDR     R3, [R3]
                         CMP     R3, #0
                         BNE     jump_func

               end_func:

                         LDMFD   SP!, {R3-R5,PC}

              jump_func:
     
                         MOV     R3, #0
                         STR     R3, [R4,#0x50]
                         B       end_func

                         
#endif      
                         END
                        
                        



                        
                        
                        
