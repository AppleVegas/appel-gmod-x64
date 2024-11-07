#pragma once

#include <windows.h>
#include <string>

#define APPEL_VERSION std::string("3.0")
#define APPEL_DEBUG

#include "Common\console.h"

namespace Appel
{
	namespace Variables {
		extern HMODULE mainModule;

		extern HANDLE mainThread;
		extern HANDLE unloadThread;

		extern bool run;
	}
	namespace Console {
		void Initialize();
		bool isInitialized();
		void HandleInput();
		void Deinitialize();

		extern bool initialized;
	}
	namespace Authorization {
		bool Authorize();
		bool IsAuthrorized();
		bool DeAuthorize();

		extern bool allowed;
		extern bool why;
	}

	namespace Hack {
		bool Initialize();
		bool Deinitialize();
	}

	bool Unload();
}
