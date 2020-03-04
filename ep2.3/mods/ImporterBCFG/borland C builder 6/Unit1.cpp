//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <io.h>
#include <alloc.h>
#include <fcntl.h>
#include <sys\stat.h>


#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

CFG_HDR *cfg_h[512];
CFG_HDR *cfg_h2[512];
int total_items=0;
int total_items2=0;
char buf[1024*100];
char buf2[1024*100];
int size;
int size2;
char* __fastcall OpenFile(char *fname,int b)
{
 int handle;
 int &bytes =   (!b) ? size : size2;
 char *mybuf = (!b) ? (buf) : (buf2);
 if ((handle = open(fname, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD)) == -1)
   {
      char s[128];
      sprintf(s,"Error Opening File %s",fname);
      ShowMessage(s);
      exit(1);
   }

   if ((bytes = read(handle, mybuf, 1024*100)) == -1)
   {
      ShowMessage("Read Failed.\n");
      exit(1);
   }  /*
   else
   {
    Form1->ListBox1->Items->Add("File read");
    Form1->ListBox2->Items->Add("File read");
//    ShowMessage(IntToStr(bytes));
   }*/
  close(handle);
  //if(!b) size=bytes;
//  else size2=bytes;
  return mybuf;
}

void __fastcall Compare(void *p,CFG_HDR *hp,int type)
{
 char *p2 = OpenFile("NATICQ1.bcfg",1);
 //char *p2=(char*)malloc(1024*100);
// strcpy(p2,buf2);
// char *p2=buf2;
 int n=size2;
 CFG_HDR *hp2;
 unsigned int i;

 while(n>=sizeof(CFG_HDR))
  {
    hp2=(CFG_HDR*)p2;
    n-=sizeof(CFG_HDR);
    p2+=sizeof(CFG_HDR);
    switch(hp2->type)
    {
    case CFG_UINT:
      n-=sizeof(unsigned int);
      if (n<0)
      {
      L_ERRCONSTR:
          Form1->ListBox3->Items->Add("Unexpected EOF!!!");
      L_ERRCONSTR1:
      }
      if(type==CFG_UINT)
      {
        if( *((unsigned int *)p) != *((unsigned int *)p2) && !strcmp(hp->name,hp2->name))
        {
            *((unsigned int *)p) = *((unsigned int *)p2);
            if(*((unsigned int *)p) > hp->max) *((unsigned int *)p) = hp->max;
       //     Form1->ListBox3->Items->Add(hp->name);
       //     Form1->ListBox3->Items->Add(IntToStr(*((unsigned int *)p)));
        }
       }
      p2+=sizeof(unsigned int);
      break;
   case CFG_INT:
      n-=sizeof(int);
      if (n<0) goto L_ERRCONSTR;
      //{
       // int k1,k2;
       // unsigned int min,max;
       // min=(k1=hp->min)>=0?k1:(-k1)*10;
       // max=(k2=hp->max)>=0?k2:(-k2)*10;
      //}
       if(type==CFG_INT)
       {
        if( *((int*)p) != *((int*)p2) && !strcmp(hp->name,hp2->name) )
        {
             *((int*)p) = *((int*)p2);
       //     Form1->ListBox3->Items->Add(hp->name);
       //     Form1->ListBox3->Items->Add( IntToStr( *((int*)p)) );
        }
        }
      p2+=sizeof(int);
      break;
    case CFG_STR_UTF8:
      n-=(hp2->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR;
      if(type==CFG_STR_UTF8)
      {
        if(strcmp((char*)p,(char*)p2)!=0 && !strcmp(hp->name,hp2->name))
         {
          memcpy(p,p2,(hp2->max+1+3)&(~3));
       //   Form1->ListBox3->Items->Add(hp->name);
       //   Form1->ListBox3->Items->Add((char*)p);
        }
      }
      p2+=(hp2->max+1+3)&(~3);
      break;
    case CFG_STR_WIN1251:
      n-=(hp2->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR;
      if(type==CFG_STR_WIN1251)
      {
        if(strcmp((char*)p,(char*)p2)!=0 && !strcmp(hp->name,hp2->name))
         {
          memcpy(p,p2,(hp2->max+1+3)&(~3));
        //  Form1->ListBox3->Items->Add(hp->name);
         // Form1->ListBox3->Items->Add((char*)p);
        }
      }
      p2+=(hp2->max+1+3)&(~3);
      break;

    case CFG_UTF8_STRING:
      n-=(hp2->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR;

      if(type==CFG_UTF8_STRING)
      {
        if(strcmp((char*)p,(char*)p2)!=0 && !strcmp(hp->name,hp2->name))
         {
          memcpy(p,p2,(hp2->max+1+3)&(~3));
      //    Form1->ListBox3->Items->Add(hp->name);
       //   Form1->ListBox3->Items->Add((char*)p);
        }
      }
      p2+=(hp2->max+1+3)&(~3);
      break;

    case CFG_CBOX:
      n-=hp2->max*sizeof(CFG_CBOX_ITEM)+4;
      if (n<0) goto L_ERRCONSTR;
      if(type==CFG_CBOX)
      {
        i=*((int *)p2);//текущий элемент комбобокс
        if (i>=hp2->max)
        {
          ShowMessage("Bad index in combobox!!!");
          goto L_ERRCONSTR1;
        }
        if(*((int *)p2)  != *((int *)p)  && !strcmp(hp->name,hp2->name))
        {
          *((int *)p) = *((int *)p2) ;
       //   Form1->ListBox3->Items->Add(hp->name);
       //   Form1->ListBox3->Items->Add(IntToStr(hp2->max));
        }
      }
      p2+=hp2->max*sizeof(CFG_CBOX_ITEM)+4;
      break;
    case CFG_STR_PASS:
      n-=(hp2->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR;
      if(type==CFG_STR_PASS)
      {
        if(strcmp((char*)p,(char*)p2)!=0 && !strcmp(hp->name,hp2->name))
         {
          memcpy(p,p2,(hp2->max+1+3)&(~3));
       //   Form1->ListBox3->Items->Add(hp->name);
       //   Form1->ListBox3->Items->Add((char*)p);
        }
      }
      p2+=(hp2->max+1+3)&(~3);
      break;
    case CFG_COORDINATES:
      n-=8;
      if (n<0) goto L_ERRCONSTR;
      /*
      ListBox1->Items->Add(hp->name);
      ListBox1->Items->Add(hp->name);
      ListBox2->Items->Add("x: " + IntToStr(*(int*)p));
      ListBox2->Items->Add("y: " + IntToStr(*((int *)p+1)));
        */
        if(type==CFG_COORDINATES)
        {
          if( ( (*(int*)p != *(int*)p2) || (*(int*)p+1 != *(int*)p2+1) ) && !strcmp(hp->name,hp2->name))
          {
            *(int*)p = *(int*)p2;
//            *(int*)p+1 = *(int*)p2+1;
            *((int *)p+1) = *((int *)p2+1);

            //Form1->ListBox3->Items->Add(hp->name);
           // Form1->ListBox3->Items->Add(hp->name);
         //   Form1->ListBox3->Items->Add("x: " + IntToStr(*(int*)p));
       //     Form1->ListBox3->Items->Add("y: " + IntToStr(*((int *)p+1)));
          }
        }
      p2+=8;
      break;
    case CFG_COLOR:
      n-=4;
      if (n<0) goto L_ERRCONSTR;
     /* ListBox1->Items->Add(hp->name);
      ListBox1->Items->Add(hp->name);
      ListBox1->Items->Add(hp->name);
      ListBox1->Items->Add(hp->name);
         char *endptr;

      ListBox2->Items->Add(*((char *)p));
      ListBox2->Items->Add(*((char *)p+1));
      ListBox2->Items->Add(*((char *)p+2));
      ListBox2->Items->Add(*((char *)p+3));*/

//      if( ( *((char *)p)!=*((char *)p2/*strcmp(*((char *)p),*((char *)p2) )*/ || ( strcmp(*((char *)p+1),*((char *)p2+1) ) ||
  //          ( strcmp(*((char *)p+2),*((char *)p2+2) ) || ( strcmp(*((char *)p+3),*((char *)p2+3) ) )
        if(( *((char *)p)!=*((char *)p2) || *((char *)p+1)!=*((char *)p2+1) ||
             *((char *)p+2)!=*((char *)p2+2) || *((char *)p+3)!=*((char *)p2+3)
            )&& !strcmp(hp->name,hp2->name) )
        {
           *((char *)p)   = *((char *)p2);
           *((char *)p+1) = *((char *)p2+1);
           *((char *)p+2) = *((char *)p2+2);
           *((char *)p+3) = *((char *)p2+3);
        }
      p2+=4;
      break;
    case CFG_LEVEL:
      if (n<0) goto L_ERRCONSTR;
   /*   if(hp->min)
      {
         char s[128];
         sprintf(s,"-- level: %s -- ",hp->name);
//         ListBox1->Font->Color = clRed;
         ListBox1->Items->Add(s);
         ListBox2->Items->Add("-- level --");
      }
      else
      {
         ListBox1->Items->Add("-- level close --");
         ListBox2->Items->Add("-- level close --");
      }     */

      continue;
    case CFG_CHECKBOX:
      n-=4;
      if (n<0) goto L_ERRCONSTR;
     /* ListBox1->Items->Add(hp->name);
      ListBox2->Items->Add(*((int *)p) ? "On" : "Off"); */
      if(type==CFG_CHECKBOX)
      {
        if(  *((int *)p) != *((int *)p2) &&   !strcmp(hp->name,hp2->name) )
        {
           *((int *)p) = *((int *)p2);
        }
      }
      p2+=4;
      break;
    case CFG_TIME:
      n-=sizeof(TTime2);
      if (n<0) goto L_ERRCONSTR;

      TTime2 *t,*t2;
      t=(TTime2*)p;
      t2=(TTime2*)p2;
     /* ListBox1->Items->Add(hp->name);
      ListBox1->Items->Add(hp->name);
      ListBox2->Items->Add(IntToStr(tt->hour));
      ListBox2->Items->Add(IntToStr(tt->min)); */
      if(type==CFG_TIME)
      {
        if( (t->min != t2->min || t->hour != t2->hour) && !strcmp(hp->name,hp2->name) )
        {
          t->min  = t2->min;
          t->hour = t2->hour;
        }
      }
      p2+=sizeof(TTime2);
      break;
    case CFG_DATE:
      n-=sizeof(TDate2);
      if (n<0) goto L_ERRCONSTR;

      TDate2 *d,*d2;
      d=(TDate2 *)p;
      d2=(TDate2 *)p2;
      if(type==CFG_DATE)
      {
        if( ( (d->year != d2->year) || (d->month != d2->month) || (d->day != d2->day) ) &&  !strcmp(hp->name,hp2->name))
        {
         d->year = d2->year;
         d->month = d2->month;
         d->day = d2->day;
        }
      }
      p2+=sizeof(TDate2);
      break;

    case CFG_RECT:
      n-=sizeof(MYRECT);
      if (n<0) goto L_ERRCONSTR;

      MYRECT *rc,*rc2;
      rc =(MYRECT *)p;
      rc2=(MYRECT *)p2;
      if(type==CFG_RECT)
      {
        if( (rc->x!=rc2->x || rc->x2!=rc2->x2 || rc->y!=rc2->y || rc->y2!=rc2->y2) && !strcmp(hp->name,hp2->name) )
        {
          rc->x  = rc2->x;
          rc->x2 = rc2->x2;
          rc->y  = rc2->y;
          rc->y2 = rc2->y2;

          Form1->ListBox3->Items->Add(hp->name);
          Form1->ListBox3->Items->Add("x: " + IntToStr( rc->x ));
          Form1->ListBox3->Items->Add("y: " + IntToStr( rc->y ));
          Form1->ListBox3->Items->Add("x2: " + IntToStr( rc2->x ));
          Form1->ListBox3->Items->Add("y2: " + IntToStr( rc2->y ));
        }
      }
      p2+=sizeof(MYRECT);
      break;

    default:
      Form1->ListBox1->Items->Add("Unsupported item");
      Form1->ListBox2->Items->Add("Unsupported item");
      goto L_ENDCONSTR;
    }
  }
L_ENDCONSTR:
}

void __fastcall TForm1::Parser()
{
  char *p = OpenFile("NATICQ.bcfg",0);
  OpenFile("NATICQ1.bcfg",1);
  int n=size;
//  char *p = buf;
  CFG_HDR *hp;

  total_items=0;
  unsigned int i;

  ListBox1->Items->Add("Starting");
  ListBox1->Items->Add("Size = " + IntToStr(size) );
  ListBox2->Items->Add("Starting");
  ListBox2->Items->Add("Size = " + IntToStr(size) );

  while(n>=sizeof(CFG_HDR))
  {
    hp=(CFG_HDR*)p;
    cfg_h[total_items]=hp;
    n-=sizeof(CFG_HDR);
    p+=sizeof(CFG_HDR);
    switch(hp->type)
    {
    case CFG_UINT:
      n-=sizeof(unsigned int);
      if (n<0)
      {
      L_ERRCONSTR:
          ListBox1->Items->Add("Unexpected EOF!!!");
      L_ERRCONSTR1:
      }

      ListBox1->Items->Add(hp->name);
      ListBox2->Items->Add(IntToStr( *((unsigned int *)p) )  );

      Compare((void *)p,hp,CFG_UINT);

      p+=sizeof(unsigned int);
      break;
   case CFG_INT:
      n-=sizeof(int);
      if (n<0) goto L_ERRCONSTR;
      //{
       // int k1,k2;
       // unsigned int min,max;
       // min=(k1=hp->min)>=0?k1:(-k1)*10;
       // max=(k2=hp->max)>=0?k2:(-k2)*10;
        ListBox1->Items->Add(hp->name);
        ListBox2->Items->Add(IntToStr(*((int*)p)) );
      //}
      Compare((void *)p,hp,CFG_INT);
      p+=sizeof(int);
      break;
    case CFG_STR_UTF8:
      n-=(hp->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR;

      ListBox1->Items->Add(hp->name);
      ListBox2->Items->Add(p);
      Compare((void *)p,hp,CFG_STR_UTF8);
      p+=(hp->max+1+3)&(~3);
      break;
    case CFG_STR_WIN1251:
      n-=(hp->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR;

      ListBox1->Items->Add(hp->name);
      ListBox2->Items->Add(p);
      Compare((void *)p,hp,CFG_STR_WIN1251);
      p+=(hp->max+1+3)&(~3);
      break;

    case CFG_UTF8_STRING:
      n-=(hp->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR;

      ListBox1->Items->Add(IntToStr(hp->max));
      Compare((void *)p,hp,CFG_UTF8_STRING);
      p+=(hp->max+1+3)&(~3);
      break;

    case CFG_CBOX:
      n-=hp->max*sizeof(CFG_CBOX_ITEM)+4;
      if (n<0) goto L_ERRCONSTR;
      i=*((int *)p);//текущий элемент комбобокс
      if (i>=hp->max)
      {
        ShowMessage("Bad index in combobox!!!");
        goto L_ERRCONSTR1;
      }
//      ListBox1->Items->Add("CBOX!!!!!!!!!!!!"/*hp->name*/);
       for(int j=0;j<hp->max;j++)
       {
         char s[128];
         sprintf(s,"    %s %d.CBOX!!! :%s",hp->name,j+1,p + j*sizeof(CFG_CBOX_ITEM)+4);
         ListBox1->Items->Add(s);
       }
//     ((CFG_CBOX_ITEM*)((char *)hp+sizeof(CFG_HDR)+4))+(j-1);
      ListBox2->Items->Add( IntToStr( ((CFG_CBOX_ITEM*)(p+4))+i)  );
      Compare((void *)p,hp,CFG_CBOX);
      p+=hp->max*sizeof(CFG_CBOX_ITEM)+4;
      break;

    case CFG_STR_PASS:
      n-=(hp->max+1+3)&(~3);
      if (n<0) goto L_ERRCONSTR;

      ListBox1->Items->Add(hp->name);
      ListBox2->Items->Add(p);

      Compare((void *)p,hp,CFG_STR_PASS);
      p+=(hp->max+1+3)&(~3);
      break;
    case CFG_COORDINATES:
      n-=8;
      if (n<0) goto L_ERRCONSTR;

      ListBox1->Items->Add(hp->name);
      ListBox1->Items->Add(hp->name);
      ListBox2->Items->Add("x: " + IntToStr(*(int*)p));
      ListBox2->Items->Add("y: " + IntToStr(*((int *)p+1)));

      Compare((void *)p,hp,CFG_COORDINATES);
      p+=8;
      break;
    case CFG_COLOR:
      n-=4;
      if (n<0) goto L_ERRCONSTR;
      ListBox1->Items->Add(hp->name);
      ListBox1->Items->Add(hp->name);
      ListBox1->Items->Add(hp->name);
      ListBox1->Items->Add(hp->name);

      ListBox2->Items->Add(*((char *)p));
      ListBox2->Items->Add(*((char *)p+1));
      ListBox2->Items->Add(*((char *)p+2));
      ListBox2->Items->Add(*((char *)p+3));

      Compare((void *)p,hp,CFG_COLOR);
      p+=4;
      break;
    case CFG_LEVEL:
      if (n<0) goto L_ERRCONSTR;
      if(hp->min)
      {
         char s[128];
         sprintf(s,"-- level: %s -- ",hp->name);
//         ListBox1->Font->Color = clRed;
         ListBox1->Items->Add(s);
         ListBox2->Items->Add("-- level --");
      }
      else
      {
         ListBox1->Items->Add("-- level close --");
         ListBox2->Items->Add("-- level close --");
      }

      continue;
    case CFG_CHECKBOX:
      n-=4;
      if (n<0) goto L_ERRCONSTR;
      ListBox1->Items->Add(hp->name);
      ListBox2->Items->Add(*((int *)p) ? "On" : "Off");
      Compare((void *)p,hp,CFG_CHECKBOX);
      p+=4;
      break;
    case CFG_TIME:
      n-=sizeof(TTime2);
      if (n<0) goto L_ERRCONSTR;

      TTime2 *tt;
      tt=(TTime2*)p;

      ListBox1->Items->Add(hp->name);
      ListBox1->Items->Add(hp->name);
      ListBox2->Items->Add(IntToStr(tt->hour));
      ListBox2->Items->Add(IntToStr(tt->min));

      Compare((void *)p,hp,CFG_TIME);
      p+=sizeof(TTime2);
      break;
    case CFG_DATE:
      n-=sizeof(TDate2);
      if (n<0) goto L_ERRCONSTR;

      TDate2 *dd;
      dd=(TDate2 *)p;
      ListBox1->Items->Add(hp->name);
      ListBox2->Items->Add(IntToStr(dd->year));
      Compare((void *)p,hp,CFG_DATE);
      p+=sizeof(TDate2);
      break;

    case CFG_RECT:
      n-=sizeof(MYRECT);
      if (n<0) goto L_ERRCONSTR;

      MYRECT *rc;
      rc=(MYRECT *)p;
      ListBox1->Items->Add(hp->name);
      ListBox2->Items->Add(IntToStr(rc->x));
      Compare((void *)p,hp,CFG_RECT);
      p+=sizeof(MYRECT);
      break;

    default:
      ListBox1->Items->Add("Unsupported item");
      ListBox2->Items->Add("Unsupported item");
      goto L_ENDCONSTR;
    }
    total_items++;
  }
L_ENDCONSTR:
 return;
}

void __fastcall TForm1::FormCreate(TObject *Sender)
{
/* int handle;
 int bytes=0;
 if ((handle = open("NATICQ.bcfg2", O_RDONLY | O_BINARY, S_IWRITE | S_IREAD)) == -1)
   {
      ShowMessage("Error Opening File\n");
      exit(1);
   }

   if ((size = read(handle, buf, 1024*100)) == -1)
   {
      ShowMessage("Read Failed.\n");
      exit(1);
   }
   else
   {
    ListBox1->Items->Add("File read");
    ListBox2->Items->Add("File read");
    // ShowMessage(IntToStr(bytes));
   }
   close(handle);
  */
/*  if ((handle =open("test.bcfg", O_CREAT | O_RDONLY | O_BINARY |   O_RDWR, S_IWRITE | S_IREAD)) == -1)
   {
      ShowMessage("11Error Opening File to write\n");
      exit(1);
   }
   write(handle, buf, size);
   close(handle);*/

 Parser();
}

//---------------------------------------------------------------------------
        
void __fastcall TForm1::Button1Click(TObject *Sender)
{
  int handle;

  if ((handle =open("NATICQ_new.bcfg", O_CREAT | O_RDONLY | O_BINARY |   O_RDWR, S_IWRITE | S_IREAD)) == -1)
   {
      ShowMessage("Error Opening File to write\n");
      exit(1);
   }
   write(handle, buf, size);
   close(handle);
}

//---------------------------------------------------------------------------


void __fastcall TForm1::Button2Click(TObject *Sender)
{
  exit(1);        
}
//---------------------------------------------------------------------------

