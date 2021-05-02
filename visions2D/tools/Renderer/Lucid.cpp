// Lucid 2D renderer Standalone

#include "visions2D.h"

int main(void) {
	Log::Initialize();
	LOG_INFO("logger initialized!");

	visions2D::Renderer* lucidRenderer = new visions2D::Renderer();
	if (lucidRenderer->Initialize(1024, 576, "lucid2d renderer")) {
		bool b_IsRunning = true;

		while (b_IsRunning) {
			SDL_Event Event;
			while (SDL_PollEvent(&Event)) {

				ImGui_ImplSDL2_ProcessEvent(&Event);

				switch (Event.type) {
				case SDL_QUIT:
					b_IsRunning = false;
					break;
				}
			}

			lucidRenderer->Render();
		}

		lucidRenderer->Shutdown();
	}

	return 0;
}