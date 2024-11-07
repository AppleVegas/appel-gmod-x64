#include "interfaces.h"
#include "Common/patternscanning.h"
#include <iostream>
#include "Common/VMTHook.h"
#include "hooking.h"

CHLClient* Interfaces::client;
CEngineClient* Interfaces::engine;
ClientEntityList* Interfaces::cliententlist;
CEngineAPI* Interfaces::EngineAPI;

IEngineVGui* Interfaces::EngineVGui;
vgui::IPanel* Interfaces::panel;
vgui::IVGui* Interfaces::IVGui;
vgui::ISystem* Interfaces::ISystem;
vgui::ISchemeManager* Interfaces::Scheme;
vgui::IInput* Interfaces::vgui_IInput;
vgui::IInputInternal* Interfaces::vgui_IInputInternal;

IClientDLLSharedAppSystems* Interfaces::CLIENT_DLL_SHARED_APPSYSTEMS;
CUniformRandomStream* Interfaces::Random;
IBaseFileSystem* Interfaces::BaseFileSystem;
IFileSystem* Interfaces::FileSystem;
ISurface* Interfaces::surface;
ClientMode* Interfaces::clientmode;
CGlobalVarsBase* Interfaces::globalvars;
CModelInfo* Interfaces::modelinfo;
char* Interfaces::present;
RenderView* Interfaces::renderview;
IViewRender* Interfaces::viewrender;
CEngineVGui* Interfaces::cenginevgui;
IRender* Interfaces::render;
IMaterialSystem* Interfaces::materialsystem;
IMatRenderContext* Interfaces::MatRenderContext;
CInputSystem* Interfaces::InputSystem;
IVModelRender* Interfaces::ModelRender;
IEngineTrace* Interfaces::EngineTrace;
CInput* Interfaces::Input;
CLuaShared* Interfaces::LuaShared;
CPlayerResource* Interfaces::PlayerResource;
IGameMovement* Interfaces::GameMovement;
IPrediction* Interfaces::Prediction;
IMoveHelper* Interfaces::MoveHelper;
CBaseClientState* Interfaces::BaseClientState;
CClientState* Interfaces::ClientState;
CIVPhysicsDebugOverlay* Interfaces::PhysicsDebugOverlay;
IPhysicsSurfaceProps* Interfaces::PhysicsSurfaceProps;
ICvar* Interfaces::cvar;
StringTable* Interfaces::EStringTable;
CNetworkStringTableContainer* Interfaces::INetworkStringTableContainer;
CServerGameClients* Interfaces::ServerGameClients;
CGameEventManager* Interfaces::GameEventManager;
IMemAlloc* Interfaces::MemAlloc;
IShaderAPI* Interfaces::ShaderAPI;
Event_HitMarker* Interfaces::HitMarkerEvent;

