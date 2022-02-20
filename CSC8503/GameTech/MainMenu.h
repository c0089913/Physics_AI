#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "PushDownState.h"

namespace NCL {
	namespace CSC8503 {
		class StateGameObject;
		class MainMenu : public PushdownState {
		public:
			enum Choice
			{
				Choice_None,
				Choice_ScrewBall,
				Choice_EvilMaze
			};
			MainMenu();
			~MainMenu();
			PushdownResult OnUpdate(float dt,
				PushdownState** newState);
			void OnAwake();
			void OnSleep() {}	
			GameObject* AddTriggerCubeToWorld(const Vector3& position, Vector3 dimensions, std::function<void(GameObject *)> triggerfunc, float inverseMass = 10.0f);
			void Update(float dt);
			void PrintText();
			void SelectObjects();
		protected:
			void Init();
			GameTechRenderer* renderer;
			GameWorld* world;
			OGLMesh* cubeMesh = nullptr;
			OGLTexture* basicTex = nullptr;
			OGLShader* basicShader = nullptr;
			GameObject* selectionObject;
			Choice choice;
		};
	}
}

