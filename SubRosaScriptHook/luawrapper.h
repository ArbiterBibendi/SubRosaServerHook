#pragma once
#include "lua/lua.hpp"
#include <Windows.h>
#include <iostream>
#include <string>

lua_State* l;

/* Import function addresses from lua's DLL  */
typedef lua_State* (*dluaL_newstate_t)(void);
dluaL_newstate_t dluaL_newstate;

typedef void (*dluaL_openlibs_t)(lua_State* L);
dluaL_openlibs_t dluaL_openlibs;

typedef int (*dluaL_loadfile_t)(lua_State* L, const char* filename);
dluaL_loadfile_t dluaL_loadfile;

typedef int (*dlua_pcall_t)(lua_State* L, int nargs, int nresults, int errfunc);
dlua_pcall_t dlua_pcall;

typedef void (*dlua_close_t)(lua_State* L);
dlua_close_t dlua_close;

typedef lua_Number(*dlua_tonumber_t) (lua_State* L, int idx);
dlua_tonumber_t dlua_tonumber;


typedef const char* (*dlua_tolstring_t)(lua_State* L, int idx, size_t* len);
dlua_tolstring_t dlua_tolstring;
#define dlua_tostring(L,i)	dlua_tolstring(L, (i), NULL)

typedef void  (*dlua_pushcclosure_t)(lua_State* L, lua_CFunction fn, int n);
dlua_pushcclosure_t dlua_pushcclosure;
#define dlua_pushcfunction(L,f)	dlua_pushcclosure(L, (f), 0)

typedef void  (*dlua_setfield_t)(lua_State* L, int idx, const char* k);
dlua_setfield_t dlua_setfield;
#define dlua_setglobal(L,s)	dlua_setfield(L, LUA_GLOBALSINDEX, (s))
#define dlua_register(L,n,f) (dlua_pushcfunction(L, (f)), dlua_setglobal(L, (n)))

typedef void  (*dlua_getfield_t)(lua_State* L, int idx, const char* k);
dlua_getfield_t dlua_getfield;
#define dlua_getglobal(L,s)	dlua_getfield(L, LUA_GLOBALSINDEX, (s))
void loadLua() {
    HMODULE luaModule = LoadLibrary(L"lua51.dll");
    if (luaModule) {
        dluaL_newstate = (dluaL_newstate_t)GetProcAddress(luaModule, "luaL_newstate");
        dluaL_openlibs = (dluaL_openlibs_t)GetProcAddress(luaModule, "luaL_openlibs");
        dluaL_loadfile = (dluaL_loadfile_t)GetProcAddress(luaModule, "luaL_loadfile");
        dlua_pcall = (dlua_pcall_t)GetProcAddress(luaModule, "lua_pcall");
        dlua_close = (dlua_close_t)GetProcAddress(luaModule, "lua_close");
        dlua_tonumber = (dlua_tonumber_t)GetProcAddress(luaModule, "lua_tonumber");
        dlua_tolstring = (dlua_tolstring_t)GetProcAddress(luaModule, "lua_tolstring");
        dlua_pushcclosure = (dlua_pushcclosure_t)GetProcAddress(luaModule, "lua_pushcclosure");
        dlua_setfield = (dlua_setfield_t)GetProcAddress(luaModule, "lua_setfield");
        dlua_getfield = (dlua_getfield_t)GetProcAddress(luaModule, "lua_getfield");

        std::cout << dluaL_newstate << std::endl;
        std::cout << dluaL_openlibs << std::endl;
        std::cout << dluaL_loadfile << std::endl;
        std::cout << dlua_pcall << std::endl;
        std::cout << dlua_close << std::endl;
        std::cout << dlua_tonumber << std::endl;
        std::cout << dlua_tolstring << std::endl;
        std::cout << dlua_pushcclosure << std::endl;
        std::cout << dlua_setfield << std::endl;
        std::cout << dlua_getfield << std::endl;
    }
    else {
        std::cout << GetLastError() << std::endl;
    }
}

/* Register C functions in lua */
static int l_SpawnVehicle(lua_State* L) { //SpawnVehicle(int ID, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, int color)
    int ID;
    vec3 pos;
    rotation rot;
    int color;
    ID = dlua_tonumber(L, 1);
    pos.x = dlua_tonumber(L, 2);
    pos.y = dlua_tonumber(L, 3);
    pos.z = dlua_tonumber(L, 4);
    rot.x = dlua_tonumber(L, 5);
    rot.y = dlua_tonumber(L, 6);
    rot.z = dlua_tonumber(L, 7);
    color = dlua_tonumber(L, 8);
    SpawnVehicle(ID, &pos, 0, &rot, color);
    return 0;
}
static int l_test(lua_State* L) {
    int ID = dlua_tonumber(L, 1);
    std::cout << "LUA TEST FUNCTION " << ID << std::endl;
    return 0;
}