void* Interfaces::GetInterface(const char* Module, const char* InterfaceName)
{
	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(Module), "CreateInterface");

	return (void*)CreateInterface(InterfaceName, NULL);
}
void Interfaces::SetupInterfaces()
{
	client = (CHLClient*)GetInterface("client.dll", "VClient017");
	engine = (CEngineClient*)GetInterface("engine.dll", "VEngineClient015");
	cliententlist = (ClientEntityList*)GetInterface("client.dll", "VClientEntityList003");

	panel = (vgui::IPanel*)GetInterface("vgui2.dll", "VGUI_Panel009");
	IVGui = (vgui::IVGui*)GetInterface("vgui2.dll", "VGUI_ivgui008");
	ISystem = (vgui::ISystem*)GetInterface("vgui2.dll", "VGUI_System010");
	surface = (ISurface*)GetInterface("vguimatsurface", "VGUI_Surface030");
	Scheme = (vgui::ISchemeManager*)GetInterface("vgui2.dll", "VGUI_Scheme010");
	vgui_IInput = (vgui::IInput*)GetInterface("vgui2.dll", "VGUI_Input005");
	vgui_IInputInternal = (vgui::IInputInternal*)GetInterface("vgui2.dll", "VGUI_InputInternal001");



	BaseFileSystem = (IBaseFileSystem*)GetInterface("filesystem_stdio.dll", "VBaseFileSystem011");
	FileSystem = (IFileSystem*)GetInterface("filesystem_stdio.dll", "VFileSystem022");

	CLIENT_DLL_SHARED_APPSYSTEMS = (IClientDLLSharedAppSystems*)GetInterface("client.dll", "VClientDllSharedAppSystems001");


	EngineAPI = (CEngineAPI*)GetInterface("engine.dll", "VENGINE_LAUNCHER_API_VERSION004");
	EngineVGui = (IEngineVGui*)GetInterface("engine.dll", "VEngineVGui001");


	modelinfo = (CModelInfo*)GetInterface("engine.dll", "VModelInfoClient006");
	renderview = (RenderView*)GetInterface("engine.dll", "VEngineRenderView014");
	materialsystem = (IMaterialSystem*)GetInterface("materialsystem.dll", "VMaterialSystem080");
	InputSystem = (CInputSystem*)GetInterface("inputsystem.dll", "InputSystemVersion001");
	ModelRender = (IVModelRender*)GetInterface("engine.dll", "VEngineModel016");
	EngineTrace = (IEngineTrace*)GetInterface("engine.dll", "EngineTraceClient003");
	PhysicsDebugOverlay = (CIVPhysicsDebugOverlay*)GetInterface("engine.dll", "VPhysicsDebugOverlay001");
	PhysicsSurfaceProps = (IPhysicsSurfaceProps*)GetInterface("vphysics.dll", "VPhysicsSurfaceProps001");
	cvar = (ICvar*)GetInterface("vstdlib.dll", "VEngineCvar004");
	ShaderAPI = (IShaderAPI*)GetInterface("shaderapidx9.dll", "ShaderApi030");
	GameEventManager = (CGameEventManager*)GetInterface("engine.dll", "GAMEEVENTSMANAGER002");
	GameMovement = (IGameMovement*)GetInterface("client.dll", "GameMovement001");
	Prediction = (IPrediction*)GetInterface("client.dll", "VClientPrediction001");
	//MoveHelper = **(IMoveHelper***)(FindPattern("client.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 0x2);
	//PlayerResource = (CPlayerResource*)(FindPattern("client.dll", "A1 ?? ?? ?? ?? 85 C0 74 06") + 1);
	LuaShared = (CLuaShared*)GetInterface("lua_shared.dll", "LUASHARED003");
	//ServerGameClients = **(CServerGameClients***)(FindPattern("engine.dll", "8B 0D ?? ?? ?? ?? FF 75 F8 D9 1D ?? ?? ?? ??") + 0x2);
	EStringTable = (StringTable*)GetInterface("engine.dll", "VEngineClientStringTable001");
	MemAlloc = *(IMemAlloc**)GetProcAddress(GetModuleHandleA("tier0.dll"), "g_pMemAlloc");

	////BaseClientState = *(CBaseClientState**)((*(uintptr_t**)engine)[84] + 1);
	INetworkStringTableContainer = *(CNetworkStringTableContainer**)GetInterface("engine.dll", "VEngineClientStringTable001");
	////Input = **(CInput***)((*(uintptr_t**)client)[14] + 0x2);
	////globalvars = **(CGlobalVarsBase***)((*(uintptr_t**)client)[0] + 0x55);
	////clientmode = **(ClientMode***)((*(uintptr_t**)client)[10] + 0x5);

	present = GetRealFromRelative((char*)findPattern("gameoverlayrenderer64.dll", PRESENT_SIG), 3, 7, false);

	////cenginevgui = (CEngineVGui*)(int(__stdcall***)(BYTE))((*(uintptr_t**)renderview)[23] + 1);
	////render = **(IRender***)((*(uintptr_t**)renderview)[12] + 0x5);
	////viewrender = **(IViewRender***)((*(uintptr_t**)client)[27] + 0x5);

	//void(*RandomSeed)(int);
	//*(void**)&RandomSeed = GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomSeed");
	//if (RandomSeed)
	//	Random = **(CUniformRandomStream***)((uintptr_t)RandomSeed + 0x5);
}

