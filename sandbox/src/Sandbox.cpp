// *******************************************************
// *******************************************************
// 
// Lucid 2D renderer Standalone
// 
// *******************************************************
// *******************************************************

#include "visions2D.h"
const unsigned int FPS = 60;
const unsigned int FRAME_TARGET_TIME = 1000 / FPS;

int main(void) {
	Log::Initialize();
	LOG_INFO("logger initialized!");

	visions2D::Renderer* sandbox = new visions2D::Renderer();
	visions2D::InputSystem* inputSystem = new visions2D::InputSystem();
	visions2D::GameWorld* gameWorld = new visions2D::GameWorld();

	if (sandbox->Initialize(1024, 576, "sandbox")) {
		inputSystem->Initialize();

		bool b_IsRunning = true;

		gameWorld->BeginPlay();
		float TicksLastFrame = 0.0f;
		while (b_IsRunning) {
			// -----------------------------------------------------------------
			float DeltaTime = (SDL_GetTicks() - TicksLastFrame) / 1000.0f;
			DeltaTime = (DeltaTime > 0.05f) ? 0.05f : DeltaTime;
			TicksLastFrame = SDL_GetTicks();
			// -----------------------------------------------------------------

			inputSystem->PrepareForUpdate();

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

			inputSystem->Update();
			gameWorld->ProcessInput(inputSystem->GetState());

			if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_SPACE)) {
				LOG_INFO("spacebar was pressed and the input system works!");
			}

			gameWorld->Update(DeltaTime);

			sandbox->PrepareToRender();
			gameWorld->Render();
			sandbox->Render();
			sandbox->Swap();

			// -----------------------------------------------------------------
			int TimeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - TicksLastFrame);
			if (TimeToWait > 0.0f && TimeToWait <= FRAME_TARGET_TIME) {
				SDL_Delay(TimeToWait);
			}
		}

		gameWorld->Destroy();
		inputSystem->Shutdown();
		sandbox->Shutdown();
	}

	delete gameWorld;
	delete inputSystem;
	delete sandbox;
	return 0;
}