@echo off
SET iarmake="D:\Pasha\Embedded Workbench 4.0 Evaluation2\common\bin\iarbuild.exe"

%iarmake% NATICQ.ewp -build Release_SGOLD
%iarmake% NATICQ.ewp -build Release_SGOLD_X75
%iarmake% NATICQ.ewp -build ESKA
%iarmake% NATICQ.ewp -build Release_ELKA

copy Release_SGOLD\Exe\NATICQ.elf naticq_sg_x65.elf
copy Release_SGOLD_X75\Exe\NATICQ.elf naticq_sg_x75.elf
copy ESKA\Exe\NATICQ.elf naticq_nsg.elf
copy Release_ELKA\Exe\NATICQ.elf naticq_ELKA.elf

copy 2ndconfig\2ndconfig_SGOLD.elf 2ndconfig_SGOLD.elf
copy 2ndconfig\2ndconfig_NSG.elf 2ndconfig_NSG.elf
copy 2ndconfig\2ndconfig_ELKA.elf 2ndconfig_ELKA.elf