


typedef struct
{
    lua_State *L;
    int type[24];
    char *data[24];
    int nums;
    int ref;
} lua_param;


void __lua_subproc( lua_param *p)
{
    lua_rawgeti (p->L, LUA_REGISTRYINDEX, p->ref);

    for(int i = 0; i < p->nums; ++i)
    {
        switch ( p->type[i] )
        {
        case  LUA_TNIL:
            lua_pushnil(p->L);
            break;

        case  LUA_TBOOLEAN:
            lua_pushboolean(p->L, (int)p->data[i]);
            break;

        case LUA_TNUMBER:
            lua_pushnumber(p->L, ( int )p->data[i]);
            break;

        case  LUA_TSTRING:
            lua_pushstring(p->L, (const char*)p->data[i]);
            break;

        case  LUA_TUSERDATA:
            lua_pushlightuserdata(p->L, (void*)p->data[i]);
            break;
        }
    }

    int status;
    if( (status = docall(p->L, p->nums, 0)) )
    {
        mluaState(p->L)->abort(status);
        mfree(p);
        return ;
    }
    //lua_pop(p->L, 1);
    mfree(p);
    return;
}



static int core_subproc (lua_State *L)
{
    mluaState(L)->keep_alive = 1;

    luaL_checktype(L, 1, LUA_TFUNCTION);
    lua_param *p = (lua_param*)malloc(sizeof(lua_param));

    int top = lua_gettop(L);
    //lua_settop(L, 1);
    lua_pushvalue(L, 1);
    p->ref = luaL_ref (L, LUA_REGISTRYINDEX );

    lua_settop(L, top);

    printf("TOP: %d - %d\n", top, lua_gettop(L));

    int pn = 0;
    char brk = 0;
    while(1)
    {
        switch (lua_type(L, pn+2) )
        {
        case  LUA_TNIL:
            printf("LUA_TNIL\n");
            p->type[pn] =  LUA_TNIL;
            p->data[pn] = 0;
            break;

        case  LUA_TBOOLEAN:
            printf("LUA_TBOOLEAN\n");
            p->type[pn] =  LUA_TBOOLEAN;
            p->data[pn] = (void*)lua_toboolean(L, pn+2);
            break;

        case LUA_TNUMBER:
            printf("LUA_TNUMBER\n");
            p->type[pn] = LUA_TNUMBER;
            p->data[pn] = (void*)lua_tointeger(L, pn+2);
            break;

        case  LUA_TSTRING:
            printf("LUA_TSTRING\n");
            p->type[pn] =  LUA_TSTRING;
            p->data[pn] = (void*)lua_tostring(L, pn+2);
            break;

        case  LUA_TUSERDATA:
            printf("LUA_TUSERDATA\n");
            p->type[pn] =  LUA_TUSERDATA;
            p->data[pn] = (void*)lua_touserdata(L, pn+2);
            break;

        case  LUA_TNONE:
            printf("LUA_TNONE\n");
            brk  = 1;
            break;

        }

        if( brk ) break;
        ++pn;
    }

    p->L = L;
    p->nums = pn;
    SUBPROC((void*)__lua_subproc, (void*)p);
    return 0;
}
