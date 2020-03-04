#include <swilib.h>
#include <conf_loader.h>

extern char const entry[9];


int main(char *exename, char *fname)
{
    char *elf_name = strrchr(exename, '\\');
    if(elf_name && elf_name[0] == '\\') elf_name++;
    else elf_name = "AddressExecuter.elf";

    InitConfig_name(elf_name);

    if(entry[1] == 'X'){
        ShowMSG(1, (int)"Конфиг создан, впишите аддрес");
        kill_elf();
        return 0;
    }

    uint32_t run = 0;
    sscanf(entry, "%X", &run);


    if(run < 0xA0000000 || run > 0xA9000000) {
        ShowMSG(1, (int)"Неверный адрес!");
        kill_elf();
        return 0;
    }

    ((void (*)())run)();

    kill_elf();
    return 0;
}
