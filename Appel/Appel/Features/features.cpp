#include "features.h"
#include "../Common/sourcesdk.h"
#include "../interfaces.h"

Game::vec2 Game::GetResolution()
{
	vec2 Res;
	Interfaces::engine->GetScreenSize(Res.x, Res.y);
	return Res;
}

bool Game::IsInGame()
{
	return Interfaces::engine->IsInGame();
}

void Game::EnableInput(bool enable)
{
	Interfaces::InputSystem->EnableInput(enable);
}

void Game::SetCursorVisible(bool visible)
{
	Interfaces::surface->SetCursorAlwaysVisible(visible);
}