/***************************************************************************
 *   Copyright © 2005-2009 by Gabriele Svelto                              *
 *   gabriele.svelto@gmail.com                                             *
 *                                                                         *
 *   This file is part of Jelatine.                                        *
 *                                                                         *
 *   Jelatine is free software: you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   Jelatine is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Jelatine.  If not, see <http://www.gnu.org/licenses/>.     *
 ***************************************************************************/

/** \file main.c
 * main() implementation of the command-line launcher and global data used by
 * the virtual machine */

#include <zzip/zzip.h>

#ifdef NEWSGOLD
#define NO_KILLDATA
#include <swilib.h>
#endif

#include "wrappers.h"

#include "vm.h"
#include "memory.h"

/******************************************************************************
 * Local function declarations                                                *
 ******************************************************************************/

void start_vm( void )
{
  vm_run(opts_get_classpath(), opts_get_boot_classpath(), opts_get_main_class(),
         opts_get_heap_size(), opts_get_jargs_n(), opts_get_jargs());
}

#ifdef NEWSGOLD
extern void sie_start_thread(void (*func)());

void start_vm_thread( void )
{
  sie_start_thread(start_vm);
}
#endif

#ifdef NEWSGOLD
int main(char *exename, char *fname, void *data0, void *data1)
#else
int main(int argc, char *argv[])
#endif
{
  char *args[2];
  args[0] = NULL;
  args[1] = NULL;
  char buf[4096];

#ifdef NEWSGOLD
  int f = open("0:\\stdout.txt", O_RDWR | O_CREAT | O_APPEND);
  __setup_stdout_fd(f);

  args[0] = data0;
  args[1] = data1;
#else
  char *fname = ((argc > 1) ? argv[1] : NULL);
  if (argc > 2)
    args[0] = argv[2];

  if (argc > 3)
    args[0] = argv[3];
#endif

  if (fname == NULL)
    return -1;

  int argcnt = 0;
  if(args[0] != NULL)
    argcnt++;

  if(args[1] != NULL)
    argcnt++;

  // Seek for a main class
  ZZIP_DIR *z = zzip_dir_open(fname, 0);
  if (z == NULL)
  {
    printf("Unable to open file %s\n", fname);
    return -2;
  }

  ZZIP_FILE *mf = zzip_file_open(z, "META-INF/MANIFEST.MF", O_RDONLY);
  if (mf == NULL)
  {
    printf("Can't open MANIFEST.MF\n");
    zzip_dir_close(z);
    return -3;
  }

  memset(buf, 0, 4096);
  int len = zzip_read(mf, buf, 4096);
  zzip_close(mf);
  zzip_dir_close(z);

  if (len <= 0)
  {
    printf("Incorrect MANIFEST.MF\n");
    return -4;
  }

  char *main = strstr(buf, "Main-Class");
  if (main != NULL)
  {
    main = strchr(main, ':');
  }
  if (main == NULL)
  {
    printf("MANIFEST.MF lacks Main-Class attribute\n");
    return -5;
  }
  main++;

  while ((main - buf < 4096) && (*main == ' ')) main++;

  char *begin = main;
  while ((main - buf < 4096) && (*main != ' ') && (*main != '\r') && (*main != '\n') && (*main != 0))
  {
    if (*main == '.')
      *main = '/';
    main++;
  }

  if ((*begin == 0) || (begin - buf == 4096) || (main - begin < 1))
  {
    printf("MANIFEST.MF has invalid Main-Class attribute\n");
    return -6;
  }

  if (*main != 0)
    *main = 0;

  opts_set_main_class(begin);

  char *newfname = (char *)malloc(strlen(fname)+1);
  strcpy(newfname, fname);

  opts_set_classpath(newfname);
  opts_set_jargs_n(argcnt);

  if (argcnt != 0)
    opts_set_jargs(args);
/*
#ifndef NEWSGOLD
  opts_set_print_opcodes(true);
  opts_set_print_methods(true);
#endif
*/
#ifdef NEWSGOLD
  SUBPROC((void *)start_vm_thread);
#else
  start_vm();
#endif

#ifdef NEWSGOLD
  close(f);
#endif
  return 0;
}

