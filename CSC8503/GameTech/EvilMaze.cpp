#include "EvilMaze.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"
#include "../CSC8503Common/PositionConstraint.h"
#include "StateGameObject.h"
#include "../CSC8503Common/SpringStateMachine.h"
#include "../CSC8503Common/RampStateMachine.h"
#include "../CSC8503Common/RotateStateMachine.h"
#include "../CSC8503Common/OscillateStateMachine.h"
#include "../CSC8503Common/AttachStateMachine.h"
#include "../CSC8503Common/MovingStateMachine.h"
#include "../CSC8503Common/TriggerGameObject.h"
#include "../../Common/Assets.h"

#include <sstream>
#include <fstream>
using namespace NCL;
using namespace CSC8503;


vector <Vector3 > testNodes;
void TestPathfinding() {
	NavigationGrid grid("TestGrid1.txt");

	NavigationPath outPath;

	Vector3 startPos(80, 2, 20);
	Vector3 endPos(80, 2, 80);

	bool found = grid.FindPath(startPos, endPos, outPath);

	Vector3 pos;
	testNodes.clear();
	while (outPath.PopWaypoint(pos)) {
		testNodes.push_back(pos);
	}
}
void EvilMaze::DisplayPathfinding() {
	/*for (int i = 1; i < path.size(); ++i) {
		Vector3 a = path[i - 1];
		Vector3 b = path[i];

		Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
	}*/
}

EvilMaze::EvilMaze()  {
	world = new GameWorld();
	renderer = &(GameTechRenderer::GetInstance());
	renderer->SetGameWorld(world);
	physics = new PhysicsSystem(*world);
	gameOver = false;
	forceMagnitude = 10.0f;
	useGravity = true;
	inSelectionMode = false;
	lives = 3;
	//TestPathfinding();
	Debug::SetRenderer(renderer);
	InitialiseAssets();
}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes,
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/
void EvilMaze::InitialiseAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh", &cubeMesh);
	loadFunc("sphere.msh", &sphereMesh);
	loadFunc("Male1.msh", &charMeshA);
	loadFunc("courier.msh", &charMeshB);
	loadFunc("security.msh", &enemyMesh);
	loadFunc("coin.msh", &bonusMesh);
	loadFunc("capsule.msh", &capsuleMesh);

	basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	InitCamera();
	InitWorld();
}

EvilMaze::~EvilMaze() {
	delete cubeMesh;
	delete sphereMesh;
	delete charMeshA;
	delete charMeshB;
	delete enemyMesh;
	delete bonusMesh;

	delete basicTex;
	delete basicShader;

	delete physics;

	delete world;
	delete[]m_Obstacles;
}

void EvilMaze::UpdateGame(float dt) {

	if (lives <= 0)
		result = LOST;
	switch (result)
	{
	case LOST:
	{
		renderer->DrawString("Lost :(", Vector2(0, 5));
		
	}break;
	case WON:
	{
		renderer->DrawString("Won. Congratulations!", Vector2(0, 5));
	}break;
	case IN_PROGRESS:
	{
		renderer->DrawString("Lives: "+ std::to_string(lives), Vector2(0, 5));
	}break;
	}
	if (!inSelectionMode) {
		world->GetMainCamera()->UpdateCamera(dt);
	}


	UpdateKeys();	
	SelectObject();
	//	MoveSelectedObject();
	std::vector <GameObject*>::const_iterator first;
	std::vector <GameObject*>::const_iterator last;
	world->GetObjectIterators(first, last);
	for (auto i = first; i != last; ++i) {
		if ((*i)->m_Machine == nullptr) {
			continue;
		}
		(*i)->m_Machine->Update(dt);
	}

	MoveBall(dt);
	
	if (result == IN_PROGRESS)
	{
		behaviour->Update(dt);
		physics->Update(dt);
	}
	world->UpdateWorld(dt);
	renderer->Update(dt);

	Debug::FlushRenderables(dt);
	renderer->Render();


	
	DisplayPathfinding();
}

PushdownState::PushdownResult EvilMaze::OnUpdate(float dt,
	PushdownState** newState)
{
	UpdateGame(dt);
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::R)) {
		return PushdownResult::Pop;
	}

	return PushdownResult::NoChange;
}

void NCL::CSC8503::EvilMaze::OnAwake()
{
}

void NCL::CSC8503::EvilMaze::OnSleep()
{
}

void EvilMaze::UpdateKeys() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1)) {
		InitWorld(); //We can reset the simulation at any time with F1
		selectionObject = nullptr;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8)) {
		world->ShuffleObjects(false);
	}
}

void EvilMaze::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(500.0f);
	world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(315.0f);
	world->GetMainCamera()->SetPosition(Vector3(-60, 40, 60));
	//world->GetMainCamera()->SetPosition(Vector3(500, 500, 500));
}

void EvilMaze::InitWorld() {
	world->ClearAndErase();
	physics->Clear();
	//BridgeConstraintTest();
//	InitMixedGridWorld(5, 5, 3.5f, 3.5f);
//	InitGameExamples();
	InitDefaultFloor();
	InitLevel();
}

