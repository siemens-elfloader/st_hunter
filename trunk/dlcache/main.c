#include <swilib.h>


/*

  Если вы не хотите использовать libcrt.a и прочие библиотеки
  и вам нужен принципиально мелкий эльф то:

  1. Вам понадобится скрипт линкера, который лежит в корне проекта
  2. вместо main использовать _start либо в параметры линкера добавить -emain
  3. Не использовать kill_elf(), а использовать конструкцию убийсва описанную ниже(почти как на иаре)
  4. Использовать дефайн __NO_DIRECT_SWI(будет задействована комманда swi, чтобы её использовать вам нужен пропатченный гцц.
          Патч http://team-sc.ru/down/gcc_462_swi_call_siemens2patch_130.zip от Barracuda)
  5. Использовать дефайн __NO_LIBC
  6. Использовать ключ -fno-builtin ( в этом случае гцц не будет некоторые функции екстернить из *.so (memcpy memset etc...) )
  7. Не использовать ключ --defsym __ex=0

*/


/* Такая же как в яре kill_data */
__attribute__((naked))
void kill_data(void *p, void (*func_p)(void *))
{
    asm ("bx  %1\n" :: "r"(p), "r"(func_p));
}


void Exit()
{
    /* будет релок на старт эльфа, объявленна в скрипте */
    extern unsigned __executable_start;

    /*
        ldr r0, =__executable_start
        bx  kill_data
    */
    kill_data(&__executable_start, (void (*)(void *))mfree_adr());
}


int _start(char *exename, char *fname)
{
    char tmp[26];
    int cleaned = 0;

    cleaned = dlclean_cache();

    if(cleaned == -1)
        sprintf(tmp, "Кеш пуст", cleaned);
    else
        sprintf(tmp, "Очищено %d библиотек", cleaned);
    ShowMSG(1, (int)tmp);

    SUBPROC(Exit);
    return 0;
}
