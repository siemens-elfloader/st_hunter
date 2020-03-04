@echo off
SET iarmake="D:\Pasha\Embedded Workbench 4.0 Evaluation2\common\bin\iarbuild.exe"

%iarmake% mail_weather.ewp -build Release_SGOLD
%iarmake% mail_weather.ewp -build ESKA
%iarmake% mail_weather.ewp -build Release_ELKA

copy Release_SGOLD\Exe\mail_weather.elf mail_weather_SGOLD.elf
copy ESKA\Exe\mail_weather.elf mail_weather_NSG.elf
copy Release_ELKA\Exe\mail_weather.elf mail_weather_ELKA.elf
