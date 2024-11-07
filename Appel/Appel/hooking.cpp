#include "hooking.h"
#include "Common/VMTHook.h"
#include "interfaces.h"
#include "Hooks/Present.h"
#include "Common/MinHook.h"
#include "appel.h"

bool Appel::Hooking::SetUp()
{
	if (MH_Initialize() != MH_OK)
	{
		return false;
	}
	
	Interfaces::SetupInterfaces();
	return true;
}

bool Appel::Hooking::SetUpDebug()
{
	MH_STATUS init = MH_Initialize();
	if (init != MH_OK)
	{
		printf("MH Init fail = %d", init);
		Sleep(2000);
		return false;
	}

	Interfaces::SetupInterfaces();
	Interfaces::PrintInterfaces();
	return true;
}

void Appel::Hooking::HookFunctions()
{
	//Present_original = *(DirectPresent*)(Interfaces::present);
	//*(DirectPresent**)(Interfaces::present) = (DirectPresent*)Present_hooked;

	MH_STATUS present_status = MH_CreateHook(*(LPVOID*)Interfaces::present, &Present_hooked, &(LPVOID&)(Present_original));
	if (present_status != MH_OK)
		printError("Failed to hook Present. MH_STATUS = %d", present_status);

	MH_STATUS setcursor_status = MH_CreateHook(&SetCursorPos, &SetCursorPos_hooked, &(LPVOID&)(SetCursorPos_original));
	if (present_status != MH_OK)
		printError("Failed to hook SetCursor. MH_STATUS = %d", setcursor_status);

	MH_EnableHook(MH_ALL_HOOKS);
}

void Appel::Hooking::UnHookFunctions()
{
	MH_DisableHook(MH_ALL_HOOKS);

	MH_RemoveHook(&(LPVOID&)(Interfaces::present));

	//*(char**)(Interfaces::present) = (char*)(Present_original);

	SetWindowLongPtrW(FindWindowW(L"Valve001", nullptr), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(orgproc));
	ImGui_ImplDX9_Shutdown();

	MH_RemoveHook(&SetCursorPos);
}

void Appel::Hooking::Shutdown()
{
	MH_Uninitialize();
}