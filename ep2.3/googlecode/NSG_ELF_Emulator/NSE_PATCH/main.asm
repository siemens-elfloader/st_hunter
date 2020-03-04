//(c)Dimadze
//NewSGold ELF Emulator Patch

                    CODE16 // Врубаем режим THUMB
                    
                    RSEG   PATCH_ONMESSAGE_HOOK:CODE 
                    
                    LDR    R0, =onmessage_hook
                    BLX    R0
                    
                    
                    RSEG   PATCH_ONKEY_HOOK:CODE 
                    
                    LDR    R0, =onkey_hook
                    BLX    R0
                    
                    
                    RSEG   PATCH_NSGCSMFINDER:CODE 
                    
                    LDR    R6, =nsg_csm_check
                    BLX    R6
                    
 
 
                    CODE32 // Врубаем режим ARM
                 

                    RSEG   PATCH_BODY:CODE 
                    
                   // Обработка клавиш  
        onkey_hook:
                   
                   ADD     LR, LR, #0x04
                   
                   LDR     R0, [R4]
                   LDR     R0, [R0,#8]
                   LDR     R2, [R0,#4]
                   LDR     R2, [R2,#0x14]
                   
                   MOV    R9,  R0
                   SWI    0x8D03
                   B      dalee
                    
                   // Обработка сообщений  
    onmessage_hook:
                     
                   ADD    LR, LR, #0x04

                   MOV    R0,  R4
                   LDR    R2, [R1, #0x00]
                   LDR    R3, [R1, #0x0C]
                   MOV    R1,  R6
                    
                   MOV    R9,  R0
                   SWI    0x8D04
                   
                   // Отправка параметров
             dalee:     
                   
                   CMP    R0,  #0xFFFFFFFF
                   MOV    R10, R0
                   MOV    R0,  R9
                   BXEQ   R2
                   BX     R10
                    
                    
                   // Обработка CSM
     nsg_csm_check:

                   ADD    LR, LR, #0x04

                   PUSH  {LR}
          
                   // Проверка на CSM NewSGold
          
                   LDR    R6,[R1, #0x08]
                   CMP    R6, #0x00
                    
                   BNE    sgold
                    
                   LDR    R6,[R1, #0x0C]
                   CMP    R6, #0x00
                    
                   BNE    sgold
                    
                   LDR    R6,[R1, #0x10]
                   CMP    R6, #0x00
                    
                   BNE    sgold
                    
                   LDR    R6,[R1, #0x14]
                   CMP    R6, #0x00
                    
                   BNE    sgold

                    
                   BL     NSG_CSM_Killer // Пробуем хоть как-то нейтрализовать CSM от NewSGold'a
                    
                    
             sgold:
             
                   MOV    R7, R3
                   MOV    R6, R1
                    
                   MOV    R4, R0
                   CMP    R0, #0
                    
                   POP   {PC}
                   

                   
   NSG_CSM_Killer:     
                    
                   // Инициализация сообщения
                   MOV    R0, #0x01
                   ADR    R1, msg
                   SWI    0x0148
                   
                   // Зануливаем аргументы
                   MOV    R0, #0x00
                   MOV    R1, #0x00
                   MOV    R2, #0x00
                   
                   // Возврат
                   BX     LR
                   
                   // Данные для сообщения
                   DATA
                   
             msg:  DC8 "Don't\nSupported!"    
              
     
                   END
                    
                    
                    
