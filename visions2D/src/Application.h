#pragma once
#include <functional>

template <typename T>
using Function = std::function<T>;
using ApplicationEvent = Function<void()>;
using UpdateEvent = Function<void(float)>;

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