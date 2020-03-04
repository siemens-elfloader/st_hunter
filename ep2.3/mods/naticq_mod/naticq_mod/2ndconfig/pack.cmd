@echo off
SET iarmake="D:\Pasha\ARM_Embedded_Workbench\Embedded Workbench 4.0 Evaluation\common\bin\iarbuild.exe"

%iarmake% NATICQ.ewp -build SGOLD
%iarmake% NATICQ.ewp -build NEWSGOLD
%iarmake% NATICQ.ewp -build ELKA

copy SGOLD\Exe\2ndconfig.elf 2ndconfig_SGOLD.elf
copy NEWSGOLD\Exe\2ndconfig.elf 2ndconfig_NSG.elf
copy ELKA\Exe\2ndconfig.elf 2ndconfig_ELKA.elf
