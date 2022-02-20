#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "PushDownState.h"
#include "EnemyBehaviour.h"

#include <functional>
namespace NCL {
	namespace CSC8503 {
		class StateGameObject;
		class EvilMaze : public PushdownState {
		public:
			enum Result
			{
				IN_PROGRESS,
				WON,
				LOST
			};
			EvilMaze();
			~EvilMaze();

			virtual void UpdateGame(float dt);
			PushdownResult OnUpdate(float dt,
				PushdownState** newState);

			void OnAwake();
			void OnSleep();
		protected:
			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			void InitWorld();
			void InitLevel();
			void InitDefaultFloor();
			void DisplayPathfinding();
			bool SelectObject();
			void MoveBall(float dt);

			GameObject* AddFloorToWorld(const Vector3& position);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddTriggerSphereToWorld(const Vector3& position, float radius, std::function<void(GameObject *)> triggerfunc, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);
			GameObject* AddTriggerCubeToWorld(const Vector3& position, Vector3 dimensions, std::function<void(GameObject *)> triggerfunc, float inverseMass = 10.0f);
			GameObject* AddOBBCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f, Quaternion orientation = Quaternion(Vector3(0, 0, 0), 0));
			
			void ShowDebugInformation(GameObject* object);

			GameTechRenderer* renderer;
			PhysicsSystem* physics;
			GameWorld* world;

			bool useGravity;
			bool inSelectionMode;
			bool gameOver;
			bool* m_Obstacles;
			float		forceMagnitude;

			GameObject* selectionObject = nullptr;

			OGLMesh* capsuleMesh = nullptr;
			OGLMesh* cubeMesh = nullptr;
			OGLMesh* sphereMesh = nullptr;
			OGLTexture* basicTex = nullptr;
			OGLShader* basicShader = nullptr;

			OGLMesh* charMeshA = nullptr;
			OGLMesh* charMeshB = nullptr;
			OGLMesh* enemyMesh = nullptr;
			OGLMesh* bonusMesh = nullptr;

			GameObject* ball;	
			GameObject* evilBall;
			EnemyBehaviour* behaviour;
			Result result;
			int lives;
		};
	}
}

