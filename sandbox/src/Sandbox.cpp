// *******************************************************
// *******************************************************
// 
// 
// 
// *******************************************************
// *******************************************************

/*
- next steps
1. be able to restart the game after you lose
2. particles

- refactoring
1. make a asset store or resource manager that stores textures, fonts, etc...
2. make a game application that will have input system, game world, collision world, etc...
3. load entities and components from .json files
*/

#include <visions2D.h>
#include "PlayerInputComponent.h"

#include <glm/gtc/random.hpp>

// TODO: This should move to a GameApplication class
visions2D::InputSystem* inputSystem = nullptr;
visions2D::GameWorld* gameWorld = nullptr;
visions2D::CollisionWorld* collisionWorld = nullptr;
bool bRenderCollision = false;
bool bIsPaused = false;

void SetIsPaused(bool value) {
	bIsPaused = value;
}

// TODO: this should be in a "Asset Store" or "Resource Manager"
visions2D::Texture* PlayerTexture = nullptr;
visions2D::Texture* ObstacleTexture = nullptr;
visions2D::Texture* PointTexture = nullptr;
visions2D::Font* LazyTown = nullptr;

// Entities... Ha
visions2D::Entity* PlayerEntity = nullptr;
visions2D::Entity* LowerCollider = nullptr;
visions2D::Entity* UpperCollider = nullptr;

// int for the points on screen
int CachedPointsValue = 0;

class ObstacleComponent : public visions2D::Component {
public:

	void BeginPlay() override {
		m_Transform = Owner->GetComponentOfType<visions2D::TransformComponent>();
	}

	void Update(float DeltaTime) override {
		if (m_Transform->Position.x <= m_OutOfScreenToTheLeft) {
			float NewY = glm::linearRand(-1.0f, 1.0f) * m_MaxObstacleY;

			m_Transform->Position.x = m_OutOfScreenToTheRight;
			m_Transform->Position.y = NewY;
		}

		m_Transform->Translate(glm::vec2(m_Velocity * DeltaTime, 0.0f));
	}

private:
	float m_Velocity = -150.0f;
	float m_OutOfScreenToTheRight = 390.0f;
	float m_OutOfScreenToTheLeft = -390.0f;
	float m_MaxObstacleY = 90.0f;
	visions2D::TransformComponent* m_Transform;
};

void CreateObstacleAt(std::string obstacleName, std::string pointsName, float x, float y) {
	visions2D::Entity* NewObstacleEntity = nullptr;
	visions2D::Entity* NewPointsEntity = nullptr;

	NewObstacleEntity = gameWorld->AddEntity(obstacleName);
	NewObstacleEntity->AddComponent<visions2D::TransformComponent>(glm::vec2(x, y), 0.0f, glm::vec2(1.0f, 1.0f));
	visions2D::SpriteComponent& obstacleSprite = NewObstacleEntity->AddComponent<visions2D::SpriteComponent>(ObstacleTexture, 0);
	obstacleSprite.SpriteColor.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	// TODO: Add the option to offset a box collider
	// TODO: Make the box collider render the right box positions
	NewObstacleEntity->AddComponent<visions2D::BoxCollider>("obstacle", glm::vec2(-50.0f, -70.0f), glm::vec2(50.0f, 70.0f));
	NewObstacleEntity->AddComponent<ObstacleComponent>();

	NewPointsEntity = gameWorld->AddEntity(pointsName);
	NewPointsEntity->AddComponent<visions2D::TransformComponent>(glm::vec2(x, y), 0.0f, glm::vec2(1.0f, 1.0f));
	NewPointsEntity->AddComponent<visions2D::BoxCollider>("point-collider", glm::vec2(-5.0f, -300.0f), glm::vec2(5.0f, 300.0f));
	NewPointsEntity->AddComponent<ObstacleComponent>();
}

void CreateEntities() {
	// creating entities
	PlayerEntity = gameWorld->AddEntity("player-entity");
	PlayerEntity->AddComponent<visions2D::TransformComponent>(glm::vec2(-200.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f));
	visions2D::SpriteComponent& spriteComponent = PlayerEntity->AddComponent<visions2D::SpriteComponent>(PlayerTexture, 0);
	spriteComponent.SpriteColor.SetColor(0.0f, 1.0f, 0.0f, 1.0f);
	spriteComponent.FlipVertical = true;
	PlayerEntity->AddComponent<visions2D::BoxCollider>("PlayerCollider", glm::vec2(-12.0f, -12.0f), glm::vec2(12.0f, 12.0f));
	PlayerEntity->AddComponent<PlayerInput>();

	CreateObstacleAt("obstacle1", "points1", 300.0f, 0.0f);
	CreateObstacleAt("obstacle2", "points2", 700.0f, 100.0f);

	UpperCollider = gameWorld->AddEntity("upper collider");
	UpperCollider->AddComponent<visions2D::TransformComponent>(glm::vec2(0.0f, 180.0f), 0.0f, glm::vec2(1.0f, 1.0f));
	UpperCollider->AddComponent<visions2D::BoxCollider>("obstacle", glm::vec2(-320.0f, -5.0f), glm::vec2(320.0f, 5.0f));

	LowerCollider = gameWorld->AddEntity("lower collider");
	LowerCollider->AddComponent<visions2D::TransformComponent>(glm::vec2(0.0f, -180.0f), 0.0f, glm::vec2(1.0f, 1.0f));
	LowerCollider->AddComponent<visions2D::BoxCollider>("obstacle", glm::vec2(-320.0f, -5.0f), glm::vec2(320.0f, 5.0f));

	inputSystem->Initialize();
	gameWorld->BeginPlay();
}

