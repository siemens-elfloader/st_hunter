

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <swilib.h>
#include <pnglist.h>
#include <nu_swilib.h>


#define LUA_LIB

#include <my_lua.h>
#include "lua.h"
#include "lauxlib.h"
#include "lj_state.h"


#define MAX_CSMS 128
#define LUA_CSMNAME "csm"

typedef struct
{
    CSM_DESC maincsm;
    WSHDR maincsm_name;
    lua_State *L;
    int csm_id;
    signed char refs[3];
    char closing;
} lua_maincsm;

const int minus11 = -11;


#define CSMHANDLE "csm"
#define CSMRAM_HANDLE "csmram"
#define GBSMSG_HANDLE "gbs_msg"

#define toCsmp(L, idx)	((lua_maincsm *)luaL_checkudata(L, idx, CSMHANDLE))
#define toCSMRAMp(L, idx)	((CSM_RAM *)luaL_checkudata(L, idx, CSMRAM_HANDLE))
#define toGBS_MSGp(L, idx) ((GBS_MSG *)luaL_checkudata(L, idx, GBSMSG_HANDLE))


int __lua_csm_database[MAX_CSMS]= {-1};
int csm_cnt = 0;

/* ============ csm database ========== */

int lua_csm_push(lua_State *L, int data)
{
    int i;
    for(i=0; i<MAX_CSMS; ++i)
    {
        if(__lua_csm_database[i] == -1) break;
    }

    if( i >= MAX_CSMS )
    {
        fprintf(stderr, "lua: can`t create a new csm\n");
        return -1;
    }
    ++csm_cnt;
    __lua_csm_database[i] = (int)data;
    lua_lock_exit(L);
    return i;
}


void lua_csm_pop(lua_State *L, int data)
{
    int i;
    for(i=0; i<MAX_CSMS; ++i)
    {
        if( __lua_csm_database[i] == data ) break;
    }

    __lua_csm_database[i] = -1;
    --csm_cnt;


    lua_unlock_exit(L);
}


int is_csm_exist(int csm)
{
    int i;
    for(i=0; i<MAX_CSMS; ++i)
    {
        if( __lua_csm_database[i] == csm ) return 1;
    }

    return 0;
}


void free_csm_desk(lua_maincsm *csm)
{
    free(csm->maincsm_name.wsbody);
    free(csm);
}


/* ======
================================================= */


CSM_RAM* pushcsmram (lua_State *L, CSM_RAM *im)
{
    lua_boxpointer(L, im);
    luaL_getmetatable(L, CSMRAM_HANDLE);
    lua_setmetatable(L, -2);
    return im;
}


CSM_RAM* checkcsmram (lua_State *L, int index)
{
    CSM_RAM *im;
    if( !lua_isuserdata(L, index) ){
        luaL_error(L, "Given parameter omited");
        return 0;
    }
    im = (CSM_RAM*)lua_unboxpointer(L, index);
    return im;
}

/* ======================== GBS_MSG ======================*/
GBS_MSG *push_gbsmsg(lua_State *L, GBS_MSG *m)
{
    lua_boxpointer(L, m);
    luaL_getmetatable(L, GBSMSG_HANDLE);
    lua_setmetatable(L, -2);
    return m;
}

GBS_MSG* check_gbsmsg(lua_State *L, int index)
{
    GBS_MSG *im;
    if( !lua_isuserdata(L, index) ){
        luaL_error(L, "Given parameter omited");
        return 0;
    }
    im = (GBS_MSG*)lua_unboxpointer(L, index);
    return im;
}

/* ======================== LUA_MAINCSM ===================== */
lua_maincsm* pushcms (lua_State *L, lua_maincsm *im)
{
    lua_boxpointer(L, im);
    luaL_getmetatable(L, CSMHANDLE);
    lua_setmetatable(L, -2);
    return im;
}


lua_maincsm* checkcms(lua_State *L, int index)
{
    lua_maincsm *im;
    if( !lua_isuserdata(L, index) ){
        luaL_error(L, "Given parameter omited");
        return 0;
    }
    im = (lua_maincsm*)lua_unboxpointer(L, index);
    return im;
}


/* ========================================================== */