void NCL::CSC8503::EvilMaze::InitLevel()
{
	std::ifstream infile(Assets::DATADIR + "TestGrid1.txt");

	int nodeSize = 0, gridWidth = 0, gridHeight = 0;
	infile >> nodeSize;
	infile >> gridWidth;
	infile >> gridHeight;

	m_Obstacles = new bool[gridWidth * gridHeight];

	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			bool& n = m_Obstacles[(gridWidth * y) + x];
			char type = 0;
			infile >> type;
			std::cout << type;
			n = type == 'x';
			if (n)
			{
				AddCubeToWorld(Vector3(x *nodeSize , 2, y * nodeSize ), Vector3(nodeSize/2, 2, nodeSize/2), 0.0f);
			}
		}
		std::cout << "\n";
	}
	infile.close();
	ball = AddSphereToWorld(Vector3(80,2,80), 2, 2.0f);
	ball->GetRenderObject()->SetColour(Vector4(0, 0, 1, 1));

	evilBall = AddTriggerSphereToWorld(Vector3(80, 2, 20), 2, [this](GameObject* obj) {}, 2.0f);
	evilBall->GetRenderObject()->SetColour(Vector4(1,0,0,1));
	behaviour = new EnemyBehaviour(evilBall, ball, m_Obstacles);
	TriggerGameObject* eb = dynamic_cast<TriggerGameObject*>(evilBall);
	eb->SetFunction(
		[this](GameObject* obj) 
		{if (obj == ball)
		{
			lives--;
			ball->GetTransform().SetPosition(Vector3(80, 2, 80));
			evilBall->GetTransform().SetPosition(Vector3(80, 2, 20));
		}
		if (obj->GetTransform().GetPosition().y == -2) //ground
			return;
		behaviour->ResetPath(); 
		});

	GameObject * winBlock = AddTriggerCubeToWorld(Vector3(180, 2, 50), Vector3(2, 2, 2), [this](GameObject* obj) {if (obj == ball) result = WON; }, 0.0f);
	winBlock->GetRenderObject()->SetColour(Vector4(1,1,0,1));
}

GameObject* EvilMaze::AddFloorToWorld(const Vector3& position) {
	GameObject* floor = new GameObject();

	Vector3 floorSize = Vector3(100, 2, 50);
	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(floor);

	return floor;
}

/*

Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple'
physics worlds. You'll probably need another function for the creation of OBB cubes too.

*/
GameObject* EvilMaze::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject();

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}

GameObject* NCL::CSC8503::EvilMaze::AddTriggerSphereToWorld(const Vector3& position, float radius, std::function<void(GameObject *)> triggerfunc, float inverseMass)
{
	GameObject* sphere = new TriggerGameObject(triggerfunc);

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}

GameObject* EvilMaze::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass) {
	GameObject* cube = new GameObject();

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

GameObject* NCL::CSC8503::EvilMaze::AddTriggerCubeToWorld(const Vector3& position, Vector3 dimensions, std::function<void(GameObject *)> func, float inverseMass)
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

GameObject* NCL::CSC8503::EvilMaze::AddOBBCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, Quaternion orientation)
{
	GameObject* cube = new GameObject();

	OBBVolume* volume = new OBBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);
	cube->GetTransform()
		.SetPosition(position)
		.SetOrientation(orientation)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

void EvilMaze::InitDefaultFloor() {
	AddFloorToWorld(Vector3(95, -2, 45));
}

void NCL::CSC8503::EvilMaze::ShowDebugInformation(GameObject* object)
{
	renderer->DrawString("Name:" + object->GetName(), Vector2(0, 5));
	std::ostringstream oss;
	oss << object->GetTransform().GetPosition();
	renderer->DrawString("Position:" + oss.str(), Vector2(0, 10));
	oss.str(std::string());
	oss << object->GetTransform().GetOrientation();
	renderer->DrawString("Orientation:" + oss.str(), Vector2(0, 15));

	if (object->m_Machine)
	{
		oss.str(std::string());
		oss << object->m_Machine->GetActiveStateName();
		renderer->DrawString("Active State:" + oss.str(), Vector2(0, 20));
	}
}

/*

Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around.

*/
bool EvilMaze::SelectObject() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode) {
		renderer->DrawString("Press Q to change to camera mode!", Vector2(5, 85));

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selectionObject = nullptr;				
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				selectionObject = (GameObject*)closestCollision.node;
				selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		renderer->DrawString("Press Q to change to select mode!", Vector2(5, 85));
	}

	if (selectionObject) {
		//renderer->DrawString("Press L to lock selected object object!", Vector2(5, 80));
		ShowDebugInformation(selectionObject);
	}
	return false;
}

void NCL::CSC8503::EvilMaze::MoveBall(float dt)
{
	dt = 0.3f;
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::LEFT))
	{
		ball->GetPhysicsObject()->SetLinearVelocity(Vector3(0,0,-100 *dt));
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::RIGHT))
	{
		ball->GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, 100 * dt));
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::UP))
	{
		ball->GetPhysicsObject()->SetLinearVelocity(Vector3(100 * dt, 0 ,0));
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::DOWN))
	{
		ball->GetPhysicsObject()->SetLinearVelocity(Vector3(-100 * dt, 0, 0));
	}
}

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/
/*void EvilMaze::MoveSelectedObject() {
	//renderer->DrawString("Click Force:" + std::to_string(forceMagnitude),
	//	Vector2(10, 20)); //Draw debug text at 10,20
	//forceMagnitude += Window::GetMouse()->GetWheelMovement() * 100.0f;

	if (!selectionObject) {
		return;//we haven’t selected anything!
	}
	//Push the selected object!
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::RIGHT)) {
		Ray ray = CollisionDetection::BuildRayFromMouse(
			*world->GetMainCamera());
		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			if (closestCollision.node == selectionObject) {
				selectionObject->GetPhysicsObject()->
					AddForceAtPosition( ray.GetDirection() * forceMagnitude,
						 closestCollision.collidedAt);
			}
		}
	}
}*/