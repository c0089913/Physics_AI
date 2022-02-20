#pragma once
#include "BehaviourSequence.h"
#include "../CSC8503Common/NavigationGrid.h"
#include<vector>
namespace NCL {
	namespace CSC8503 {
		class GameObject;
		class EnemyBehaviour
		{
			GameObject* enemy;
			GameObject* player;
			BehaviourSequence* m_Sequence;
			NavigationGrid m_Grid;
			std::vector<Vector3> m_Path;
			bool* m_Obstacles;
			Vector3 GetPointOnGrid(Vector3 pos);
			float delta;
		public:
			EnemyBehaviour(GameObject *obj, GameObject* p, bool *obstacles);
			~EnemyBehaviour();
			void SetEntity(GameObject* obj);
			void Update(float dt);
			void ResetPath() { delta = 0; }
		};
	}
}
