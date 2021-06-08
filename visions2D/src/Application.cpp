#include "Application.h"
#include "Renderer/Renderer.h"
#include "Renderer/DearImGui.h"
#include "Log.h"

const unsigned int FPS = 60;
const unsigned int FRAME_TARGET_TIME = 1000 / FPS;

namespace visions2D {
	

	Application::Application(const AppConfig& Config) {
		ConfigBeingUsed = Config;
	}


	void Application::Run() {
		Log::Initialize();
		LOG_INFO("[application] logger initialized!");

		// Initialize Renderer
		// Run game loop, also with the defined updates on the application.

		bool bIsRunning = false;
		unsigned int TicksLastFrame = 0;
		m_RendererRef = new Renderer();
		if (m_RendererRef->Initialize(static_cast<float>(ConfigBeingUsed.Width), static_cast<float>(ConfigBeingUsed.Height), ConfigBeingUsed.WindowName)) {
			bIsRunning = true;

			if (ConfigBeingUsed.Startup != nullptr) {
				ConfigBeingUsed.Startup();
			}

			while (bIsRunning) {
				// -----------------------------------------------------------------
				float DeltaTime = (static_cast<float>((SDL_GetTicks() - TicksLastFrame))) / 1000.0f;
				DeltaTime = (DeltaTime > 0.05f) ? 0.05f : DeltaTime;
				TicksLastFrame = SDL_GetTicks();
				// -----------------------------------------------------------------

				if (ConfigBeingUsed.PreProcessInput != nullptr) {
					ConfigBeingUsed.PreProcessInput();
				}

				SDL_Event Event;
				while (SDL_PollEvent(&Event)) {
					DearImGui::ProcessEvent(&Event);

					switch (Event.type) {
					case SDL_QUIT:
						bIsRunning = false;
						break;
					case SDL_KEYDOWN:
						if (Event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
							bIsRunning = false;
						}
						break;
					}
				}

				if (ConfigBeingUsed.ProcessInput != nullptr) {
					ConfigBeingUsed.ProcessInput();
				}

				if (ConfigBeingUsed.Update != nullptr) {
					ConfigBeingUsed.Update(DeltaTime);
				}

				m_RendererRef->PrepareToRender();
				
				if (ConfigBeingUsed.Render != nullptr) {
					ConfigBeingUsed.Render(m_RendererRef);
				}

				m_RendererRef->Render();

				if (ConfigBeingUsed.ImGuiRender != nullptr) {
					ConfigBeingUsed.ImGuiRender();
				}

				m_RendererRef->Swap();

				// -----------------------------------------------------------------
				int TimeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - TicksLastFrame);
				if (TimeToWait > 0.0f && TimeToWait <= FRAME_TARGET_TIME) {
					SDL_Delay(TimeToWait);
				}
			}
		}

		m_RendererRef->Shutdown();
		delete m_RendererRef;
	}
}