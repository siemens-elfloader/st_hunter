/*
Загрузчик конфигурации
(c) Rst7, Kibab
 Осуществляет загрузку конфига и предоставляет функции инициализации
*/
#ifndef _CONFLOADER_H_
  #define _CONFLOADER_H_

// Вызвать обязательно перед обращением к переменным конфигурации
//void InitConfig();
int InitConfig(void* cfgstruct, int len, char *name,int disk);

extern const char *successed_config_filename;
int SaveConfigData(const char *fname);

#endif
