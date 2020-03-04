//repfunc.asm - replaced functions
//void DrawImgBW(unsigned int x, unsigned int y, unsigned int picture, char *pen, char *brush) and void DrawImg(unsigned int x, unsigned int y, unsigned int picture) 
                    EXTERN DrawImgBW_new
                       
                    RSEG PATCH_DRAWIMGBW:CODE:ROOT
                   
                  
                    CODE16 //Cut in THUMB mode

                    LDR   R7,=DrawImgBW_new //Load the register R7 address of the new F-uu DrawImgBW_new
 	    BX    R7                //Arrives at an absolute address with the change of command on ARM
                    
                     //Zabem remaining fragments of the old F-uu empty commands...
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP

                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
                    NOP
                    NOP
                    NOP
                    
                    NOP
                    NOP
              
                    END
//
                 
                    
                    
                    
