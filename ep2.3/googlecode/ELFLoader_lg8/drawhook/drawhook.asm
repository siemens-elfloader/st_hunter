//drawhook.asm  
//(c)Dimadze
//DRAW_HOOK

                    EXTERN DrawObject_5_0x0A
                    EXTERN oldDrawObj_sub_1
                    EXTERN oldDrawObj_sub_2
                    
                    CODE16  //We cut in the mode THUMB
                    
                       
                    RSEG PATCH_DRAWOBJECT:CODE
                    
#ifndef NOTALIGN    //If address (address - 1) DrawObject aligned multiple of four, then use this Products DirectiveNOTALIGN, in xcl-fail to enter address - 1 
                    //If no,that should in fail to enter address xcl-(Yes, yes, of swilib.vkp) DrawObject - 3 (to take away 3)
                    //§¬§à§ã§ñ§Ü§Ú elf2vkp.exe ...

                    POP     {R4,R5,R7,PC} //§£§à§ã§ã§ä§Ñ§ß§Ñ§Ó§Ý§Ú§Ó§Ñ§Ö§Þ §Ý§Ú§ê§ß§Ú§ð §Ù§Ñ§ä§×§â§ä§å§ð §Ü§à§Þ§Ñ§ß§Õ§å
#endif
                    PUBLIC start

             start:
                    
                    PUSH  {R4,LR}

                    LDR   R4,=drawobj_new
 	            BLX   R4 //§±§à§Û§Õ§×§Þ §ß§Ñ §à§Ò§ß§à§Ó§Ý§×§ß§ß§å§ð §æ-§Ú§ð §à§ä§â§Ú§ã§à§Ó§Ü§Ú
                    
                    POP   {R4,PC}
                    
                    
                    RSEG PATCH_BODY:CODE
                    

       drawobj_new:
                    
                    PUSH  {R4,R5,LR}
                    
                    LDRB  R5, [R0, #0x00] //Load the first byte DRWOBJ
                    
                    CMP   R5, #0x05       //Checking whether an object is the fifth drawing?
                    BEQ   alpha_check     //Depending on the test proceed to check the alpha channel
                    
         not_alpha: //No, no alpha channel, draw the old-fashioned
                    
                    MOV   R4, R0
                    
                    LDR   R5 ,=oldDrawObj_sub_1
                    LDR   R5 ,[R5, #0x00]
                    BLX   R5
               
                    
                    CMP   R0, #0x00 
                    BEQ   end_drawobj_new
                    MOV   R1, R4
                    
                    LDR   R5 ,=oldDrawObj_sub_2
                    LDR   R5 ,[R5, #0x00]
                    BLX   R5
                    
                    
   end_drawobj_new:
                        
                    POP   {R4,R5,PC}
                    
       alpha_check: //Yeah, still a 5 object rendering
           
                    LDR   R5, [R0, #20]
                    LDRH  R5, [R5, #0x02]
                    
                    CMP   R5, #0x000A //Checking whether the alpha channel?
                    BNE   not_alpha
                    
                    //Yeah, the alpha channel is! Draw a special function...
                    LDR   R5, =DrawObject_5_0x0A            
 	            BLX   R5
                    
                    POP   {R4,R5,PC}
              
                    END ;start
                    
                 
                    
                    
                    
