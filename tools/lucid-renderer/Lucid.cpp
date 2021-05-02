// *******************************************************
// *******************************************************
// 
// Lucid 2D renderer Standalone
// 
// *******************************************************
// *******************************************************

#include "visions2D.h"

int main(void) {
	Log::Initialize();
	LOG_INFO("logger initialized!");

	visions2D::Renderer* lucidRenderer = new visions2D::Renderer();

	std::string a = "aahoy";

	if (lucidRenderer->Initialize(1024, 576, "lucid2d renderer")) {
		bool b_IsRunning = true;

		while (b_IsRunning) {
			SDL_Event Event;
			while (SDL_PollEvent(&Event)) {

				visions2D::DearImGui::ProcessEvent(&Event);

				switch (Event.type) {
				case SDL_QUIT:
					b_IsRunning = false;
					break;
				case SDL_KEYDOWN:
					if (Event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
						b_IsRunning = false;
					}
					break;
				}

			}

			lucidRenderer->Render();
		}

		lucidRenderer->Shutdown();
	}

	delete lucidRenderer;
	return 0;
}