
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <swilib.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <luajit.h>
#include <lj_lib.h>
#include <lj_arch.h>
#include <my_lua.h>
#include <stdarg.h>




void drawImage(int x, int y, const char *pic)
{
    DrawImg(x, y, (unsigned int)pic);
}
