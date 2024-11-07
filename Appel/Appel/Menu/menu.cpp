#include "menu.h"
//#include "../UI/UIManager.h"
#include "../UI/BoolOption.h"
#include "../UI/TextOption.h"
#include "../UI/ChooseOption.h"
#include "../UI/NumberOption.h"
#include "../UI/RegularOption.h"
#include "../UI/SubOption.h"
#include "../UI/RegularSubmenu.h"
#include "../UI/PlayerSubmenu.h"

#include "../appel.h"
#include "../Config/config.h"

bool Menu::initialized = false;
bool Menu::opened = false;
void Menu::Initialize()
{
	using namespace Appel::UI;
	
	g_UiManager = std::make_unique<UIManager>();
	
	if (!g_UiManager->FontsInitialized())
		g_UiManager->InitFonts();

	initialized = true;

	g_UiManager->AddSubmenu<RegularSubmenu>("MAIN MENU", Home, [](RegularSubmenu* sub)
	{

		sub->AddOption<SubOption>("Self", nullptr, Home_Self);
		//sub->AddOption<SubOption>("All Online Players", nullptr, SubmenuAllPlayers);
		sub->AddOption<SubOption>("Online Players", nullptr, Home_OnlinePlayers);
		sub->AddOption<BoolOption<bool>>("~bB~qo~ro~yl", nullptr, Appel::config.c_bool("test"), BoolDisplay::OnOff, false);
		sub->AddOption<SubOption>("Что?", nullptr, Home_OnlinePlayers);
		//sub->AddOption<SubOption>("Teleports", nullptr, SubmenuTP);
		sub->AddOption<SubOption>("Settings", nullptr, Home_Settings);
		sub->AddOption<TextOption>(nullptr, nullptr, "--- Hey Cousin ---");
	});

	g_UiManager->AddSubmenu<RegularSubmenu>("Settings", Home_Settings, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Themes", nullptr, Home_Settings_Themes);
			sub->AddOption<NumberOption<float>>("X Position", nullptr, &g_UiManager->m_PosX, 0.0f, 0.95f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("Y Position", nullptr, &g_UiManager->m_PosY, 0.0f, 0.95f, 0.01f, 2);
			sub->AddOption<RegularOption>(std::move(RegularOption("Version").SetRightText(APPEL_VERSION.c_str())));
		});

	g_UiManager->AddSubmenu<RegularSubmenu>("Themes", Home_Settings_Themes, [](RegularSubmenu* sub)
		{
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Theme", nullptr, &Lists::HeaderTypesFrontend, &Lists::HeaderTypesPosition, true, []
				{
					g_UiManager->m_HeaderType = Lists::HeaderTypesBackend[Lists::HeaderTypesPosition];
				});
		});
}

bool Menu::IsInitialized()
{
	return initialized;
}

void Menu::Tick()
{
	if (!IsInitialized())
		Initialize();

	Appel::UI::g_UiManager->OnTick();

}

bool Menu::KeyboardHandler(UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_F5:
			Menu::opened = !Menu::opened;
			return false;
		}
		if (Menu::opened)
		{
			switch (wParam)
			{
			case VK_DOWN:
				Appel::UI::g_UiManager->Down();
				return false;
			case VK_UP:
				Appel::UI::g_UiManager->Up();
				return false;
			case VK_LEFT:
				Appel::UI::g_UiManager->Left();
				return false;
			case VK_RIGHT:
				Appel::UI::g_UiManager->Right();
				return false;
			case VK_BACK:
				Appel::UI::g_UiManager->Back();
				return false;
			case VK_RETURN:
				Appel::UI::g_UiManager->Enter();
				return false;
			}
		}
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_F5:
			return false;
		}
		if (Menu::opened)
		{
			switch (wParam)
			{
			case VK_DOWN:
				return false;
			case VK_UP:
				return false;
			case VK_LEFT:
				return false;
			case VK_RIGHT:
				return false;
			case VK_BACK:
				return false;
			case VK_RETURN:
				return false;
			}
		}
	}

	return true;
}

void Menu::Deinitialize()
{
	Appel::UI::g_UiManager->UnInitFonts();
	Appel::UI::g_UiManager.reset();
	initialized = false;
}