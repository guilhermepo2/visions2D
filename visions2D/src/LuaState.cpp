#include "LuaState.h"
#include "Entity/Entity.h"
#include "Entity/Component/TransformComponent.h"

// *********************************************************************************
// Lua function:
// void _Log(string)
int lua_Log(lua_State* L) {
	const char* message = lua_tostring(L, 1);
	LOG_INFO("[script-side lua] {0}", message);
	return 0;
}

// *********************************************************************************
// Lua function:
// void _Move(Entity*, x, y)
int lua_TransformTranslate(lua_State* L) {
	visions2D::Entity* owner = static_cast<visions2D::Entity*>(lua_touserdata(L, 1));
	float x = lua_tonumber(L, 2);
	float y = lua_tonumber(L, 3);

	visions2D::TransformComponent* t = owner->GetComponentOfType<visions2D::TransformComponent>();
	if (t != nullptr) {
		t->Translate(glm::vec2(x, y));
	}

	return 0;
}

namespace visions2D {
	namespace lua {
		static bool bIsInitialized;
		static lua_State* g_LuaState;

		void InitializeLuaState() {
			g_LuaState = luaL_newstate();
			luaL_openlibs(g_LuaState);
			LOG_INFO("[engine-side lua] lua state initialized");

			lua_register(g_LuaState, "_Log", lua_Log);
			lua_register(g_LuaState, "_Move", lua_TransformTranslate);
		}

		bool CheckLua(lua_State* L, int r) {
			if (r != LUA_OK) {
				std::string errormsg = lua_tostring(L, -1);
				LOG_ERROR(errormsg);
				return false;
			}

			return true;
		}

		lua_State* GetState() {
			return g_LuaState;
		}
	}
}