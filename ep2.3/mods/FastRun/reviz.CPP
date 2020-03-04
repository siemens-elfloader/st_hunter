#include <iostream.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
void main()
{
  int i;
  FILE *f;
  if((f=fopen("reviz.h","r"))!=NULL)
  {
	fscanf(f,"#ifndef _REVIZ_H_\n#define _REVIZ_H_\nint reviz=%03d;\n#endif\n",&i);
	i++;
	unlink("reviz.h");
	fclose(f);
  }
  if((f=fopen("reviz.h","w+"))!=NULL)
  {
	fprintf(f,"#ifndef _REVIZ_H_\n#define _REVIZ_H_\nint reviz=%03d;\n#endif\n",i);
   fclose(f);
  }
}