void Start() {
	inputSystem = new visions2D::InputSystem();
	gameWorld = new visions2D::GameWorld();
	collisionWorld = new visions2D::CollisionWorld();
	if (collisionWorld->Initialize()) {
		LOG_INFO("app: collision world initialized");
	}

	// load assets here...
	LazyTown = new visions2D::Font();
	LazyTown->Load("./assets/ChevyRay - Lazytown.ttf");
	// loading textures
	PlayerTexture = new visions2D::Texture();
	PlayerTexture = LazyTown->RenderToTexture("@", 32);
	ObstacleTexture = LazyTown->RenderToTextureWrapped("xxx\nxxx\nxxx\nxxx\nxxx", 32, 128);
	// PlayerTexture->Load("./src/DefaultAssets/White.png");
	PointTexture = LazyTown->RenderToTexture(std::to_string(CachedPointsValue), 24);

	CreateEntities();
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

	if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_I)) {
		bRenderCollision = !bRenderCollision;
	}

	if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_P)) {
		SetIsPaused(!bIsPaused);
	}

	// Restarting the Game...
	if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_R)) {
		gameWorld->Destroy();
		CachedPointsValue = 0;

		PlayerEntity = nullptr;

		CreateEntities();
		SetIsPaused(false);
	}
	
}

void Update(float DeltaTime) {
	
	if (bIsPaused) return;

	gameWorld->Update(DeltaTime);
	collisionWorld->VerifyAllCollisions();

	// updating cached points
	if (PlayerEntity->GetComponentOfType<PlayerInput>()->GetPoints() != CachedPointsValue) {
		CachedPointsValue = PlayerEntity->GetComponentOfType<PlayerInput>()->GetPoints();

		// This should be "standard" way of updating a text
		PointTexture->Unload();
		delete PointTexture;
		PointTexture = LazyTown->RenderToTexture(std::to_string(CachedPointsValue), 24);
	}

	// checking for game over
	bool IsPlayerAlive = PlayerEntity->GetComponentOfType<PlayerInput>()->IsAlive();
	if (!IsPlayerAlive && !bIsPaused) {
		SetIsPaused(true);

		PointTexture->Unload();
		delete PointTexture;
		PointTexture = LazyTown->RenderToTexture("Game Over!", 24);
	}
}

// TODO: RendererReference here because the reference is needed to push the draw information.
// I want to keep this here because I can substitute it for the batch.
// So the engine gives the application a drawing batch
void Render(visions2D::Renderer* RendererReference) {
	gameWorld->Render(RendererReference);

	if (bRenderCollision) {
		collisionWorld->Render(RendererReference);
	}

	// Rendering Points here
	{
		visions2D::RenderData rd;
		rd.Texture = PointTexture;
		rd.TextureScale = glm::vec2(PointTexture->GetWidth(), PointTexture->GetHeight());
		rd.TexCoords = nullptr;
		rd.WorldRotation = 0.0f;
		rd.WorldPosition = glm::vec2(0.0f, 150.0f);
		// turns out fonts are upside down... wtf...
		rd.WorldScale = glm::vec2(1.0f, -1.0f);
		RendererReference->SpriteRenderData.push_back(rd);
	}
}

void OnImGui() {
	/*
	ImGui::Begin("Debug");
	int Points = PlayerEntity->GetComponentOfType<PlayerInput>()->GetPoints();
	ImGui::InputInt("Points", &Points);
	ImGui::End();
	*/

	/*
	ImGui::Begin("Debug");
	ImGui::InputFloat("Obstacle X", &ObstacleXPosition, 1.0f);
	ImGui::End();
	*/
}

void Shutdown() {
	gameWorld->Destroy();
	delete gameWorld;

	inputSystem->Shutdown();
	delete inputSystem;

	collisionWorld->Shutdown();
	delete collisionWorld;
}

int main(void) {

	visions2D::AppConfig conf;
	conf.WindowName = "Flappy ASCII";
	conf.Width = 640;
	conf.Height = 360;

	conf.Startup = Start;
	conf.PreProcessInput = PreInput;
	conf.ProcessInput = Input;
	conf.ImGuiRender = OnImGui;
	conf.Update = Update;
	conf.Render = Render;

	// maybe I should have a static Application::Run(AppConfig) ?!
	visions2D::Application* app = new visions2D::Application(conf);
	app->Run();
	delete app;

	return 0;
}