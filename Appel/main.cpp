#include "Appel\appel.h"

void appel_main(HMODULE hModule) 
{
    Appel::Variables::run = true;

    Appel::Console::Initialize();

    if (!Appel::Hack::Initialize())
    {
        Appel::Variables::run = false;
        return;
    }
    

    printInfo("Appel successfully loaded! \n");
    Appel::Console::HandleInput();
}

void appel_unload(HMODULE hModule)
{
	while (Appel::Variables::run)
    {
		Sleep(1000);
	}

    if (Appel::Unload())
        ExitThread(0);
}

BOOL __stdcall DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Appel::Variables::mainModule = hModule;
        Appel::Variables::mainThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)appel_main, hModule, 0, nullptr);
        Appel::Variables::unloadThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)appel_unload, hModule, 0, nullptr);
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

