@echo off
SET iarmake="D:\Pasha\ARM_Embedded_Workbench\Embedded Workbench 4.0 Evaluation\common\bin\iarbuild.exe"

%iarmake% NATICQ.ewp -build SGOLD
%iarmake% NATICQ.ewp -build NEWSGOLD
%iarmake% NATICQ.ewp -build ELKA2

copy SGOLD\Exe\1stconfig.elf 1stconfig_SGOLD.elf
copy NEWSGOLD\Exe\1stconfig.elf 1stconfig_NSG.elf
copy ELKA2\Exe\1stconfig.elf 1stconfig_ELKA.elf
