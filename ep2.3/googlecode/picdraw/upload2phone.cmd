REM Пропишите в строках ниже пути к OBEXTOOL и к эльфу (в директории Release текущего проекта)
SET obextool="C:\Program Files\totalcmd65\Plugins\wfx\VSOFS\obexcopy.exe"
SET elf_file="E:\IARProj\tutorial\PicDraw\Release\Exe\PicDraw.elf"
%obextool% %elf_file% \Data\PicDraw.elf