static void lua_maincsm_oncreate(CSM_RAM *data)
{
    lua_maincsm *lua_csm = (lua_maincsm*)data->constr;
    if(mluaState(lua_csm->L)->is_aborting == 1) return;

    ((LUA_MAIN_CSM*)lua_csm)->csm.state=0;
    ((LUA_MAIN_CSM*)lua_csm)->csm.unk1=0;


    if(lua_csm->refs[0] > -1)
    {
        lua_rawgeti (lua_csm->L, LUA_REGISTRYINDEX, lua_csm->refs[0]);
        pushcsmram(lua_csm->L, data);
        int status;
        if( (status = docall(lua_csm->L, 1, 0)) )
        {
            mluaState(lua_csm->L)->abort(status);
            return;
        }
    }
}


static void lua_maincsm_onclose(CSM_RAM *data)
{
    fprintf(stderr, "%s\n", __FUNCTION__);
    lua_maincsm *lua_csm = (lua_maincsm*)data->constr;
    lua_State *L = lua_csm->L;

    if(mluaState(lua_csm->L)->is_aborting == 1)
    {
        lua_csm_pop(L, lua_csm->csm_id);
        free_csm_desk(lua_csm);
        return;
    }

    if(lua_csm->refs[1] > -1)
    {
        lua_rawgeti (lua_csm->L, LUA_REGISTRYINDEX, lua_csm->refs[1]);
        pushcsmram(lua_csm->L, data);
        int status;
        if( (status = docall(lua_csm->L, 1, 0)) )
        {
            lua_csm_pop(L, lua_csm->csm_id);
            mluaState(lua_csm->L)->abort(status);
            free_csm_desk(lua_csm);
            return;
        }
    }
    lua_csm_pop(L, lua_csm->csm_id);
    free_csm_desk(lua_csm);
}


int lua_maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
    lua_maincsm *lua_csm = (lua_maincsm*)data->constr;

    LUA_MAIN_CSM *csm=(LUA_MAIN_CSM*)data;
    if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
    {
        fprintf(stderr, "%s: destroy!!!!!!!!!!!!!\n\n", __FUNCTION__);
        csm->csm.state = -3;
    }

    if(mluaState(lua_csm->L)->is_aborting == 1) return 1;

    int ret = 1;
    if(lua_csm->refs[2] > -1)
    {
        lua_rawgeti (lua_csm->L, LUA_REGISTRYINDEX, lua_csm->refs[2]);
        lua_pushlightuserdata(lua_csm->L, data);
        lua_pushlightuserdata(lua_csm->L, msg);
        int status;
        if( (status = docall(lua_csm->L, 2, 1)) )
        {
            mluaState(lua_csm->L)->abort(status);
            return 0;
        }
        ret = lua_tonumber(lua_csm->L, -1);
        lua_pop(lua_csm->L, 1);
    }

    return (ret);
}



void init_csmdesc(CSM_DESC *desc)
{
    desc->onMessage = lua_maincsm_onmessage;
    desc->onCreate  = lua_maincsm_oncreate;
#ifdef NEWSGOLD
    desc->zero1     = 0;
    desc->zero2     = 0;
    desc->zero3     = 0;
    desc->zero4     = 0;
#endif
    desc->onClose   = lua_maincsm_onclose;
    desc->datasize  = sizeof(LUA_MAIN_CSM);
    desc->statesize = 1;
    desc->states    = &minus11;
}


void init_csmname(WSHDR *ws, const char *name)
{
    ws->wsbody = (unsigned short*)malloc(139*sizeof(unsigned short));
    ws->ws_malloc = NAMECSM_MAGIC1;
    ws->ws_mfree = NAMECSM_MAGIC2;
    ws->isbody_allocated = 0;
    ws->maxlen = 139;
    ws->unk1  = 0;
    wsprintf(ws, "%t", name);
}


/*******************************************************************************/
/*******************************************************************************/

static int cms_new(lua_State *L)
{
    mluaState(L)->keep_alive = 1;
    const int type = luaL_checkint(L, 1);
    const char *name = luaL_checkstring(L, 2);

    luaL_checktype(L, 3, LUA_TFUNCTION);
    luaL_checktype(L, 4, LUA_TFUNCTION);
    luaL_checktype(L, 5, LUA_TFUNCTION);

    LUA_MAIN_CSM main_csm;


    lua_maincsm *mcsm = malloc(sizeof(lua_maincsm));
    mcsm->csm_id = -1;
    mcsm->L = L;
    mcsm->closing = 0;
    memset(mcsm->refs, -1, sizeof(mcsm->refs));


    lua_pushvalue(L, 5);
    mcsm->refs[2] = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_pushvalue(L, 4);
    mcsm->refs[1] = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_pushvalue(L, 3);
    mcsm->refs[0] = luaL_ref(L, LUA_REGISTRYINDEX);

    init_csmdesc(&mcsm->maincsm);
    init_csmname(&mcsm->maincsm_name, name);


    LockSched();
    if(type)
        mcsm->csm_id = CreateCSM((const CSM_DESC*)mcsm, &main_csm, 0);
    else
    {
        CSM_RAM *save_cmpc;
        save_cmpc=CSM_root()->csm_q->current_msg_processing_csm;
        CSM_root()->csm_q->current_msg_processing_csm=CSM_root()->csm_q->csm.first;
        mcsm->csm_id = CreateCSM((const CSM_DESC*)mcsm, &main_csm, 0);
        CSM_root()->csm_q->current_msg_processing_csm=save_cmpc;
    }
    UnlockSched();

    void **ud = (void **)malloc(8);
    ud[0] = (void *)mcsm;
    ud[1] = (void *)mcsm->csm_id;
    pushcms(L, (lua_maincsm*)ud);
    lua_csm_push(L, mcsm->csm_id);
    return 1;
}




