
#include <spl/exc_handler.h>
#include <spl/process.h>
#include <spl/thread.h>
#include "emu_context.h"


void AbortCommonHandler(int id, unsigned long lr, unsigned long cpsr, int type);

extern char abt_stack[ABORT_STACK_SIZE];
extern char und_stack[UND_STACK_SIZE];


CONTEXT context;


void (*emualate_load_store_instruction_sub)(CONTEXT *ctx) = 0;



void SetDomainAccess(unsigned int domains)
{
    //__MCR(15, 0, domains, 3, 0, 0);
    for (volatile int i = 0; i < 20; i++);
}


void emualate_load_store_instruction()
{
    if(emualate_load_store_instruction_sub)
        emualate_load_store_instruction_sub(&context);
}

