Это полностью моя утилита для перевода секций формата ELF (Executable and Linkable Format) в VKP (V_Klay Patch)  патч. В отличии от традиционной утилиты elf2vkp, данная версия имеет обширное количество опций и не имеет привязки к Siemens.



Показ всех опций можно сделать вот так:

elf2vkp.exe -help

Самый простейший способ использования:

elf2vkp.exe patch.elf

Ну чтобы к примеру использовать для Siemens, надо запустить примерно так:

elf2vkp.exe patch.elf -vkp patch.vkp -ff FF.bin -hc CX75v25 -nr Патч -c Pupkin  -ba 0xA0000000

Если вам надо собрать патч для EXE, а там физичечкие адреса не совсем совпадают со смещениями файла, вы можете корректировать адреса для генерации патча по нужным смещениям вот так:

elf2vkp.exe patch.elf -ff program.exe -sa .text=0x120000 -sa .data=0x2444000

.text и .data это названия секций, адреса которых будут генерироватся по соответствующим смещениям в файле.

Это, конечно, очень скудная справка, но если вам надо знать больше, то задавайте вопросы на нашем форуме.