void subclosecsm(int csm)
{
    CloseCSM(csm);
}


static int csm_free(lua_State *L)
{
    void **u = (void**)checkcms(L, 1);
    lua_maincsm * csm = u[0];
    int id = (int)u[1];

    if(is_csm_exist(id)){
        fprintf(stderr, "Closing csm\n");
        fflush(stdout);
        mluaState(L)->RunInLuaProc(subclosecsm, (void*)id);
    }

    free(u);
    return 0;
}


static int csm_parentOfGui(lua_State *L)
{
    LUA_MAIN_CSM *cram = (LUA_MAIN_CSM*)checkcsmram(L, 1);
    int id = luaL_checkinteger(L, 2);
    cram->gui_id = id;
    return 0;
}


static int csm_isIdle(lua_State *L)
{
#define idlegui_id(icsm) (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])
    CSM_RAM *icsm = FindCSMbyID(CSM_root()->idle_id);

    if ( IsGuiOnTop(idlegui_id(icsm)) )
    {
        lua_pushboolean(L, 1);
        return 1;
    }

    lua_pushboolean(L, 0);
    return 1;
}


static int csm_isIPCMsg(lua_State *L)
{
    GBS_MSG * msg = check_gbsmsg (L, 1);
    if (msg->msg == MSG_IPC)
    {
        lua_pushboolean(L, 1);
        return 1;
    }

    lua_pushboolean(L, 0);
    return 1;
}


static int csm_ipc_nameto(lua_State *L)
{
    GBS_MSG * msg = check_gbsmsg (L, 1);
    if (msg->msg == MSG_IPC)
    {
        IPC_REQ *ipc=(IPC_REQ*)((msg)->data0);
        if (ipc)
        {
            lua_pushstring(L, ipc->name_to);
            return 1;
        }
    }

    lua_pushnil(L);
    return 1;
}


static int csm_ipc_namefrom(lua_State *L)
{
    GBS_MSG * msg = check_gbsmsg (L, 1);
    if (msg->msg == MSG_IPC)
    {
        IPC_REQ *ipc=(IPC_REQ*)((msg)->data0);
        if (ipc)
        {
            lua_pushstring(L, ipc->name_from);
            return 1;
        }
    }
    lua_pushnil(L);
    return 1;
}


static const luaL_Reg  csmlib[] =
{
    {"new", cms_new},
    //{"free", csm_free},
    {"parentOfGui", csm_parentOfGui},
    {"isIdle", csm_isIdle},
    {"isIPCMsg", csm_isIPCMsg},
    {"ipc_nameto", csm_ipc_nameto},
    {"ipc_namefrom", csm_ipc_namefrom},
    {NULL, NULL}
};


static const luaL_Reg  cmslib_meta[] =
{
    {"parentOfGui", csm_parentOfGui},
    {"isIdle", csm_isIdle},
    {"isIPCMsg", csm_isIPCMsg},
    {"ipc_nameto", csm_ipc_nameto},
    {"ipc_namefrom", csm_ipc_namefrom},

    {"__gc", csm_free},
    {NULL, NULL}
};


int csm_register (lua_State *L)
{
    luaL_newmetatable(L, CSMHANDLE);  /* create metatable for file handles */
    lua_pushvalue(L, -1);  /* push metatable */
    lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
    luaL_register(L, NULL, cmslib_meta);  /* file methods */

    return 1;
}

LUALIB_API int luaopen_csm (lua_State *L)
{
    memset(__lua_csm_database, -1, (sizeof(__lua_csm_database)));
    csm_register(L);
    luaL_register(L, LUA_CSMNAME, csmlib);
    return 1;
}
