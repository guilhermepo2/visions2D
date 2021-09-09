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
		void BindScriptFunctions(lua_State* L);
		bool IsInitialized();

		bool CheckLua(lua_State* L, int r);
		lua_State* GetState();
	}
}