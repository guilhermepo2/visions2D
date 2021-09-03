#include "Log.h"
#include "LuaComponent.h"

/*
void lua_Log(const char* message) {
	LOG_INFO("[lua script] {0}", message);
}
*/


namespace visions2D {
	
	// ***************************************************************
	LuaComponent::LuaComponent(const std::string& LuaFilePath) {
		LOG_INFO("LUA COMPONENT ADDED!");
		m_StateRef = lua::GetState();
		LuaAssetPath = LuaFilePath;

		// call "OnCreate" function"
		CallLuaFunction("OnCreate");
	}

	// ***************************************************************
	LuaComponent::~LuaComponent() {
		CallLuaFunction("Destroy");
	}

	// ***************************************************************
	void LuaComponent::Initialize() {
		CallLuaFunction("Initialize");
	}

	// ***************************************************************
	void LuaComponent::BeginPlay() {
		CallLuaFunction("BeginPlay");
	}

	// ***************************************************************
	bool LuaComponent::ProcessInput(const InputState& CurrentInputState) {
		unreferenced(CurrentInputState);
		CallLuaFunction("ProcessInput");
		return false;
	}

	// ***************************************************************
	void LuaComponent::Update(float DeltaTime) {

		if (lua::CheckLua(m_StateRef, luaL_dofile(m_StateRef, LuaAssetPath.c_str()))) {
			lua_getglobal(m_StateRef, "Update");
			if (lua_isfunction(m_StateRef, -1)) {
				lua_pushlightuserdata(m_StateRef, this->Owner);
				lua_pushnumber(m_StateRef, DeltaTime);
				(lua_pcall(m_StateRef, 2, 0, 0) != 0);
			}
		}

	}

	// ***************************************************************
	void LuaComponent::Render(Renderer* RendererReference) {
		unreferenced(RendererReference);
		CallLuaFunction("Render");
	}

	// ***************************************************************
	void LuaComponent::Destroy() {
		CallLuaFunction("Destroy");
	}

	// ***************************************************************
	void LuaComponent::CallLuaFunction(const std::string& FunctionName, bool bOmitWarnings) {
		if (lua::CheckLua(m_StateRef, luaL_dofile(m_StateRef, LuaAssetPath.c_str()))) {
			lua_getglobal(m_StateRef, FunctionName.c_str());
			if (lua_isfunction(m_StateRef, -1)) {
				if (lua_pcall(m_StateRef, 0, 0, 0) != 0) {
					LOG_WARNING("Couldn't call function {0} on {1}", FunctionName, LuaAssetPath);
				}
			}
			else if(!bOmitWarnings){
				LOG_WARNING("Couldn't find function {0} on {1}", FunctionName, LuaAssetPath);
			}
		}
	}
}