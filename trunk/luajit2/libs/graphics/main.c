
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


#define GRAPHICSHANDLE "graphics"

int guis_cnt = 0;

int exist_guis[128] = {-1};

void push_to_exist_guis(int id)
{
    int i = 0;
    while(exist_guis[i] != -1 && i < 128) ++i;

    if(i == 128) return;

    exist_guis[i] = id;
}


void pop_from_exist_guis(int id)
{
    int i = 0;
    while(exist_guis[i] != id && i < 128) ++i;

    if(i == 128) return;
    exist_guis[i] = -1;
}


char is_gui_exist(int id)
{
    int i = 0;
    while(exist_guis[i] != id && i < 128) ++i;
    if(i == 128) return 0;
    return 1;
}



CSM_RAM* checkcsmram (lua_State *L, int index)
{
    CSM_RAM *im;
    if( !lua_isuserdata(L, index) )
    {
        luaL_error(L, "Given parameter omited");
        return 0;
    }
    im = (CSM_RAM*)lua_unboxpointer(L, index);
    return im;
}



LUA_MAIN_GUI* push_graphics (lua_State *L, LUA_MAIN_GUI *im)
{
    lua_boxpointer(L, im);
    luaL_getmetatable(L, GRAPHICSHANDLE);
    lua_setmetatable(L, -2);
    return im;
}


LUA_MAIN_GUI* check_graphics(lua_State *L, int index)
{
    LUA_MAIN_GUI *im;
    if( !lua_isuserdata(L, index) )
    {
        luaL_error(L, "Given parameter omited");
        return 0;
    }
    im = (LUA_MAIN_GUI*)lua_unboxpointer(L, index);
    return im;
}



void OnRedraw(LUA_MAIN_GUI *data)
{
    lua_State *L = data->L;
    if( mluaState(L)->is_aborting == 1) return;
    if(data->ref[0] > -1)
    {
        lua_rawgeti (L, LUA_REGISTRYINDEX, data->ref[0]);
        int status;
        if( (status = docall(L, 0, 0)) )
        {
            fprintf(stderr, "Calling error\n");
            mluaState(L)->abort(status);
            return;
        }
    }
}


void onCreate(LUA_MAIN_GUI *data, void *(*malloc_adr)(int)  )
{
    if( mluaState(data->L)->is_aborting == 1 ) return;
    data->gui.state=1;
    if(data->ref[1] > -1)
    {
        lua_rawgeti (data->L, LUA_REGISTRYINDEX, data->ref[1]);
        int status;
        if( (status = docall(data->L, 0, 0)) )
        {
            fprintf(stderr, "Calling error\n");
            mluaState(data->L)->abort(status);
            return;
        }
    }

    lua_lock_exit(data->L);
}


void onClose(LUA_MAIN_GUI *data, void (*mfree_adr)(void *)  )
{
    pop_from_exist_guis(data->id);
    --guis_cnt;
    if( mluaState(data->L)->is_aborting == 1 )
    {
        fprintf(stderr, "Aborting, poping gui %d\n", data->id);
        lua_unlock_exit(data->L);
        return;
    }

    data->gui.state=0;
    if(data->ref[2] > -1)
    {
        lua_rawgeti (data->L, LUA_REGISTRYINDEX, data->ref[2]);
        int status;
        if( (status = docall(data->L, 0, 0)) )
        {
            fprintf(stderr, "Calling error\n");
            mluaState(data->L)->abort(status);
        }
    }

    lua_unlock_exit(data->L);
    fprintf(stderr, "Pop gui %d\n", data->id);
}


void onFocus(LUA_MAIN_GUI *data, void *(*malloc_adr)(int) , void (*mfree_adr)(void *) )
{
    if( mluaState(data->L)->is_aborting == 1 ) return;
    data->gui.state=2;
    //DisableIDLETMR();
    if(data->ref[3] > -1)
    {
        lua_rawgeti (data->L, LUA_REGISTRYINDEX, data->ref[3]);
        int status;
        if( (status = docall(data->L, 0, 0)) )
        {
            fprintf(stderr, "Calling error\n");
            mluaState(data->L)->abort(status);
            return;
        }
    }
}


void onUnfocus(LUA_MAIN_GUI *data, void (*mfree_adr)(void *) )
{
    if( mluaState(data->L)->is_aborting == 1 ) return;
    if (data->gui.state!=2) return;
    data->gui.state=1;
    if(data->ref[4] > -1)
    {
        lua_rawgeti (data->L, LUA_REGISTRYINDEX, data->ref[4]);
        int status;
        if( (status = docall(data->L, 0, 0)) )
        {
            fprintf(stderr, "Calling error\n");
            mluaState(data->L)->abort(status);
            return;
        }
    }
}


