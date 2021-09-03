#pragma once
#include "Log.h"

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

namespace visions2D {
	namespace lua {
		void InitializeLuaState();
		bool CheckLua(lua_State* L, int r);
		lua_State* GetState();
	}
}