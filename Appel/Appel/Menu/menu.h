#define TABWIDTH 65
#define TABWHEIGHT 30
#include <stdint.h>
#include <d3d9.h>
#include "../UI/UIManager.h"

enum Submenu : uint32_t
{
	Home,
	Home_Self,
	Home_OnlinePlayers,
	Home_Settings,
	Home_Settings_Themes,
	
};

namespace Menu
{
	namespace IG
	{
		extern bool initialised;
		extern bool opened;
		void DrawTextEditor();
		bool KeyboardHandler(UINT msg, WPARAM wParam, LPARAM lParam);
	}

	void Initialize();
	bool IsInitialized();
	void Tick();
	void Deinitialize();
	bool KeyboardHandler(UINT msg, WPARAM wParam, LPARAM lParam);

	extern bool initialized;
	extern bool opened;
	inline const char* g_SelectedName{};
};

namespace Menu::Lists
{
	inline const char* HeaderTypesFrontend[]
	{
		"Appel",
		"Macintosh",
		"Garry's Mod"
	};

	inline Appel::UI::HeaderType HeaderTypesBackend[]
	{
		Appel::UI::HeaderType::Appel,
		Appel::UI::HeaderType::Mac,
		Appel::UI::HeaderType::Garry,
	};

	inline std::size_t HeaderTypesPosition = 0;

	inline const char* DemoList[10]
	{
		"One",
		"Two",
		"Three",
		"Four",
		"Five",
		"Six",
		"Seven",
		"Eight",
		"Nine",
		"Ten"
	};

	inline std::size_t DemoListPos = 0;
}