@echo off
SET iarmake="D:\Pasha\Embedded Workbench 4.0 Evaluation2\common\bin\iarbuild.exe"


%iarmake% NATICQ.ewp -build Release_SGOLD_X75

copy Release_SGOLD_X75\Exe\NATICQ.elf naticq_sg_x75.elf

copy 2ndconfig\2ndconfig_SGOLD.elf 2ndconfig_SGOLD.elf
copy 2ndconfig\2ndconfig_ELKA.elf 2ndconfig_ELKA.elf