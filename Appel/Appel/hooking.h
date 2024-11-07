#pragma once

#define PRESENT_SIG "\x48\x8B\x05????\x4D\x8B\xCE"

namespace Appel
{
	namespace Hooking {
		bool SetUp();
		bool SetUpDebug();
		void HookFunctions();
		void UnHookFunctions();
		void Shutdown();
	}
}
