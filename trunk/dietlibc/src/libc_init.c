
#include <swilib.h>
#include <stdlib.h>
#include <ep3/loader.h>


/* === on exit shared lib === */
extern void __libc_exit(int);
void _fini()
{
    //exit(0);
    __libc_exit(0);
}



/* === on init ==========*/

void _init()
{
    extern void *__ex;
    Elf32_Exec *ex = (Elf32_Exec *)&__ex;
/*
    FILE *fp = fopen("4:\\log.txt", "w+");
    if(fp)
    {
        fprintf(fp, "Libc addr: %X - %d", ex->body, ex->bin_size);
        fclose(fp);
    }*/
}