int OnKey(LUA_MAIN_GUI *data, GUI_MSG *msg)
{
    if(msg->gbsmsg->submess == 0xFDEAD)
    {
        fprintf(stderr, "Calling error onkey\n");
        return 1;
    }


    if( mluaState(data->L)->is_aborting == 1 ) return 1;
    int ret = 0;
    if(data->ref[5] > -1)
    {
        lua_rawgeti (data->L, LUA_REGISTRYINDEX, data->ref[5]);

        lua_pushnumber(data->L, msg->gbsmsg->msg);
        lua_pushnumber(data->L, msg->gbsmsg->submess);
        int status;
        if( (status = docall(data->L, 2, 1)) )
        {
            mluaState(data->L)->abort(status);
            return 0;
        }

        ret = (int)lua_tointeger(data->L, -1);
        lua_pop(data->L, -1);
    }


    return (ret);
}



/** ==================================================*/

int method8(void)
{
    return(0);
}

const RECT Canvas= {0,0,0,0};
const void * const gui_methods[11]=
{
    (void *)OnRedraw,
    (void *)onCreate,
    (void *)onClose,
    (void *)onFocus,
    (void *)onUnfocus,
    (void *)OnKey,
    0,
    (void *)kill_data,
    (void *)method8,
    (void *)method8,
    0
};



int graphics_new (lua_State *L)
{
    LUA_MAIN_GUI *lua_main_gui = malloc(sizeof(LUA_MAIN_GUI));
    void **dat = (void**)malloc(4 * 4);
    dat[0] = lua_main_gui;
    dat[1] = (void *)0;

    zeromem(lua_main_gui,sizeof(LUA_MAIN_GUI));
    lua_main_gui->gui.canvas=(void *)(&Canvas);
    lua_main_gui->gui.methods=(void *)gui_methods;
    lua_main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
    lua_main_gui->L = L;
    lua_main_gui->closing = 0;
    //lua_csm->csm.state=0;
    //lua_csm->csm.unk1=0;

    luaL_checktype(L, 1, LUA_TFUNCTION);
    luaL_checktype(L, 2, LUA_TFUNCTION);
    luaL_checktype(L, 3, LUA_TFUNCTION);
    luaL_checktype(L, 4, LUA_TFUNCTION);
    luaL_checktype(L, 5, LUA_TFUNCTION);
    luaL_checktype(L, 6, LUA_TFUNCTION);

    LUA_MAIN_CSM *cram = 0;

    if(lua_type(L, 7) != LUA_TNONE)
        cram = (LUA_MAIN_CSM*)checkcsmram(L, 7);

    if( mluaState(L)->is_aborting == 1 )
    {
        free(lua_main_gui);
        lua_pushnil(L);
        return 0;
    }

    //lua_pushvalue(L, 6);
    lua_main_gui->ref[5] = luaL_ref(L, LUA_REGISTRYINDEX);

    //lua_pushvalue(L, 5);
    lua_main_gui->ref[4] = luaL_ref(L, LUA_REGISTRYINDEX);

    //lua_pushvalue(L, 4);
    lua_main_gui->ref[3] = luaL_ref(L, LUA_REGISTRYINDEX);

    //lua_pushvalue(L, 3);
    lua_main_gui->ref[2] = luaL_ref(L, LUA_REGISTRYINDEX);

    //lua_pushvalue(L, 2);
    lua_main_gui->ref[1] = luaL_ref(L, LUA_REGISTRYINDEX);

    //lua_pushvalue(L, 1);
    lua_main_gui->ref[0] = luaL_ref(L, LUA_REGISTRYINDEX);


    mluaState(L)->keep_alive = 1;

    lua_main_gui->id = CreateGUI_30or2(lua_main_gui);

    if(cram)
    {
        //cram->gui_id = lua_main_gui->id;
        //cram->csm.state=0;
        //cram->csm.unk1=0;
    }

    dat[1] = (void *)lua_main_gui->id;

    guis_cnt++;
    push_to_exist_guis(lua_main_gui->id);
    push_graphics(L, (LUA_MAIN_GUI*)dat);
    return 1;
}



int graphics_id(lua_State *L)
{
    void **dat = (void**)check_graphics(L, 1);
    LUA_MAIN_GUI *lua_main_gui = (LUA_MAIN_GUI*)dat[0];
    if(!lua_main_gui) return 0;
    lua_pushinteger(L, lua_main_gui->id);
    return 1;
}



