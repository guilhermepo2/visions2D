#pragma once
#include "Component.h"
#include "LuaState.h"

namespace visions2D {

	class LuaComponent : public Component {
	public:
		LuaComponent(const std::string& LuaFilePath);
		~LuaComponent();

		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual bool ProcessInput(const InputState& CurrentInputState) override;
		virtual void Update(float DeltaTime) override;
		virtual void Render(Renderer* RendererReference) override;
		virtual void Destroy() override;

	private:
		void CallLuaFunction(const std::string& FunctionName, bool bOmitWarnings = true);
		lua_State* m_StateRef;
		std::string LuaAssetPath;
	};
}