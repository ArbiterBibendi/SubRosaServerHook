// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Definitions.h"
#include <Windows.h>
#include <iostream>
#include "luawrapper.h"





DWORD hook_detour(void* toReplace, void* toBeReplacedWith, int len) {
    DWORD currentProtections;
    VirtualProtect(toReplace, len, PAGE_EXECUTE_READWRITE, &currentProtections);
    /* 
    *   First we set up the gateway, which will hold the original
    *   executable bytes from the beginning of the hooked function
    *   and a jmp to the original code
    * 
    *   This will be used as a function pointer at the end of our function
    */
    DWORD gateway = (DWORD)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!gateway) {
        std::cout << GetLastError();
    }
    else {
        std::cout << std::hex << "GATEWAY: 0x" << gateway << " TOREPLACE: 0x" << toReplace << std::endl;
        DWORD originalCodeAddress = (DWORD)toReplace - ((DWORD)gateway) - 5;
        memcpy((void*)gateway, toReplace, len);
        *(BYTE*)((BYTE*)gateway + len) = 0xE9;
        *(DWORD*)((DWORD)gateway + len + 1) = originalCodeAddress;


        /* Now we actually replace the bytes in the beginning of the function with a jmp to our function */
        *(BYTE*)toReplace = 0xE9;
        *(DWORD*)((DWORD)toReplace + 1) = ((DWORD)toBeReplacedWith - (DWORD)toReplace - 5);
        VirtualProtect(toReplace, 5, currentProtections, &currentProtections);
        return gateway;
    }
}

int __cdecl ourSpawnVehicle(int a, vec3* b, vec3* c, rotation* d, int e) {
    //printf("Spawn Type: %x | X: %f, Y: %f, Z: %f | %x | %f, %f, %f | %d\n", a, b->x, b->y, b->z, c, d->a, d->e, d->i, e);

    b->y += 40;
    return SpawnVehicle(a, b, c, d, e);
}

bool key_pressed(int key) {
    if ((GetAsyncKeyState(key) & 1)) {
        return true;
    }
    else {
        return false;
    }
}


int itemID = 0; //TEMPORARY
bool haveExecutedMainInit = false;
void mainInit() { // This function is called once after the world loads on the main thread in the context of the server
    std::cout << "Initializing..." << std::endl;
    loadLua();

    l = dluaL_newstate();
    dluaL_openlibs(l);

    dlua_register(l, "spawnVehicle", l_SpawnVehicle);
    dlua_register(l, "test", l_test);

    dluaL_loadfile(l, "script.lua");
    int err = dlua_pcall(l, 0, 0, 0);
    if (err) {
        std::cout << dlua_tostring(l, -1) << std::endl;
    }

    dlua_getglobal(l, "init");
    err = dlua_pcall(l, 0, 0, 0);
    if (err) {
        std::cout << dlua_tostring(l, -1) << std::endl;
    }
    haveExecutedMainInit = true;
}
void __fastcall hMainLoop() { // This function is called once per loop iteration on the main thread in the context of the server
    if (!haveExecutedMainInit) {
        mainInit();
    }
    dlua_getglobal(l, "loop");
    int err = dlua_pcall(l, 0, 0, 0);
    if (err) {
        std::cout << dlua_tostring(l, -1) << std::endl;
    }
    if (key_pressed('O')) {
        vec3 pos;
        rotation rot;
        pos.x = 2951.29f;
        pos.y = 24.869f;
        pos.z = 1532.15f;
        
        
        SpawnItem(itemID++, &pos, 0, &rot);
    }
    HookedLoopFunct();
}

DWORD WINAPI on_attach(LPVOID lparam) {
    /* Get address of server module */
    DWORD subRosaDedicated;
    GetModuleHandleEx(0, NULL, (HMODULE*)&subRosaDedicated);
    std::cout << "DLL injected successfully :/ Server loaded at " << std::hex << "0x" << subRosaDedicated << std::endl;;


    /* Calculate address of functions and hook them if necessary */
    SpawnItem = (SpawnItem_p)(subRosaDedicated + 0xA39F0);
    
    SpawnVehicle = (SpawnVehicle_p) (subRosaDedicated + 0x5D0A0);
    SpawnVehicle = (SpawnVehicle_p)hook_detour(SpawnVehicle, ourSpawnVehicle, 5);

    HookedLoopFunct = (HookedLoopFunct_p)(subRosaDedicated + 0x63CB0);
    HookedLoopFunct = (HookedLoopFunct_p)hook_detour(HookedLoopFunct, hMainLoop, 8);

    bool shouldExit = false;
    while (!shouldExit) {
        
        if (key_pressed('L')) {
            std::cout << "Exit\n";
            //shouldExit = true;
        }
        Sleep(100);
    }
    dlua_close(l);
    FreeLibraryAndExitThread((HMODULE)lparam, 0);
    return 0;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, on_attach, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

