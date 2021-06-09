#pragma once
#include "Common.h"

namespace visions2D {

	class Renderer;
	using RenderEvent = Function<void(Renderer*)>;

	struct AppConfig {
		const char* WindowName;
		int Width;
		int Height;

		ApplicationEvent Startup;
		ApplicationEvent Shutdown;

		ApplicationEvent PreProcessInput;
		ApplicationEvent ProcessInput;

		UpdateEvent Update;

		RenderEvent Render;
		ApplicationEvent ImGuiRender;

	};

	class Application {
	public:
		Application(const AppConfig& Config);
		void Run();

	private:
		AppConfig ConfigBeingUsed;
		Renderer* m_RendererRef; // I guess all applications have to have a renderer, right?!
	};


}