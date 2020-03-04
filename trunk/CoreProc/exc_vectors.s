

#include "arm_defs.h"

                        .extern  abt_stack
                        .extern  und_stack


                        .extern  da_def_handler
                        .extern  pa_def_handler
                        .extern  ui_def_handler


                        .extern  swi2_def_jumper
                        .extern  swi3_def_jumper


                        .extern  getpid
                        .extern  AbortCommonHandler
                        .extern  get_stack_pointer_from_proc




                        .global  da_handler_vector
                        .global  pa_handler_vector
                        .global  ui_handler_vector


                        .global  swi2_vector
                        .global  swi3_vector


                        .arm
                        @ Data Abort Handler


                        .global processException
                        @ void processException(int pid)
     processException:

                        mov   r4, r0                    @ сохраним r0(pid)
                        @b     processExceptionEvent      @ вызываем обработчик исключения от имени процесса

                        mov   r0, r4                    @ восстановим r0
                        @bl    processStateContext       @ берём контекст

                        ldr   sp, [r0, #13*4] @ sp      @ восстановим sp
                        ldr   lr, [r0, #14*4] @ lr      @ востановим lr

                        ldmia r0, {r0-r12}    @ r0-r12  @ востанавливаем остальные регистры

                        movs  pc, lr                    @ возвращаемся к выполнению программы

     da_handler_vector:

                        stmfd  sp!, {r0-r3, lr}
                        bl     getpid                   @ берём pid
                        @bl     processStateContext      @ берёмместо под контекст
                        cmp    r0, #0                   @ проверяем
                        beq    no_proc_ctx

                        ldmfd  sp!, {r1}                @ восстанавливаем r0
                        str    r1, [r0]                 @ пишем в контекст

                        ldmfd  sp!, {r1-r3, lr}         @ восстанавливаем lr

                        add   r0, #4                    @ отнимает 4 байта от контекста, там записан r0
                        stmia r0, {r1-r15}              @ записываем остальные регистры
                        sub   r0, #4                    @ восстанавливаем положение


                        mrs   r1, spsr                  @ берём состояние флагов процессора
                        str   r1, [r0, #16*4]           @ пишем в контекст

                        mov   r1, #0x10
                        msr   cpsr_c, r1

                        bl    getpid                    @ берём pid
                        @b     processExceptionHandler   @ вызываем обработчик
                        @b     processException

        no_proc_ctx:

                        @subs    pc, lr, #4

                        @ Prefetch Abort Handler



       pa_handler_vector:


                         LDR       SP, =abt_stack+0x1000


                         SUB       R5,  LR, #0x04
                         MRS       R6,  SPSR


                         BL        getpid


                         CMP       R0, #-1
                         BEQ       no_elf_pabort

                         MOV       R4, R0
                         BL        gettid
                         MOV       R7, R0
                         MOV       R0, R4

                         MOV       R1, R0
                         MOV       R2, R5
                         MOV       R3, R6


                         STMFD     SP!, {R1, R5-R7}
                         BL        get_stack_pointer_from_proc
                         LDMFD     SP!, {R1, R5-R7}


                         MOV       SP, R0


                         MOV	   R1, R1, ASL #16
                         MOV	   R7, R7, ASL #16
                         ORR       R0, R1, R7, LSR #16
                         MOV       R1, R5
                         MOV       R2, R6
                         MOV       R3, #0x02


                         BIC       R6, R6, #0xC0
                         MSR       CPSR_c, R6


                         B         AbortCommonHandler


          no_elf_pabort:


                         ADD       LR, R5, #0x04
                         LDR       R5, =pa_def_handler
                         LDR       R5, [R5, #0x00]
                         MOV       PC, R5






                         @ Undefined Instruction Handler


       ui_handler_vector:


                         LDR       SP, =und_stack+0x1000


                         SUB       R5,  LR, #0x04
                         MRS       R6,  SPSR


                         BL        getpid


                         CMP       R0, #-1
                         BEQ       no_elf_undef

                         MOV       R4, R0
                         BL        gettid
                         MOV       R7, R0
                         MOV       R0, R4

                         MOV       R1, R0
                         MOV       R2, R5
                         MOV       R3, R6


                         STMFD     SP!, {R1, R5-R7}
                         BL        get_stack_pointer_from_proc
                         LDMFD     SP!, {R1, R5-R7}


                         MOV       SP, R0


                         MOV	   R1, R1, ASL #16
                         MOV	   R7, R7, ASL #16
                         ORR       R0, R1, R7, LSR #16
                         MOV       R1, R5
                         MOV       R2, R6
                         MOV       R3, #0x03


                         BIC       R6, R6, #0xC0
                         MSR       CPSR_c, R6


                         B         AbortCommonHandler


          no_elf_undef:




                         ADD       LR, R5, #0x04
                         LDR       R5, =ui_def_handler
                         LDR       R5, [R5, #0x00]
                         MOV       PC, R5


                         @(S)Exits SWI2 && SWI3


                         @ --- SWI2 ---




            swi2_vector:


                         MOV       R3, LR


                         STMFD     SP!, {R3}
                         BL        gettid
                         MOV       R4, R0
                         BL        getpid
                         LDMFD     SP!, {R3}


                         CMP       R0, #-1
                         BEQ       no_elf_swi2


                         MOV       R1, R0


                         LDMFD     SP!, {R2}




                         STMFD     SP!, {R1-R3}
                         BL        get_stack_pointer_from_proc
                         LDMFD     SP!, {R1-R3}


                         MOV       SP, R0


                         MOV	   R1, R1, ASL #16
                         MOV	   R4, R4, ASL #16
                         ORR       R0, R1, R4, LSR #16
                         MOV       R1, R3
                         MOV       R3, #0x04






                         MOV       R6, R2
                         BIC       R6, R6, #0xC0
                         MSR       CPSR_c, R6


                         B         AbortCommonHandler




            no_elf_swi2:


                         LDR       R0, =swi2_def_jumper
                         LDR       R0, [R0, #0x00]
                         MOV       PC, R0




                         @ --- SWI3 ---


            swi3_vector:


                         MOV       R3, LR


                         STMFD     SP!, {R3}
                         BL        gettid
                         MOV       R4, R0
                         BL        getpid
                         LDMFD     SP!, {R3}


                         CMP       R0, #-1
                         BEQ       no_elf_swi3




                         MOV       R1, R0


                         LDMFD     SP!, {R2}




                         STMFD     SP!, {R1-R3}
                         BL        get_stack_pointer_from_proc
                         LDMFD     SP!, {R1-R3}


                         MOV       SP, R0


                         MOV	   R1, R1, ASL #16
                         MOV	   R4, R4, ASL #16
                         ORR       R0, R1, R4, LSR #16
                         MOV       R1, R3
                         MOV       R3, #0x04



                         MOV       R6, R2
                         BIC       R6, R6, #0xC0
                         MSR       CPSR_c, R6


                         B         AbortCommonHandler




            no_elf_swi3:


                         LDR       R0, =swi3_def_jumper
                         LDR       R0, [R0, #0x00]
                         MOV       PC, R0



