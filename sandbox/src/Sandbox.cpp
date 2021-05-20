// *******************************************************
// *******************************************************
// 
// 
// 
// *******************************************************
// *******************************************************

#include "visions2D.h"

static struct {
	float LastDeltaTime;
} SandboxStats_Data;

visions2D::InputSystem* inputSystem = nullptr;
visions2D::GameWorld* gameWorld = nullptr;
visions2D::Texture* characterTexture = nullptr;
visions2D::Texture* mapTexture = nullptr;

visions2D::Tilesheet* characterTilesheet = nullptr;
visions2D::Tilesheet* mapTilesheet = nullptr;
visions2D::Tilemap* dungeon = nullptr;
visions2D::Entity Player;

void Start() {
	inputSystem = new visions2D::InputSystem();
	gameWorld = new visions2D::GameWorld();

	characterTexture = new visions2D::Texture();
	characterTexture->Load("./src/DefaultAssets/chara_hero.png");

	characterTilesheet = new visions2D::Tilesheet(characterTexture);
	characterTilesheet->Slice(48, 48);

	mapTexture = new visions2D::Texture();
	mapTexture->Load("./src/DefaultAssets/tiles_dungeon_v1.1.png");
	mapTilesheet = new visions2D::Tilesheet(mapTexture);
	mapTilesheet->LoadFromTiledJson("./src/DefaultAssets/Map/dungeon_tiles.json");

	dungeon = new visions2D::Tilemap();
	dungeon->LoadFromJSON("./src/DefaultAssets/Map/dungeon_map.json");

	Player = gameWorld->AddEntity("Player");
	Player.AddComponent<visions2D::TransformComponent>(glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f));
	Player.AddComponent<visions2D::TileComponent>(characterTilesheet, 1, 0);

	inputSystem->Initialize();
	gameWorld->BeginPlay();
}

void PreInput() {
	inputSystem->PrepareForUpdate();
}

void Input() {
	inputSystem->Update();
	gameWorld->ProcessInput(inputSystem->GetState());

	if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_SPACE)) {
		LOG_INFO("spacebar was pressed and the input system works!");
	}

	// This should be a player component!
	// translating the transform with input should be a scriptable thing
	if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_D)) {
		Player.GetComponentOfType<visions2D::TransformComponent>()->Translate(glm::vec2(16.0f, 0.0f));
		Player.GetComponentOfType<visions2D::TransformComponent>()->SetScaleXSign(1);
	}
	else if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_A)) {
		Player.GetComponentOfType<visions2D::TransformComponent>()->Translate(glm::vec2(-16.0f, 0.0f));
		Player.GetComponentOfType<visions2D::TransformComponent>()->SetScaleXSign(-1);
	}
	else if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_W)) {
		Player.GetComponentOfType<visions2D::TransformComponent>()->Translate(glm::vec2(0.0f, 16.0f));
	}
	else if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_S)) {
		Player.GetComponentOfType<visions2D::TransformComponent>()->Translate(glm::vec2(0.0f, -16.0f));
	}
}

void Update(float DeltaTime) {
	gameWorld->Update(DeltaTime);
	SandboxStats_Data.LastDeltaTime = DeltaTime;
}

void Render(visions2D::Renderer* RendererReference) {
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

			RendererReference->SpriteRenderData.push_back(rd);

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
		RendererReference->SpriteRenderData.push_back(rd);
	}
	// ----------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------
}

void OnImGui() {
	{
		ImGui::Begin("Stats");

		ImGui::Text("Last Delta Time: ");
		ImGui::SameLine();
		ImGui::Text("%.2f", SandboxStats_Data.LastDeltaTime);

		ImGui::Text("Estimated FPS based on Delta Time: ");
		ImGui::SameLine();
		ImGui::Text("%.2f", (1.0f / SandboxStats_Data.LastDeltaTime));

		ImGui::End();
	}
}

void Shutdown() {
	gameWorld->Destroy();
	inputSystem->Shutdown();
	delete gameWorld;
	delete inputSystem;
}

int main(void) {

	visions2D::AppConfig conf;
	conf.WindowName = "sandbox";
	conf.Width = 1024;
	conf.Height = 576;

	conf.Startup = Start;
	conf.PreProcessInput = PreInput;
	conf.ProcessInput = Input;
	conf.ImGuiRender = OnImGui;
	conf.Update = Update;
	conf.Render = Render;

	// maybe I should have a static Application::Run(AppConfig) ?!
	visions2D::Application* app = new visions2D::Application(conf);
	app->Run();

	return 0;
}