#include "../inc/swilib.h"
#include "List.h"
#include "File.h"
#include "mlist.h"
#include "csm.h"
#include "dialog.h"

//CList *qlist;
//DialogCSM *csm;


int main()
{
  MainDialog* dialog = new MainDialog();
  StandartList *l = new StandartList;
  CList *list = new CList;
  //l->Show("Left","RIGHT");

   
  // csm->SetName("Mark");
//   csm->Create();
//   qlist = new CList;//Создаем наш лист
  //CFile *file = new CFile("4:\\classlist.txt");// файл, в качестве параметра имя файла
 // file->open(A_Create+A_Truncate+A_WriteOnly);//открываем файл,в качестве параметра флаги доступа
  
  //ну добавляем итемы
  list->add("1.fisrt");
  list->add("2.second");
  list->add("3.text");
  list->add("4.ibibibi");  
  list->add("5.)))))))");

  l->list = list;
  dialog->list = l;
  dialog->Show();  
  //for(int i=0 ; i<qlist->Count(); i++) ShowMSG(1,(int)qlist->Items(i)->name);
  
//  StandartList *l = new StandartList;
  //l->Show("Left","RIGHT");

  
  
  /*
  //сохранение в файл для наглядности
  for(int i=0 ; i<list->Count(); i++)
  {
    file->write(list->Items(i)->name,strlen(list->Items(i)->name));
    file->write("\n",1);
  }
  
  //в качестве второго параметра -1, это будет расчитываться длина строки strlen в функции write
  file->write("--------------------\n",-1);

  //удаляем второй элемент в списке ("2.second");
  list->Delete(0,1);
  
  //новая струкрута
  LIST_STRUCT *ls = new LIST_STRUCT();
    ls->next = 0;
    ls->name = "2.Insert item =)";
  list->Insert(ls,1); //вставляем структуру во вторую позицию 
 
  LIST_STRUCT *ch = new LIST_STRUCT();
    ch->next = 0;
    ch->name = "3.This is change text =)";  
  list->Change(ch,2);//изменяем структуру в третьей позиции
  
  
  LIST_STRUCT *ls2 = new LIST_STRUCT();
    ls2->next = 0;
    ls2->name = "NEw items in end";
  list->Insert(ls2,list->Count());//добавляем в конец новый элемент,альтернатива add, просто для пробы =)

  //тоже самое тока через перегрузку функций...
  list->Insert("NEw items in end22",list->Count());

  //добавление в начала списка
  list->Insert("NEw items in start",0);
  
  //сохранение в файл для наглядности
  for(int i = 0 ; i<list->Count(); i++)
  {
    file->write(list->Items(i)->name,strlen(list->Items(i)->name));
    file->write("\n",1);
  }  

  file->close();//закрываем поток, так же поток прикрывается при удалении в деструкторе,вдруг забыли =)
  file->execute();//запускаем файл
  */
  //удаляем...:)
  //delete qlist;
//  delete l;
  //delete file;  
  return 0;
}