int graphics_gc(lua_State *L)
{
    fprintf(stderr, "%s: %s\n", __FILE__, __FUNCTION__);
    void **dat = (void**)check_graphics(L, 1);
    LUA_MAIN_GUI *lua_main_gui = (LUA_MAIN_GUI*)dat[0];

    if(!lua_main_gui) goto end;
    if(!is_gui_exist((int)dat[1]))  goto end;

    if(mluaState(L)->is_aborting)
        memset(lua_main_gui->ref, -1, sizeof(lua_main_gui->ref));

    pop_from_exist_guis(lua_main_gui->id);
    lua_main_gui->closing = 1;
    GeneralFunc_flag1(lua_main_gui->id, 1);

end:
    free(dat);
    return 0;
}



int graphics_width(lua_State *L)
{
    lua_pushinteger(L, ScreenW());
    return 1;
}



int graphics_height(lua_State *L)
{
    lua_pushinteger(L, ScreenH());
    return 1;
}



static const luaL_Reg graphics_lib[] =
{
    {"new", graphics_new},
    {"width", graphics_width},
    {"height", graphics_height},
    {"id", graphics_id},
    {NULL, NULL}
};


static const luaL_Reg graphics_mlib[] =
{
    {"id", graphics_id},
    {"width", graphics_width},
    {"height", graphics_height},
    {"__gc", graphics_gc},
    {NULL, NULL}
};



void registerFont(lua_State *L, int idx, char *name)
{
    lua_pushnumber(L, idx);
    lua_setfield(L, -2, name);
}


static void setConstants(lua_State *L)
{
    lua_pushnumber(L, LEFT_SOFT);
    lua_setfield(L, -2, "LEFT_SOFT");

    lua_pushnumber(L, RIGHT_SOFT);
    lua_setfield(L, -2, "RIGHT_SOFT");

    lua_pushnumber(L, GREEN_BUTTON);
    lua_setfield(L, -2, "GREEN_BUTTON");

    lua_pushnumber(L, RED_BUTTON);
    lua_setfield(L, -2, "RED_BUTTON");

    lua_pushnumber(L, UP_BUTTON);
    lua_setfield(L, -2, "UP_BUTTON");

    lua_pushnumber(L, DOWN_BUTTON);
    lua_setfield(L, -2, "DOWN_BUTTON");

    lua_pushnumber(L, LEFT_BUTTON);
    lua_setfield(L, -2, "LEFT_BUTTON");

    lua_pushnumber(L, RIGHT_BUTTON);
    lua_setfield(L, -2, "RIGHT_BUTTON");

    lua_pushnumber(L, ENTER_BUTTON);
    lua_setfield(L, -2, "ENTER_BUTTON");
}


int graphics_register (lua_State *L)
{
    luaL_newmetatable(L, GRAPHICSHANDLE);  /* create metatable for file handles */
    lua_pushvalue(L, -1);  /* push metatable */
    lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
    luaL_register(L, NULL, graphics_mlib);  /* file methods */
    return 1;
}


LUALIB_API int luaopen_graphics(lua_State *L)
{
    memset(exist_guis, -1, sizeof(exist_guis));
    graphics_register(L);
    luaL_register(L, GRAPHICSHANDLE, graphics_lib);

    setConstants(L);

    registerFont(L, FONT_LARGE_BOLD, "FONT_LARGE_BOLD");
    registerFont(L, FONT_LARGE_ITALIC, "FONT_LARGE_ITALIC");
    registerFont(L, FONT_LARGE_ITALIC_BOLD, "FONT_LARGE_ITALIC_BOLD");
    registerFont(L, FONT_MEDIUM, "FONT_MEDIUM");
    registerFont(L, FONT_MEDIUM_BOLD, "FONT_MEDIUM_BOLD");
    registerFont(L, FONT_MEDIUM_ITALIC, "FONT_MEDIUM_ITALIC");
    registerFont(L, FONT_MEDIUM_ITALIC_BOLD, "FONT_MEDIUM_ITALIC_BOLD");
    registerFont(L, FONT_NUMERIC_SMALL, "FONT_NUMERIC_SMALL");
    registerFont(L, FONT_NUMERIC_SMALL_BOLD, "FONT_NUMERIC_SMALL_BOLD");

    registerFont(L, FONT_NUMERIC_XSMALL, "FONT_NUMERIC_XSMALL");
    registerFont(L, FONT_SMALL, "FONT_SMALL");
    registerFont(L, FONT_SMALL_BOLD, "FONT_SMALL_BOLD");
    registerFont(L, FONT_SMALL_ITALIC, "FONT_SMALL_ITALIC");
    registerFont(L, FONT_SMALL_ITALIC_BOLD, "FONT_SMALL_ITALIC_BOLD");
    registerFont(L, FONT_NUMERIC_LARGE, "FONT_NUMERIC_LARGE");
    registerFont(L, FONT_NUMERIC_MEDIUM, "FONT_NUMERIC_MEDIUM");

    return 1;
}