void Interfaces::PrintInterfaces()
{
	std::cout << "CHLClient - 0x" << client << std::endl;
	std::cout << "CEngineClient - 0x" << engine << std::endl;
	std::cout << "ClientEntityList - 0x" << cliententlist << std::endl;
	std::cout << "CEngineAPI - 0x" << EngineAPI << std::endl;

	std::cout << "IEngineVGui - 0x" << EngineVGui << std::endl;
	std::cout << "vgui::IPanel - 0x" << panel << std::endl;
	std::cout << "vgui::IVGui - 0x" << IVGui << std::endl;
	std::cout << "vgui::ISystem - 0x" << ISystem << std::endl;
	std::cout << "vgui::ISchemeManager - 0x" << Scheme << std::endl;
	std::cout << "vgui::IInput - 0x" << vgui_IInput << std::endl;
	std::cout << "vgui::IInputInternal - 0x" << vgui_IInputInternal << std::endl;

	std::cout << "IClientDLLSharedAppSystems - 0x" << CLIENT_DLL_SHARED_APPSYSTEMS << std::endl;
	std::cout << "CUniformRandomStream - 0x" << Random << std::endl;
	std::cout << "IBaseFileSystem - 0x" << BaseFileSystem << std::endl;
	std::cout << "IFileSystem - 0x" << FileSystem << std::endl;
	std::cout << "ISurface - 0x" << surface << std::endl;
	std::cout << "ClientMode - 0x" << clientmode << std::endl;
	std::cout << "CGlobalVarsBase - 0x" << globalvars << std::endl;
	std::cout << "CModelInfo - 0x" << modelinfo << std::endl;

	printf("\nIDirectPresent - 0x%p\n\n", present);//std::cout << "IDirectPresent - 0x" << std::hex << present << std::endl;

	std::cout << "RenderView - 0x" << renderview << std::endl;
	std::cout << "IViewRender - 0x" << viewrender << std::endl;
	std::cout << "CEngineVGui - 0x" << cenginevgui << std::endl;
	std::cout << "IRender - 0x" << render << std::endl;
	std::cout << "IMaterialSystem - 0x" << materialsystem << std::endl;
	std::cout << "IMatRenderContext - 0x" << MatRenderContext << std::endl;
	std::cout << "IInputSystem - 0x" << InputSystem << std::endl;
	std::cout << "IVModelRender - 0x" << ModelRender << std::endl;
	std::cout << "IEngineTrace - 0x" << EngineTrace << std::endl;
	std::cout << "CInput - 0x" << Input << std::endl;
	std::cout << "CLuaShared - 0x" << LuaShared << std::endl;
	std::cout << "CPlayerResource - 0x" << PlayerResource << std::endl;
	std::cout << "IGameMovement - 0x" << GameMovement << std::endl;
	std::cout << "IPrediction - 0x" << Prediction << std::endl;
	std::cout << "IMoveHelper - 0x" << MoveHelper << std::endl;
	std::cout << "CBaseClientState - 0x" << BaseClientState << std::endl;
	std::cout << "CClientState - 0x" << ClientState << std::endl;
	std::cout << "CIVPhysicsDebugOverlay - 0x" << PhysicsDebugOverlay << std::endl;
	std::cout << "IPhysicsSurfaceProps - 0x" << PhysicsSurfaceProps << std::endl;
	std::cout << "ICvar - 0x" << cvar << std::endl;
	std::cout << "StringTable - 0x" << EStringTable << std::endl;
	std::cout << "CNetworkStringTableContainer - 0x" << INetworkStringTableContainer << std::endl;
	std::cout << "CServerGameClients - 0x" << ServerGameClients << std::endl;
	std::cout << "CGameEventManager - 0x" << GameEventManager << std::endl;
	std::cout << "IMemAlloc - 0x" << MemAlloc << std::endl;
	std::cout << "IShaderAPI - 0x" << ShaderAPI << std::endl;
	std::cout << "Event_HitMarker - 0x" << HitMarkerEvent << std::endl;
}