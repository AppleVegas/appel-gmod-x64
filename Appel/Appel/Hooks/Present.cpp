#include "Present.h"
#include "../Common/console.h"
#include "../Menu/menu.h"
#include "../Drawing/dx_drawing.h"
#include "../UI/UIManager.h"

#include "../interfaces.h"
#include "../Features/features.h"

DirectPresent Present_original;

hk_SetCursorPos SetCursorPos_original = NULL;

HHOOK kbdHook{ NULL };

WNDPROC orgproc = nullptr;

bool openedfully = false;
bool closedfully = false;

bool opening = false;
BOOL WINAPI SetCursorPos_hooked(int X, int Y)
{
	if (Menu::IG::opened)
		return false;

	return SetCursorPos_original(X, Y);
}

LRESULT __stdcall proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	

	ImGui_ImplDX9_WndProcHandler(hwnd, msg, wParam, lParam);

	if (!Menu::KeyboardHandler(msg, wParam, lParam))
		return false;

	if (!Menu::IG::KeyboardHandler(msg, wParam, lParam))
		return false;

	return CallWindowProcA(orgproc, hwnd, msg, wParam, lParam);
}

HRESULT __stdcall Present_hooked(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	if (Menu::opened) 
	{
		static bool initFonts;

		pDevice->BeginScene();

		init(pDevice);
		Draw.GetDevice(pDevice);
		Draw.Reset();

		if (Draw.Font()) Draw.OnLostDevice();

		Menu::Tick();

		pDevice->EndScene();
	}
	if (!Menu::IG::initialised)
	{
		auto gmod = FindWindowW(L"Valve001", nullptr);

		orgproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(gmod, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(proc)));

		ImFontConfig font_config;
		font_config.OversampleH = 1; //or 2 is the same
		font_config.OversampleV = 1;
		font_config.PixelSnapH = true;

		static const ImWchar ranges[] =
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x0400, 0x044F, // Cyrillic
			0,
		};
		ImGuiIO& io = ImGui::GetIO();
		ImFont* font1 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 14.0f, &font_config, ranges);
		font1->DisplayOffset.y -= 1;
		ImGui_ImplDX9_Init(gmod, pDevice);
		ImGuiStyle& style = ImGui::GetStyle();

		Menu::IG::initialised = true;
	}
	else
	{
		if (Menu::IG::opened)
		{
			Game::EnableInput(false);
			if (!openedfully)
			{
				//Game::SetCursorVisible(true);
				openedfully = true;
				closedfully = false;
			}
		}
		else
		{

			if (!closedfully)
			{
				if (Game::IsInGame())
				{
					Game::vec2 Resolution = Game::GetResolution();
					//printf(std::to_string(Resolution.x).c_str(), " ", std::to_string(Resolution.y).c_str(), "\n");
					SetCursorPos_original(Resolution.x / 2, Resolution.y / 2);
				}
				//Game::SetCursorVisible(false);
				closedfully = true;
				openedfully = false;
			}
			Game::EnableInput(true);
		}

		//if (HackVars::Visuals::FreeCam && HackVars::Visuals::Enabled)
		//	InputSystem()->EnableInput(false);

		ImGui_ImplDX9_NewFrame();
		Menu::IG::DrawTextEditor();

		ImGui::Render();
	}

	return Present_original(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}