#include "windows.h"
#include <string>
typedef void* (*CreateInterfaceFn)(const char* Name, int* ReturnCode);
class InterfaceReg
{
public:
	BYTE			m_CreateFn[4];
	const char* m_pName;
	InterfaceReg* m_pNext;
};
class IBaseInterface
{
public:
	virtual	~IBaseInterface() {}
};
class IClientDLLSharedAppSystems
{
public:
	virtual int	Count() = 0;
	virtual char const* GetDllName(int idx) = 0;
	virtual char const* GetInterfaceName(int idx) = 0;
};
class IAppSystem
{
public:
	virtual void		Nothing_Important__00() = 0;
	virtual void		Nothing_Important__01() = 0;
	virtual void		Nothing_Important__02() = 0;
	virtual void		Nothing_Important__03() = 0;
};
enum InitReturnVal_t
{
	INIT_FAILED = 0,
	INIT_OK,

	INIT_LAST_VAL,
};
class CAppSystemGroup; // look into later
struct StartupInfo_t
{
	void* m_pInstance;
	const char* m_pBaseDirectory;	// Executable directory ("c:/program files/half-life 2", for example)
	const char* m_pInitialMod;		// Mod name ("cstrike", for example)
	const char* m_pInitialGame;		// Root game name ("hl2", for example, in the case of cstrike)
	CAppSystemGroup* m_pParentAppSystemGroup;
	bool m_bTextMode;
};
class CEngineAPI
{
public:
	virtual bool Connect(CreateInterfaceFn factory) = 0;
	virtual void Disconnect() = 0;
	virtual void* QueryInterface(const char* pInterfaceName) = 0;
	virtual InitReturnVal_t Init() = 0;
	virtual void Shutdown() = 0;
	virtual void SetStartupInfo(StartupInfo_t& info) = 0;
	virtual int Run() = 0;
	virtual void SetEngineWindow(void* hWnd) = 0;
	virtual void PostConsoleCommand(const char* pConsoleCommand) = 0;
	virtual bool IsRunningSimulation() const = 0;
	virtual void ActivateSimulation(bool bActive) = 0;
	virtual void SetMap(const char* pMapName) = 0;
};

namespace vgui
{
	class IInput;
	class IInputInternal;
	class SurfacePlat;
	class IClientPanel;
	class Panel;
	class IScheme;
	class ISchemeManager;
	class IPanel;
	class IVGui;
	class ISystem;
};

class CUniformRandomStream;
class IBaseFileSystem;
class IFileSystem;
class IEngineVGui;
class CHLClient;
class CEngineClient;
class ClientEntityList;
class CPlayerResource;
class IPanel;
class ISurface;
class ClientMode;
class CGlobalVarsBase;
class CModelInfo;
class RenderView;
class CEngineVGui;
class IRender;
class IViewRender;
class IMaterialSystem;
class IMatRenderContext;
class CInputSystem;
class IVModelRender;
class IEngineTrace;
class CInput;
class CLuaShared;
class IGameMovement;
class IPrediction;
class IMoveHelper;
class CBaseClientState;
class CClientState;
class CIVPhysicsDebugOverlay;
class IPhysicsSurfaceProps;
class ICvar;
class CServerGameClients;
class StringTable;
class CGameEventManager;
class CNetworkStringTableContainer;
class IMemAlloc;
class IShaderAPI;
class Event_HitMarker;

namespace Interfaces
{
	extern CHLClient* client;
	extern CEngineClient* engine;
	extern ClientEntityList* cliententlist;
	extern CEngineAPI* EngineAPI;

	extern IEngineVGui* EngineVGui;
	extern vgui::IPanel* panel;
	extern vgui::IVGui* IVGui;
	extern vgui::ISystem* ISystem;
	extern vgui::ISchemeManager* Scheme;
	extern vgui::IInput* vgui_IInput;
	extern vgui::IInputInternal* vgui_IInputInternal;

	extern IClientDLLSharedAppSystems* CLIENT_DLL_SHARED_APPSYSTEMS;
	extern CUniformRandomStream* Random;
	extern IBaseFileSystem* BaseFileSystem;
	extern IFileSystem* FileSystem;
	extern ISurface* surface;
	extern ClientMode* clientmode;
	extern CGlobalVarsBase* globalvars;
	extern CModelInfo* modelinfo;
	extern char* present;
	extern RenderView* renderview;
	extern IViewRender* viewrender;
	extern CEngineVGui* cenginevgui;
	extern IRender* render;
	extern IMaterialSystem* materialsystem;
	extern IMatRenderContext* MatRenderContext;
	extern CInputSystem* InputSystem;
	extern IVModelRender* ModelRender;
	extern IEngineTrace* EngineTrace;
	extern CInput* Input;
	extern CLuaShared* LuaShared;
	extern CPlayerResource* PlayerResource;
	extern IGameMovement* GameMovement;
	extern IPrediction* Prediction;
	extern IMoveHelper* MoveHelper;
	extern CBaseClientState* BaseClientState;
	extern CClientState* ClientState;
	extern CIVPhysicsDebugOverlay* PhysicsDebugOverlay;
	extern IPhysicsSurfaceProps* PhysicsSurfaceProps;
	extern ICvar* cvar;
	extern StringTable* EStringTable;
	extern CNetworkStringTableContainer* INetworkStringTableContainer;
	extern CServerGameClients* ServerGameClients;
	extern CGameEventManager* GameEventManager;
	extern IMemAlloc* MemAlloc;
	extern IShaderAPI* ShaderAPI;
	extern Event_HitMarker* HitMarkerEvent;

	void* GetInterface(const char* Module, const char* InterfaceName);
	void SetupInterfaces();
	void PrintInterfaces();
}