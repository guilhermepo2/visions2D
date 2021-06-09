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
visions2D::CollisionWorld* collisionWorld = nullptr;

visions2D::Texture* characterTexture = nullptr;
visions2D::Texture* mapTexture = nullptr;
visions2D::Texture* shipTexture = nullptr;
visions2D::Texture* meteorTexture = nullptr;

visions2D::Tilesheet* characterTilesheet = nullptr;
visions2D::Tilesheet* mapTilesheet = nullptr;
visions2D::Tilemap* dungeon = nullptr;
visions2D::Entity* Player;
visions2D::Entity* Meteor;

int MovementDirection = 0;
float RotationSpeed = 135.0f;
bool Accelerating = false;
float ShipSpeed = 180.0f;

visions2D::Font* LazyTownFont = nullptr;
visions2D::Font* ArialFont = nullptr;
visions2D::Font* NotoSans = nullptr;
visions2D::Font* SentyWen = nullptr;
visions2D::Texture* HelloWorldTexture = nullptr;
visions2D::Texture* ltText = nullptr;
visions2D::Texture* zhongguoText = nullptr;
visions2D::Texture* kafei = nullptr;

visions2D::Texture* WhiteTexture = nullptr;

void Start() {
	inputSystem = new visions2D::InputSystem();
	gameWorld = new visions2D::GameWorld();
	collisionWorld = new visions2D::CollisionWorld();
	if (collisionWorld->Initialize()) {
		LOG_INFO("app: collision world initialized");
	}

	characterTexture = new visions2D::Texture();
	characterTexture->Load("./src/DefaultAssets/chara_hero.png");

	WhiteTexture = new visions2D::Texture();
	WhiteTexture->Load("./src/DefaultAssets/White.png");

	characterTilesheet = new visions2D::Tilesheet(characterTexture);
	characterTilesheet->Slice(48, 48);

	shipTexture = new visions2D::Texture();
	shipTexture->Load("./src/DefaultAssets/Spaceship/playerShip3_blue.png");

	meteorTexture = new visions2D::Texture();
	meteorTexture->Load("./src/DefaultAssets/Spaceship/meteorBrown_big4.png");

	mapTexture = new visions2D::Texture();
	mapTexture->Load("./src/DefaultAssets/tiles_dungeon_v1.1.png");
	mapTilesheet = new visions2D::Tilesheet(mapTexture);
	mapTilesheet->LoadFromTiledJson("./src/DefaultAssets/Map/dungeon_tiles.json");

	dungeon = new visions2D::Tilemap();
	dungeon->LoadFromJSON("./src/DefaultAssets/Map/dungeon_map.json");

	Player = gameWorld->AddEntity("Player");
	Player->AddComponent<visions2D::TransformComponent>(glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f));
	Player->AddComponent<visions2D::SpriteComponent>(shipTexture, 0);
	Player->AddComponent<visions2D::BoxCollider>("PlayerShip", glm::vec2(0.0f, 0.0f), glm::vec2(60.0f, 60.0f));

	Meteor = gameWorld->AddEntity("Meteor");
	Meteor->AddComponent<visions2D::TransformComponent>(glm::vec2(200.0f, 100.0f), 0.0f, glm::vec2(1.0f, 1.0f));
	Meteor->AddComponent<visions2D::SpriteComponent>(meteorTexture, 0);
	Meteor->AddComponent<visions2D::BoxCollider>("Meteor", glm::vec2(0.0f, 0.0f), glm::vec2(90.0f, 90.0f));

	// TODO: Have a static method to create and load fonts?!
	// TODO: It's probably better to have a centralized "Asset Manager" with Textures, Fonts, etc...
	LazyTownFont = new visions2D::Font();
	ArialFont = new visions2D::Font();
	NotoSans = new visions2D::Font();
	SentyWen = new visions2D::Font();

	LazyTownFont->Load("./src/DefaultAssets/ChevyRay - Lazytown.ttf");
	ArialFont->Load("./src/DefaultAssets/Arial.ttf");
	NotoSans->Load("./src/DefaultAssets/NotoSansSC-Light.otf");
	SentyWen->Load("./src/DefaultAssets/SentyWEN2017.ttf");

	HelloWorldTexture = LazyTownFont->RenderToTexture("Hello World", 12);

	const char* LatinXText = "olÃ¡, como estÃ¡s?";
	ltText = ArialFont->RenderToTexture(LatinXText, 12);

	Uint16 ILikeCoffee_Unicode[32] = { 0x6211,0x559c,0x6b22,0x5496,0x5561, 0 };
	Uint16 Hello_UnicodeHex[1024] = { 0x4F60,0x597D, 0 }; //= (Hexadecimal) Unicode encoding: Hello
	Uint16 Hello_UnicodeDec[1024] = { 20320, 22909, 0 }; //= (Decimal) Unicode encoding: Hello
	const char* Hello_UTF8 = u8"\u4F60\u597D";
	const char* ILikeCoffee_UTF8 = u8"\u6211\u559c\u6b22\u5496\u5561\0";

	zhongguoText = NotoSans->RenderToTexture(Hello_UTF8, 40);
	kafei = SentyWen->RenderToTexture(ILikeCoffee_UTF8, 64);

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
	
	visions2D::TransformComponent* playerTransform = Player->GetComponentOfType<visions2D::TransformComponent>();

	if (Accelerating) {
		
		float XSpeed = ShipSpeed * glm::sin(glm::radians(playerTransform->Rotation));
		float YSpeed = ShipSpeed * glm::cos(glm::radians(playerTransform->Rotation));
		playerTransform->Translate(glm::vec2(XSpeed, YSpeed) * DeltaTime);
	}

	playerTransform->Rotate(MovementDirection * RotationSpeed * DeltaTime);
	gameWorld->Update(DeltaTime);
	SandboxStats_Data.LastDeltaTime = DeltaTime;

	collisionWorld->VerifyAllCollisions();
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
		rd.WorldPosition = glm::vec2(200.0f, 0.0f);
		// turns out fonts are upside down... wtf...
		rd.WorldScale = glm::vec2(1.0f, -1.0f);
		RendererReference->SpriteRenderData.push_back(rd);
	}

	// Rendering Player
	if (Player->HasComponentOfType<visions2D::TransformComponent>() && Player->HasComponentOfType<visions2D::SpriteComponent>()) {
		visions2D::RenderData rd;
		visions2D::SpriteComponent* s = Player->GetComponentOfType<visions2D::SpriteComponent>();
		rd.Texture = s->tex;

		rd.TextureScale = glm::vec2(shipTexture->GetWidth(), shipTexture->GetHeight());
		rd.TexCoords = nullptr;
		rd.WorldRotation = Player->GetComponentOfType<visions2D::TransformComponent>()->Rotation;
		rd.WorldPosition = Player->GetComponentOfType<visions2D::TransformComponent>()->Position;
		rd.WorldScale = Player->GetComponentOfType<visions2D::TransformComponent>()->Scale;
		rd.tint = visions2D::Color(1.0f, 1.0f, 1.0f, 1.0f);
		RendererReference->SpriteRenderData.push_back(rd);
	}

	if (Player->HasComponentOfType<visions2D::TransformComponent>() && Player->HasComponentOfType<visions2D::BoxCollider>()) {
		visions2D::RenderData rd;
		visions2D::TransformComponent* t = Player->GetComponentOfType<visions2D::TransformComponent>();
		visions2D::BoxCollider* b = Player->GetComponentOfType<visions2D::BoxCollider>();
		visions2D::Math::Rectangle rect = b->GetWorldPositionRectangle();

		rd.Texture = WhiteTexture;
		rd.TextureScale = glm::vec2(rect.Width(), rect.Height());
		rd.TexCoords = nullptr;
		rd.WorldRotation = 0;
		rd.WorldPosition = glm::vec2(rect.Position().x, rect.Top() - rect.Height());
		rd.WorldScale = t->Scale;
		rd.tint = visions2D::Color(1.0f, 0.0f, 0.0f, 0.5f);
		RendererReference->SpriteRenderData.push_back(rd);
	}

	// Rendering Meteor
	if (Meteor->HasComponentOfType<visions2D::TransformComponent>() && Meteor->HasComponentOfType<visions2D::SpriteComponent>()) {
		visions2D::RenderData rd;
		visions2D::SpriteComponent* s = Meteor->GetComponentOfType<visions2D::SpriteComponent>();
		rd.Texture = s->tex;

		rd.TextureScale = glm::vec2(meteorTexture->GetWidth(), meteorTexture->GetHeight());
		rd.TexCoords = nullptr;
		rd.WorldRotation = Meteor->GetComponentOfType<visions2D::TransformComponent>()->Rotation;
		rd.WorldPosition = Meteor->GetComponentOfType<visions2D::TransformComponent>()->Position;
		rd.WorldScale = Meteor->GetComponentOfType<visions2D::TransformComponent>()->Scale;
		rd.tint = visions2D::Color(1.0f, 1.0f, 1.0f, 1.0f);
		RendererReference->SpriteRenderData.push_back(rd);
	}

	if (Meteor->HasComponentOfType<visions2D::TransformComponent>() && Meteor->HasComponentOfType<visions2D::BoxCollider>()) {
		visions2D::RenderData rd;
		visions2D::TransformComponent* t = Meteor->GetComponentOfType<visions2D::TransformComponent>();
		visions2D::BoxCollider* b = Meteor->GetComponentOfType<visions2D::BoxCollider>();
		visions2D::Math::Rectangle rect = b->GetWorldPositionRectangle();

		rd.Texture = WhiteTexture;
		rd.TextureScale = glm::vec2(rect.Width(), rect.Height());
		rd.TexCoords = nullptr;
		rd.WorldRotation = 0;
		rd.WorldPosition = glm::vec2(rect.Position().x, rect.Top() - rect.Height());
		rd.WorldScale = t->Scale;
		rd.tint = visions2D::Color(1.0f, 0.0f, 0.0f, 0.5f);
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