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

// **********************************************************************************
class PlayerInput : public visions2D::Component {
public:
	void BeginPlay() override {
		m_TransformReference = Owner->GetComponentOfType<visions2D::TransformComponent>();
		visions2D::BoxCollider* b = Owner->GetComponentOfType<visions2D::BoxCollider>();

		// TODO: Have an easier way to bind the callback!
		if (b != nullptr) {
			b->OnCollisionEnter = COLLISION_CALLBACK(&PlayerInput::PlayerOnCollisionEnter);
			b->OnCollisionExit = COLLISION_CALLBACK(&PlayerInput::PlayerOnCollisionExit);
		}

		m_bIsAlive = true;
	}

	void PlayerOnCollisionEnter(visions2D::BoxCollider* Other) {
		if (Other->GetTag() == "obstacle") {
			m_bIsAlive = false;
		}
		else if (Other->GetTag() == "point-collider") {
			m_Points++;
		}
	}

	void PlayerOnCollisionExit(visions2D::BoxCollider* Other) {
		// LOG_INFO("Player OnCollisionExit with {0}", Other->Owner->Name);
	}

	bool ProcessInput(const visions2D::InputState& CurrentInputState) override {

		if (CurrentInputState.Mouse.WasMouseKeyPressedThisFrame(visions2D::v2D_Mousecode::MOUSECODE_LEFT)) {
			m_VerticalVelocity = m_UpForce;
			return true;
		}

		return false;
	}

	void Update(float DeltaTime) override {
		m_VerticalVelocity -= DeltaTime * m_Gravity;
		m_TransformReference->Translate(glm::vec2(0.0f, m_VerticalVelocity * DeltaTime));

		if (m_VerticalVelocity <= 0.0f) {
			float CurrentRotation = m_TransformReference->Rotation;
			CurrentRotation = glm::min(CurrentRotation + (1.0f * DeltaTime * m_RotationSpeed), 45.0f);
			m_TransformReference->Rotation = CurrentRotation;
		}
		else {
			m_TransformReference->Rotation = -30.0f;
		}
	}

	inline int GetPoints() const { return m_Points; }
	inline bool IsAlive() const { return m_bIsAlive; }
private:
	visions2D::TransformComponent* m_TransformReference = nullptr;
	float m_UpForce = 225.0f;
	float m_RotationSpeed = 100.0f;
	float m_VerticalVelocity = 0.0f;
	float m_Gravity = 500.0f;

	bool m_bIsAlive;
	int m_Points = 0;
};


// TODO: This should move to a GameApplication class
visions2D::InputSystem* inputSystem = nullptr;
visions2D::GameWorld* gameWorld = nullptr;
visions2D::CollisionWorld* collisionWorld = nullptr;
bool bRenderCollision = false;
bool bIsPaused = false;
visions2D::ResourceManager* resourceManager = nullptr;
visions2D::Entity* PlayerEntity = nullptr;

void Start() {
	inputSystem = new visions2D::InputSystem();
	gameWorld = new visions2D::GameWorld();
	collisionWorld = new visions2D::CollisionWorld();
	if (collisionWorld->Initialize()) {
		LOG_INFO("app: collision world initialized");
	}
	resourceManager = new visions2D::ResourceManager();
	visions2D::Random::Initialize();

	resourceManager->AddTexture("player", "./src/DefaultAssets/plane.png");


	PlayerEntity = gameWorld->AddEntity("player-entity");
	PlayerEntity->AddComponent<visions2D::TransformComponent>(glm::vec2(-200.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f));
	visions2D::SpriteComponent& spriteComponent = PlayerEntity->AddComponent<visions2D::SpriteComponent>(resourceManager->GetTexture("player"), 0);
	spriteComponent.SpriteColor.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	// PlayerEntity->AddComponent<PlayerInput>();
	PlayerEntity->AddComponent<visions2D::LuaComponent>("test.lua");

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

	// **********************************************************************************
	// Testing some lua...
	// lua_State* L = luaL_newstate();
	/*
	luaL_openlibs(L);
	if (CheckLua(L, luaL_dofile(L, "test.lua"))) {
		LOG_INFO("Succesfully read lua file!");
		lua_getglobal(L, "a");
		if (lua_isnumber(L, -1)) {
			LOG_INFO("a = {0}", (int)lua_tointeger(L, -1));
		}
		lua_getglobal(L, "b");
		if (lua_isnumber(L, -1)) {
			LOG_INFO("b = {0}", (int)lua_tointeger(L, -1));
		}
		lua_getglobal(L, "c");
		if (lua_isnumber(L, -1)) {
			LOG_INFO("c = {0}", (int)lua_tointeger(L, -1));
		}
	}
	*/

	// **********************************************************************************

	visions2D::Application* app = new visions2D::Application(conf);
	app->Run();
	delete app;

	return 0;
}