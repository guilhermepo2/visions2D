// *******************************************************
// *******************************************************
// 
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

	visions2D::Texture* characterTexture = new visions2D::Texture();
	characterTexture->Load("./src/DefaultAssets/chara_hero.png");

	visions2D::Color textureColor;

	float DefaultTexCoords[] = {
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f
	};

	if (sandbox->Initialize(1024, 576, "sandbox")) {
		visions2D::FramebufferSpecification spec;
		spec.Width = 1024;
		spec.Height = 576;
		visions2D::Framebuffer* theFrameBuffer = nullptr;
		theFrameBuffer = new visions2D::Framebuffer(spec);

		inputSystem->Initialize();

		bool b_IsRunning = true;

		visions2D::Entity& Player = gameWorld->AddEntity("Player");
		Player.AddComponent<visions2D::TransformComponent>(glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f));

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

			if ( inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_SPACE) ) {
				LOG_INFO("spacebar was pressed and the input system works!");
			}

			gameWorld->Update(DeltaTime);

			sandbox->PrepareToRender();
			// theFrameBuffer->Bind();
			glClear(GL_COLOR_BUFFER_BIT);
			glEnable(GL_BLEND);
			glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

			// gameWorld->Render();

			visions2D::RenderData rd;
			rd.Texture = characterTexture;
			rd.TextureScale = glm::vec2(characterTexture->GetWidth(), characterTexture->GetHeight());
			rd.TexCoords = DefaultTexCoords;
			rd.WorldRotation = Player.GetComponentOfType<visions2D::TransformComponent>()->Rotation;
			rd.WorldPosition = Player.GetComponentOfType<visions2D::TransformComponent>()->Position;
			rd.WorldScale = Player.GetComponentOfType<visions2D::TransformComponent>()->Scale;
			rd.tint = textureColor;
			sandbox->SpriteRenderData.push_back(rd);

			sandbox->Render();
			// theFrameBuffer->Unbind();

			{
				ImGui::Begin("Scene");
				// showing the framebuffer with dear imgui
				unsigned int FramebufferTexture = theFrameBuffer->GetColorAttachmentID();
				ImGui::Image((void*)FramebufferTexture, ImVec2{ 800, 600 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				ImGui::End();
			}

			{
				ImGui::Begin("Color Options");
				ImGui::ColorEdit4("square color", textureColor.rgba);
				ImGui::ColorEdit4("clear color", sandbox->GetCamera()->CameraBackgroundColor.rgba);
				ImGui::End();
			}

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