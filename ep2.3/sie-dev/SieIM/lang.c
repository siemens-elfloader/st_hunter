#include "inc\swilib.h"
#include "lang.h"

/*

Тут отсутствует проверка на запись/чтение пустых строк. Не надо добавлять проверку,
ибо это может повлиять на корреутность отображения эльфон ленга!!

*/

int GetFileSize(const char * fname)	// размер файла MC(с)
{
  unsigned int io_error;
  FSTATS fs;
  if (GetFileStats(fname, &fs, &io_error)==-1)
    return 0;
  return (fs.size);
}

int InitLgp(char *lang, void *lng)	// если ленг структура
{
	int f;
	int i = 1;
	unsigned err;
	LANGH head;
  
  if ((f=fopen(lang,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
  {
    fread(f,&head,sizeof(head),&err);
	if (sizeof(head)+head.size == GetFileSize(lang))	// если размер совпадает
		fread(f,lng,head.size,&err);	// читаем структуру
	else	// иначе
		i = 0;	// идем нах
    fclose(f,&err);
  }
	else
		i = 0;
  
	return i;	// результат чтения ленга (1 - успешно, 0 - ошибка)
}

int InitLgp_2(char *lang, void **array)	// если ленг массив
{
	int f;
	int i = 1;
	unsigned err;
	LANGH head;
  
  if ((f=fopen(lang,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
  {
    fread(f,&head,sizeof(head),&err);
	if (sizeof(head)+head.size == GetFileSize(lang))	// если размер совпадает
	{
		array = malloc(4*head.count);	// резервируем место под массив
		for (register int i = 0;i <= head.count; i++)	// заполняем строки
		{
			int size = 0;	// размер строчки
			
			fread(f,&size,sizeof(size),&err);	// считываем размер
			array[i] = malloc(size);	// резервируем место под строку
			fread(f,array[i],size,&err);	// саму строчку
		}
	}
	else
		i = 0;
    fclose(f,&err);
  }
	else
		i = 0;
  
	return i;	// результат чтения ленга (1 - успешно, 0 - ошибка)
}

int CreateLgp(char *lang,  int build, int type, int count, int size, char *cright, void *lng)	// если структура
{
	int f;
	int i = 1;
	unsigned err;
	LANGH head;
	// заносим данные
	head.build = build;	// сборка
	head.type = type;	// тип языка
	head.count = count;	// количество строк
	snprintf(head.cright,64,cright);	// копирайты
	head.size = sizeof(head) + size;	// размер ленга
  
  if ((f=fopen(lang,A_WriteOnly+A_BIN+A_Create,P_WRITE,&err))!=-1)
  {
    fwrite(f,&head,sizeof(head),&err);
    fwrite(f,&lng,head.size,&err);
    fclose(f,&err);
  }
	else
	i = 0;
  
	return i;	// результат записи ленга (1 - успешно, 0 - ошибка)
}

int CreateLgp_2(char *lang,  int build, int type, int count, int size, char *cright, void **array)	// если массив
{
	int f;
	int i = 1;
	unsigned err;
	LANGH head;
	// записываем информацию
	head.build = build;	// сборка
	head.type = type;	// тип языка
	head.count = count;	// количество строк
	snprintf(head.cright,64,cright);	// копирайты
	head.size = sizeof(head) + size + count * sizeof(int);	// надо уточнить на счет размера,
								// т.к. может ошибся в вычислении
  
  if ((f=fopen(lang,A_WriteOnly+A_BIN+A_Create,P_WRITE,&err))!=-1)
  {
    fwrite(f,&head,sizeof(head),&err);	// пишем заголовок
		
	for (register int i = 0; i <= count; i++)	// записываем сами строки
	{
		int str_size = 0;	// размр строки
		
		str_size = strlen(array[i]);
		// все вычли, записываем
		fwrite(f,&str_size,sizeof(int),&err);	//	размер строчки
		fwrite(f,array[i],str_size,&err);	// саму строчку
	}
    fclose(f,&err);
  }
	else
	i = 0;
  
	return i;
}

int FreeLgp()
{
	return 0;	// а это нах не нужно
}

int GetLgpInfo(char *lang, LANGH *head)	// получение информации о ленге
{
	int f;
	int i = 1;
	unsigned err;
  
  if ((f=fopen(lang,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
  {
    fread(f,&head,sizeof(head),&err);
    fclose(f,&err);
  }	
  else
		i = 0;

		return i;	// если 1 - то прочитано, иначе - 0
}
