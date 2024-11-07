#pragma once 

namespace Game {
	struct vec2 {
		int x;
		int y;
	};

	vec2 GetResolution();
	bool IsInGame();
	void EnableInput(bool enable);
	void SetCursorVisible(bool visible);
}