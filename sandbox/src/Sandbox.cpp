// *******************************************************
// *******************************************************
// 
// 
// 
// *******************************************************
// *******************************************************

#include "visions2D.h"
#include <SDL_ttf.h>

static struct {
	float LastDeltaTime;
} SandboxStats_Data;

visions2D::InputSystem* inputSystem = nullptr;
visions2D::GameWorld* gameWorld = nullptr;
visions2D::Texture* characterTexture = nullptr;
visions2D::Texture* mapTexture = nullptr;
visions2D::Texture* shipTexture = nullptr;

visions2D::Tilesheet* characterTilesheet = nullptr;
visions2D::Tilesheet* mapTilesheet = nullptr;
visions2D::Tilemap* dungeon = nullptr;
visions2D::Entity Player;

int MovementDirection = 0;
float RotationSpeed = 135.0f;
bool Accelerating = false;
float ShipSpeed = 180.0f;

visions2D::Texture* HelloWorldTexture = nullptr;
visions2D::Texture* ltText = nullptr;
visions2D::Texture* zhongguoText = nullptr;
visions2D::Texture* kafei = nullptr;

void Start() {
	inputSystem = new visions2D::InputSystem();
	gameWorld = new visions2D::GameWorld();

	characterTexture = new visions2D::Texture();
	characterTexture->Load("./src/DefaultAssets/chara_hero.png");

	characterTilesheet = new visions2D::Tilesheet(characterTexture);
	characterTilesheet->Slice(48, 48);

	shipTexture = new visions2D::Texture();
	shipTexture->Load("./src/DefaultAssets/Spaceship/playerShip3_blue.png");

	mapTexture = new visions2D::Texture();
	mapTexture->Load("./src/DefaultAssets/tiles_dungeon_v1.1.png");
	mapTilesheet = new visions2D::Tilesheet(mapTexture);
	mapTilesheet->LoadFromTiledJson("./src/DefaultAssets/Map/dungeon_tiles.json");

	dungeon = new visions2D::Tilemap();
	dungeon->LoadFromJSON("./src/DefaultAssets/Map/dungeon_map.json");

	Player = gameWorld->AddEntity("Player");
	Player.AddComponent<visions2D::TransformComponent>(glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f));
	Player.AddComponent<visions2D::SpriteComponent>(shipTexture, 0);

	// This is being done purely to test out putting a font in the screen!
	TTF_Font* LazytownFont = TTF_OpenFont("./src/DefaultAssets/ChevyRay - Lazytown.ttf", 12);
	if (LazytownFont == nullptr) {
		LOG_WARNING("Couldn't load lazytown font!");
	}
	HelloWorldTexture = new visions2D::Texture();
	SDL_Color color;
	color.r = 255; color.g = 255; color.b = 255; color.a = 255;
	SDL_Surface* surf = TTF_RenderUTF8_Blended(LazytownFont, "Hello World", color);
	if (surf != nullptr) {
		HelloWorldTexture->CreateFromSurface(surf);
		SDL_FreeSurface(surf);
	}
	// Font should be ready to render now!

	const char* LatinXText = "olÃ¡, como estÃ¡s?";
	TTF_Font* Arial = TTF_OpenFont("./src/DefaultAssets/Arial.ttf", 12);
	if (Arial == nullptr) {
		LOG_WARNING("SDL2_TTF is racist!");
	}
	SDL_Surface* surf2 = TTF_RenderUTF8_Blended(Arial, LatinXText, color);
	if (surf == nullptr) {
		LOG_WARNING("Couldn't create texture for {0}", LatinXText);
	}

	if (surf2 != nullptr) {
		
		ltText = new visions2D::Texture();
		ltText->CreateFromSurface(surf2);
		SDL_FreeSurface(surf2);
	}


	Uint16 NoWay[32] = { 0x6211,0x559c,0x6b22,0x5496,0x5561, 0 }; // wo xihuan kafei
	Uint16 msg[1024] = { 0x4F60,0x597D, 0 }; //= Unicode encoding: Hello
	TTF_Font* NotoSans = TTF_OpenFont("./src/DefaultAssets/NotoSansSC-Light.otf", 40);
	TTF_Font* SentyWen = TTF_OpenFont("./src/DefaultAssets/SentyWEN2017.ttf", 40);
	SDL_Surface* surf3 = TTF_RenderUNICODE_Blended(NotoSans, NoWay, color);
	SDL_Surface* surf4 = TTF_RenderUNICODE_Blended(SentyWen, msg, color);

	if (surf3 != nullptr) {
		zhongguoText = new visions2D::Texture();
		zhongguoText->CreateFromSurface(surf3);
		SDL_FreeSurface(surf3);
	}

	if (surf4 != nullptr) {
		kafei = new visions2D::Texture();
		kafei->CreateFromSurface(surf4);
		SDL_FreeSurface(surf4);
	}

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

	MovementDirection = 0;
	if (inputSystem->GetState().Keyboard.IsKeyDown(visions2D::v2D_Keycode::KEYCODE_D)) {
		MovementDirection += 1;
	}
	else if (inputSystem->GetState().Keyboard.IsKeyDown(visions2D::v2D_Keycode::KEYCODE_A)) {
		MovementDirection -= 1;
	}

	if (inputSystem->GetState().Mouse.IsMouseKeyDown(visions2D::v2D_Mousecode::MOUSECODE_LEFT)) {
		Accelerating = true;
	}
	else {
		Accelerating = false;
	}
}

