#include "appel.h"
#include <windows.h>
#include <string>
#include "Common/console.h"
#include "hooking.h"

HMODULE Appel::Variables::mainModule;

HANDLE Appel::Variables::mainThread;
HANDLE Appel::Variables::unloadThread;

bool Appel::Variables::run;

bool Appel::Console::initialized;

void Appel::Console::Initialize()
{
	if (isInitialized())
		return;

	CreateConsole(std::string("Appel v" + APPEL_VERSION).c_str());
	printColor(COLOR_PURPLE, R"EOF(
  ___                   _ 
 / _ \                 | |
/ /_\ \_ __  _ __   ___| |
|  _  | '_ \| '_ \ / _ \ |
| | | | |_) | |_) |  __/ |
\_| |_/ .__/| .__/ \___|_|    By AppleVegas
      | |   | |           
      |_|   |_|           
)EOF");
	printf("\n\n\n");
	initialized = true;
}

void Appel::Console::HandleInput()
{
	while (Appel::Variables::run) {

		// Get input from the user.
		char usrInput[2048] = { 0 };
		char* nextUsrInput = NULL;
		fgets(usrInput, 2047, stdin);

		// Seperate and validate the user input.
		char* cmd = strtok_s(usrInput, " ", &nextUsrInput);


		if (strcmp(cmd, "help\n") == 0) {
			printColorS(COLOR_BLUE, "Command list: \n");
			printColorS(COLOR_CYAN, "help - show command list. \n");
			printColorS(COLOR_CYAN, "exit - uninject Appel. \n");
			continue;
		}
		if (strcmp(cmd, "exit\n") == 0) {
			printInfo("Uninjected.\n");
			Appel::Variables::run = false;
			break;
		}
		else {
			printErrorS("Unknown command! \n");
			continue;
		}
	}
}

bool Appel::Console::isInitialized()
{
	return initialized;
}

void Appel::Console::Deinitialize()
{
	if (!isInitialized())
		return;

	fclose(stdout);
	fclose(stdin);

	FreeConsole();

	initialized = false;
}

bool Appel::Hack::Initialize()
{
#ifdef APPEL_DEBUG
	if (!Appel::Hooking::SetUpDebug())
		return false;
#else
	if (!Appel::Hooking::SetUp())
		return false;
#endif
	Appel::Hooking::HookFunctions();
	return true;
}

bool Appel::Hack::Deinitialize()
{
	Appel::Hooking::UnHookFunctions();
	Appel::Hooking::Shutdown();
	return true;
}

bool Appel::Unload()
{
	//InputSystem()->EnableInput(true);
	//surface()->SetCursorAlwaysVisible(false);
	//SetWindowLongPtrW(FindWindowW(L"Valve001", nullptr), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(orgproc));
	//delete Interfaces::_I::HitMarkerEvent;
	//DetourTransactionBegin();
	//DetourUpdateThread(GetCurrentThread());
	//DetourDetach(&(PVOID&)pLoadBufferx, cLoadbufferx);
	//DetourTransactionCommit();
	//Hook::UnHookFunctions();

	//ImGui_ImplDX9_Shutdown();
	Appel::Hack::Deinitialize();
	if (TerminateThread(Appel::Variables::mainThread, 0))
	{
		Sleep(1000);
		Appel::Console::Deinitialize();
		FreeLibraryAndExitThread(Appel::Variables::mainModule, 0);
		return true;
	}
	return false;
}
