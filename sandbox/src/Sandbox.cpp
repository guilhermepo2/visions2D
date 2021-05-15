// *******************************************************
// *******************************************************
// 
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

	float DefaultTexCoords[] = {
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f
	};

	if (sandbox->Initialize(1024, 576, "sandbox")) {

		visions2D::Texture* characterTexture = new visions2D::Texture();
		characterTexture->Load("./src/DefaultAssets/chara_hero.png");
		visions2D::Tilesheet* characterTilesheet = new visions2D::Tilesheet(characterTexture);
		characterTilesheet->Slice(48, 48);

		visions2D::Texture* mapTexture = new visions2D::Texture();
		mapTexture->Load("./src/DefaultAssets/tiles_dungeon_v1.1.png");
		visions2D::Tilesheet* mapTilesheet = new visions2D::Tilesheet(mapTexture);
		mapTilesheet->LoadFromTiledJson("./src/DefaultAssets/Map/dungeon_tiles.json");
		visions2D::Tilemap* dungeon = new visions2D::Tilemap();
		dungeon->LoadFromJSON("./src/DefaultAssets/Map/dungeon_map.json");

		inputSystem->Initialize();

		bool b_IsRunning = true;

		visions2D::Entity& Player = gameWorld->AddEntity("Player");
		Player.AddComponent<visions2D::TransformComponent>(glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f));
		Player.AddComponent<visions2D::TileComponent>(characterTilesheet, 1, 0);

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

			// This should be a player component!
			// translating the transform with input should be a scriptable thing
			if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_D)) {
				Player.GetComponentOfType<visions2D::TransformComponent>()->Translate(glm::vec2(16.0f, 0.0f));
			}
			else if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_A)) {
				Player.GetComponentOfType<visions2D::TransformComponent>()->Translate(glm::vec2(-16.0f, 0.0f));
			}
			else if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_W)) {
				Player.GetComponentOfType<visions2D::TransformComponent>()->Translate(glm::vec2(0.0f, 16.0f));
			}
			else if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_S)) {
				Player.GetComponentOfType<visions2D::TransformComponent>()->Translate(glm::vec2(0.0f, -16.0f));
			}

			gameWorld->Update(DeltaTime);
			sandbox->PrepareToRender();
			gameWorld->Render();

			int currentData = 0;
			float StartingX = -(dungeon->GetMapWidth() / 2) * mapTilesheet->GetTileWidth();
			float StartingY = (dungeon->GetMapHeight() / 2) * mapTilesheet->GetTileHeight();
			glm::vec2 Position = glm::vec2(StartingX, StartingY);
			int data;

			for (int i = 0; i < dungeon->GetMapWidth(); i++) {
				for (int j = 0; j < dungeon->GetMapHeight(); j++) {
					visions2D::RenderData rd;
					rd.Texture = mapTexture;
					rd.TextureScale = glm::vec2(mapTilesheet->GetTileWidth(), mapTilesheet->GetTileHeight());
					data = dungeon->GetData(currentData);
					rd.TexCoords = mapTilesheet->GetTexCoordsFromId(data);

					rd.WorldRotation = 0.0f;
					rd.WorldPosition = glm::vec2(Position.x, Position.y);
					rd.WorldScale = glm::vec2(1.0f, 1.0f);

					rd.tint = visions2D::Color(1.0f, 1.0f, 1.0f, 1.0f);

					sandbox->SpriteRenderData.push_back(rd);

					Position.x += mapTilesheet->GetTileWidth();
					currentData++;
				}
				Position.y -= mapTilesheet->GetTileHeight();
				Position.x = StartingX;
			}

			// ----------------------------------------------------------------------------------------------------------------------------
			// ----------------------------------------------------------------------------------------------------------------------------
			// Move this to game world render or something like that
			if (Player.HasComponentOfType<visions2D::TransformComponent>() && Player.HasComponentOfType<visions2D::TileComponent>()) {
				visions2D::RenderData rd;
				visions2D::TileComponent* t = Player.GetComponentOfType<visions2D::TileComponent>();

				// get this from the tilecomponent	//
				rd.Texture = characterTexture;		//
				// -------------------------------	//

				rd.TextureScale = glm::vec2(characterTilesheet->GetTileWidth(), characterTilesheet->GetTileHeight());
				rd.TexCoords = characterTilesheet->GetTexCoordsFromId(t->Data);
				rd.WorldRotation = Player.GetComponentOfType<visions2D::TransformComponent>()->Rotation;
				rd.WorldPosition = Player.GetComponentOfType<visions2D::TransformComponent>()->Position;
				rd.WorldScale = Player.GetComponentOfType<visions2D::TransformComponent>()->Scale;
				rd.tint = visions2D::Color(1.0f, 1.0f, 1.0f, 1.0f);
				sandbox->SpriteRenderData.push_back(rd);
			}
			// ----------------------------------------------------------------------------------------------------------------------------
			// ----------------------------------------------------------------------------------------------------------------------------

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