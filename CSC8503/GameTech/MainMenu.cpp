#include "MainMenu.h"
#include "ScrewBall.h"
#include "EvilMaze.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"
#include "../CSC8503Common/TriggerGameObject.h"

NCL::CSC8503::MainMenu::MainMenu()
{
	world = new GameWorld();
	renderer = &GameTechRenderer::GetInstance();
	renderer->SetGameWorld(world);
	Debug::SetRenderer(renderer);
	Init();
	selectionObject = nullptr;
	choice = Choice_None;
}

NCL::CSC8503::MainMenu::~MainMenu()
{
	delete cubeMesh;
	delete world;
}

PushdownState::PushdownResult NCL::CSC8503::MainMenu::OnUpdate(float dt,
	PushdownState** newState)
{
	Update(dt);
	switch (choice)
	{
	case Choice_ScrewBall:
		*newState = new ScrewBall();		
		return PushdownResult::Push;
	case Choice_EvilMaze:
		*newState = new EvilMaze();
		return PushdownResult::Push;
	case Choice_None:
		return  PushdownResult::NoChange;
	}
	return PushdownResult::NoChange;
}

void NCL::CSC8503::MainMenu::OnAwake()
{
	selectionObject = nullptr;
	choice = Choice_None;
	renderer->SetGameWorld(world); 
	std::cout << "main menu!\n"; 
	Window::GetWindow()->ShowOSPointer(true);
	Window::GetWindow()->LockMouseToWindow(false);
}

void NCL::CSC8503::MainMenu::Init()
{   
	//load mesh
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};
	loadFunc("cube.msh", &cubeMesh);

	//load shaders and texture
	basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	//setup camera
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(100.0f);
	world->GetMainCamera()->SetPitch(-4.55f);
	world->GetMainCamera()->SetYaw(0.87f);
	world->GetMainCamera()->SetPosition(Vector3(1.8f, 0.0f, 42.7f));

	//setup buttons
	GameObject* obj = AddTriggerCubeToWorld(Vector3(1, 1, 1), Vector3(10, 3, 0.1), [this](GameObject *) {this->choice = Choice_ScrewBall; }, 0.0f);
	obj->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));

	obj = AddTriggerCubeToWorld(Vector3(1, -10, 1), Vector3(10, 3, 0.1), [this](GameObject *) {this->choice = Choice_EvilMaze; }, 0.0f);
	obj->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));

	//enable mouse pointer
	Window::GetWindow()->ShowOSPointer(true);
	Window::GetWindow()->LockMouseToWindow(false);
}

GameObject* NCL::CSC8503::MainMenu::AddTriggerCubeToWorld(const Vector3& position, Vector3 dimensions, std::function<void(GameObject *)> func, float inverseMass)
{
	GameObject* cube = new TriggerGameObject(func);

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);
	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

void NCL::CSC8503::MainMenu::Update(float dt)
{
	world->UpdateWorld(dt);
	SelectObjects();
	//world->GetMainCamera()->UpdateCamera(dt);
	PrintText();
	renderer->Update(dt);
	Debug::FlushRenderables(dt);
	renderer->Render();
}

void NCL::CSC8503::MainMenu::PrintText()
{
	Debug::Print("CW 2021", Vector2(45, 25));
	Debug::Print("Launch Screwball!", Vector2(35, 38));
	Debug::Print("Launch Evil maze", Vector2(35, 70));
}

void NCL::CSC8503::MainMenu::SelectObjects()
{
	if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) 
	{

		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) 
		{
			selectionObject = (GameObject*)closestCollision.node;			
			selectionObject->OnCollisionBegin(nullptr);
		}		
	}
}
