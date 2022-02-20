#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "PushDownState.h"
#include <functional>
namespace NCL {
	namespace CSC8503 {
		class StateGameObject;
		class TriggerGameObject;
		class ScrewBall	: public PushdownState	{
		public:
			ScrewBall();
			~ScrewBall();

			virtual void UpdateGame(float dt);
			PushdownResult OnUpdate(float dt,
				PushdownState** newState);

			void OnAwake();
			void OnSleep();
			enum Result
			{
				ONGOING,
				WON,
				LOST
			};
		protected:
			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			void InitWorld();

			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void InitDefaultFloor();
			void BridgeConstraintTest();
	
			bool SelectObject();

			GameObject* AddFloorToWorld(const Vector3& position);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions,float inverseMass = 10.0f);
			TriggerGameObject* AddTriggerCubeToWorld(const Vector3& position, Vector3 dimensions, std::function<void(GameObject *)> triggerfunc, float inverseMass = 10.0f);
			GameObject* AddOBBCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, Quaternion orientation = Quaternion(Vector3(0, 0, 0), 0));

			GameObject* AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f);

			void AddSprings();
			void AddBlocks();
			void AddRamps();
			void AddFancyBar();
			void AddRewardBlocks();
			void ShowDebugInformation(GameObject* object);
			void ShowTimer();

			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;

			OGLMesh*	capsuleMesh = nullptr;
			OGLMesh*	cubeMesh	= nullptr;
			OGLMesh*	sphereMesh	= nullptr;
			OGLTexture* basicTex	= nullptr;
			OGLShader*	basicShader = nullptr;

			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			GameObject* ball;
			Vector3 lockedOffset		= Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

			LARGE_INTEGER timer, start;
			LARGE_INTEGER frequency;
			int score;
			Result result;

			Vector4 colour;
		};
	}
}

