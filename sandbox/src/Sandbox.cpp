// *******************************************************
// *******************************************************
// 
// 
// 
// *******************************************************
// *******************************************************

/*
- refactoring
1. make a asset store or resource manager that stores textures, fonts, etc...
2. make a game application that will have input system, game world, collision world, etc...
3. load entities and components from .json files
*/

#include <visions2D.h>

// TODO: This should move to a GameApplication class
visions2D::InputSystem* inputSystem = nullptr;
visions2D::GameWorld* gameWorld = nullptr;
visions2D::CollisionWorld* collisionWorld = nullptr;
bool bRenderCollision = false;
bool bIsPaused = false;
visions2D::ResourceManager* resourceManager = nullptr;

void Start() {
	inputSystem = new visions2D::InputSystem();
	gameWorld = new visions2D::GameWorld();
	collisionWorld = new visions2D::CollisionWorld();
	if (collisionWorld->Initialize()) {
		LOG_INFO("app: collision world initialized");
	}
	resourceManager = new visions2D::ResourceManager();
	visions2D::Random::Initialize();

	inputSystem->Initialize();
	gameWorld->BeginPlay();
}

void PreInput() {
	inputSystem->PrepareForUpdate();
}

void Input() {
	inputSystem->Update();
	gameWorld->ProcessInput(inputSystem->GetState());

	if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_I)) {
		bRenderCollision = !bRenderCollision;
	}
	
}

void Update(float DeltaTime) {
	
	if (bIsPaused) return;

	gameWorld->Update(DeltaTime);
	collisionWorld->VerifyAllCollisions();
}

void Render(visions2D::Renderer* RendererReference) {
	gameWorld->Render(RendererReference);

	if (bRenderCollision) {
		collisionWorld->Render(RendererReference);
	}
}

void Shutdown() {
	gameWorld->Destroy();
	delete gameWorld;

	collisionWorld->Shutdown();
	delete collisionWorld;

	inputSystem->Shutdown();
	delete inputSystem;
}

int main(void) {

	visions2D::AppConfig conf;
	conf.WindowName = "sandbox";
	conf.Width = 640;
	conf.Height = 360;

	conf.Startup = Start;
	conf.PreProcessInput = PreInput;
	conf.ProcessInput = Input;
	conf.Update = Update;
	conf.Render = Render;

	visions2D::Application* app = new visions2D::Application(conf);
	app->Run();
	delete app;

	return 0;
}