void Update(float DeltaTime) {
	
	visions2D::TransformComponent* playerTransform = Player.GetComponentOfType<visions2D::TransformComponent>();

	if (Accelerating) {
		
		float XSpeed = ShipSpeed * glm::sin(glm::radians(playerTransform->Rotation));
		float YSpeed = ShipSpeed * glm::cos(glm::radians(playerTransform->Rotation));
		playerTransform->Translate(glm::vec2(XSpeed, YSpeed) * DeltaTime);
	}

	playerTransform->Rotate(MovementDirection * RotationSpeed * DeltaTime);
	gameWorld->Update(DeltaTime);
	SandboxStats_Data.LastDeltaTime = DeltaTime;
}

void Render(visions2D::Renderer* RendererReference) {
	gameWorld->Render();

	
	{
		visions2D::RenderData rd;
		rd.Texture = HelloWorldTexture;
		rd.TextureScale = glm::vec2(HelloWorldTexture->GetWidth(), HelloWorldTexture->GetHeight());
		rd.TexCoords = nullptr;
		rd.WorldRotation = 0.0f;
		rd.WorldPosition = glm::vec2(200.0f, 200.0f);
		// turns out fonts are upside down... wtf...
		rd.WorldScale = glm::vec2(1.0f, -1.0f);
		RendererReference->SpriteRenderData.push_back(rd);
	}
	

	{
		visions2D::RenderData rd;
		rd.Texture = ltText;
		rd.TextureScale = glm::vec2(ltText->GetWidth(), ltText->GetHeight());
		rd.TexCoords = nullptr;
		rd.WorldRotation = 0.0f;
		rd.WorldPosition = glm::vec2(200.0f, 150.0f);
		// turns out fonts are upside down... wtf...
		rd.WorldScale = glm::vec2(1.0f, -1.0f);
		RendererReference->SpriteRenderData.push_back(rd);
	}

	{
		visions2D::RenderData rd;
		rd.Texture = zhongguoText;
		rd.TextureScale = glm::vec2(zhongguoText->GetWidth(), zhongguoText->GetHeight());
		rd.TexCoords = nullptr;
		rd.WorldRotation = 0.0f;
		rd.WorldPosition = glm::vec2(200.0f, 100.0f);
		// turns out fonts are upside down... wtf...
		rd.WorldScale = glm::vec2(1.0f, -1.0f);
		RendererReference->SpriteRenderData.push_back(rd);
	}

	{
		visions2D::RenderData rd;
		rd.Texture = kafei;
		rd.TextureScale = glm::vec2(kafei->GetWidth(), kafei->GetHeight());
		rd.TexCoords = nullptr;
		rd.WorldRotation = 0.0f;
		rd.WorldPosition = glm::vec2(200.0f, 50.0f);
		// turns out fonts are upside down... wtf...
		rd.WorldScale = glm::vec2(1.0f, -1.0f);
		RendererReference->SpriteRenderData.push_back(rd);
	}


	if (Player.HasComponentOfType<visions2D::TransformComponent>() && Player.HasComponentOfType<visions2D::SpriteComponent>()) {
		visions2D::RenderData rd;
		visions2D::SpriteComponent* s = Player.GetComponentOfType<visions2D::SpriteComponent>();
		rd.Texture = s->tex;

		rd.TextureScale = glm::vec2(shipTexture->GetWidth(), shipTexture->GetHeight());
		rd.TexCoords = nullptr;
		rd.WorldRotation = Player.GetComponentOfType<visions2D::TransformComponent>()->Rotation;
		rd.WorldPosition = Player.GetComponentOfType<visions2D::TransformComponent>()->Position;
		rd.WorldScale = Player.GetComponentOfType<visions2D::TransformComponent>()->Scale;
		rd.tint = visions2D::Color(1.0f, 1.0f, 1.0f, 1.0f);
		RendererReference->SpriteRenderData.push_back(rd);
	}
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