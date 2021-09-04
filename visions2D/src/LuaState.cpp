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

// *********************************************************************************
// Lua function:
// void SetRotation(Entity*, rotation)
int lua_SetRotation(lua_State* L) {
	visions2D::Entity* owner = static_cast<visions2D::Entity*>(lua_touserdata(L, 1));
	float rotation = lua_tonumber(L, 2);

	visions2D::TransformComponent* t = owner->GetComponentOfType<visions2D::TransformComponent>();
	if (t != nullptr) {
		t->Rotation = rotation;
	}

	return 0;
}

// *********************************************************************************
// Lua function:
// float _GetCurrentRotation(Entity*)
int lua_GetCurrentRotation(lua_State* L) {
	visions2D::Entity* owner = static_cast<visions2D::Entity*>(lua_touserdata(L, 1));
	float Rotation = 0.0f;

	visions2D::TransformComponent* t = owner->GetComponentOfType<visions2D::TransformComponent>();

	if (t != nullptr) {
		Rotation = t->Rotation;
	}

	lua_pushnumber(L, Rotation);
	return 1;
}

namespace visions2D {
	namespace lua {
		static bool bIsInitialized = false;
		static lua_State* g_LuaState = nullptr;

		void InitializeLuaState() {
			g_LuaState = luaL_newstate();
			bIsInitialized = true;
			luaL_openlibs(g_LuaState);
			LOG_INFO("[engine-side lua] lua state initialized");

			lua_register(g_LuaState, "_Log", lua_Log);
			lua_register(g_LuaState, "_Move", lua_TransformTranslate);
			lua_register(g_LuaState, "_GetCurrentRotation", lua_GetCurrentRotation);
			lua_register(g_LuaState, "_SetRotation", lua_SetRotation);
		}

		bool IsInitialized() {
			return bIsInitialized;
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