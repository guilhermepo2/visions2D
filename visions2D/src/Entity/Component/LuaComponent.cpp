#include "Log.h"
#include "LuaComponent.h"
#include "Input/Input.h"

namespace visions2D {

	struct MouseEvent {
		float x;
		float y;

		bool LeftButtonWasPressed;
		bool LeftButtonIsHeld;
		bool LeftButtonWasReleased;
	};
	
	// ***************************************************************
	LuaComponent::LuaComponent(const std::string& LuaFilePath) {
		LOG_INFO("LUA COMPONENT ADDED!");

		// one state for every lua script
		// maybe with this I don't need a global lua state?
		m_StateRef = luaL_newstate();
		luaL_openlibs(m_StateRef);
		visions2D::lua::BindScriptFunctions(m_StateRef);

		LuaAssetPath = LuaFilePath;

		if(visions2D::lua::CheckLua(m_StateRef, luaL_dofile(m_StateRef, LuaAssetPath.c_str()))) {
			LOG_INFO("Loaded Script: {0}", LuaAssetPath);
		}
		else {
			LOG_ERROR("Couldn't load script: {0}", LuaAssetPath);
		}

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
		lua_getglobal(m_StateRef, "ProcessInput");

		if (lua_isfunction(m_StateRef, -1)) {
			lua_pushlightuserdata(m_StateRef, this->Owner);

			// mouse state
			MouseEvent e;
			e.x = CurrentInputState.Mouse.GetPosition().x;
			e.y = CurrentInputState.Mouse.GetPosition().y;
			e.LeftButtonWasPressed = CurrentInputState.Mouse.WasMouseKeyPressedThisFrame(v2D_Mousecode::MOUSECODE_LEFT);
			e.LeftButtonIsHeld = CurrentInputState.Mouse.IsMouseKeyDown(v2D_Mousecode::MOUSECODE_LEFT);
			e.LeftButtonWasReleased = CurrentInputState.Mouse.WasKMouseKeyReleasedThisFrame(v2D_Mousecode::MOUSECODE_LEFT);
			lua_newtable(m_StateRef);
			lua_pushnumber(m_StateRef, e.x);
			lua_setfield(m_StateRef, -2, "mouse_x");
			lua_pushnumber(m_StateRef, e.y);
			lua_setfield(m_StateRef, -2, "mouse_y");
			lua_pushboolean(m_StateRef, e.LeftButtonWasPressed);
			lua_setfield(m_StateRef, -2, "left_button_pressed");
			lua_pushboolean(m_StateRef, e.LeftButtonIsHeld);
			lua_setfield(m_StateRef, -2, "left_button_held");
			lua_pushboolean(m_StateRef, e.LeftButtonWasReleased);
			lua_setfield(m_StateRef, -2, "left_button_released");

			lua_pcall(m_StateRef, 2, 0, 0);
			return true;
		}
		
		return false;
	}

	// ***************************************************************
	void LuaComponent::Update(float DeltaTime) {

		lua_getglobal(m_StateRef, "Update");
		if (lua_isfunction(m_StateRef, -1)) {
			lua_pushlightuserdata(m_StateRef, this->Owner);
			lua_pushnumber(m_StateRef, DeltaTime);
			lua_pcall(m_StateRef, 2, 0, 0);
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

		lua_getglobal(m_StateRef, FunctionName.c_str());
		if (lua_isfunction(m_StateRef, -1)) {
			if (lua_pcall(m_StateRef, 0, 0, 0) != 0) {
				LOG_WARNING("Couldn't call function {0} on {1}", FunctionName, LuaAssetPath);
				std::string errormsg = lua_tostring(m_StateRef, -1);
				LOG_ERROR(errormsg);
			}
		}
		else if (!bOmitWarnings) {
			LOG_WARNING("Couldn't find function {0} on {1}", FunctionName, LuaAssetPath);
		}
	}
}