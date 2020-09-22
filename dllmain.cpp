#include <iostream>
#include <functional>
#include "CustomAPI.h"
#include "offsets.h"
using namespace std;

uint64_t cslua, adhesive;
uint64_t* luaRuntime;


using runFileInternal_t = int(_fastcall*)(uint64_t, const char*, std::function<int(char*)>);
using loadSystemFileInternal_t = int(_fastcall*)(uint64_t, const char*);

runFileInternal_t runFileInternal;


void getMods() {
    while (!cslua) {
        cslua = CustomAPI::GetModuleA("citizen-scripting-lua");
        luaRuntime = (uint64_t*)(cslua + 0x63CF80);
    }
    while (!adhesive) {
        adhesive = CustomAPI::GetModuleA("adhesive");
    }
    runFileInternal = (runFileInternal_t)(cslua + o_runFileInternal);
}

int LoadSystemFile(const char* scriptName) {
    *(BYTE*)(adhesive + o_adhesive) = 1;
    return runFileInternal((uint64_t)(*luaRuntime), scriptName, std::bind((loadSystemFileInternal_t)(cslua + o_loadSystemFileInternal), (uint64_t)(cslua + o_luaRuntime), std::placeholders::_1));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  callReason, LPVOID lpReserved) {
    if (callReason == DLL_PROCESS_ATTACH) {
        getMods();

        while (true)
        {
            if (GetAsyncKeyState(VK_F5) & 1)
            {
                LoadSystemFile("C:\menu\menu.lua");
            }
        }
    }

    return TRUE;
}
