#include "windows.h"
#include <d3d9.h>
#include "../DX9SDK/d3dx9.h"

#include "../Common/Imgui/imgui.h"
#include "../Common/Imgui/ImGuiFileDialog.h"
#include "../Common/Imgui/TextEditor.h"
#include "../Common/Imgui/imgui_impl_dx9.h"

typedef HRESULT(__stdcall* DirectPresent)(IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);

typedef BOOL(WINAPI* hk_SetCursorPos)(int, int);

extern DirectPresent Present_original;
extern hk_SetCursorPos SetCursorPos_original;
extern HHOOK kbdHook;

extern WNDPROC orgproc;

BOOL WINAPI SetCursorPos_hooked(int X, int Y);
HRESULT __stdcall Present_hooked(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
extern IMGUI_API LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);