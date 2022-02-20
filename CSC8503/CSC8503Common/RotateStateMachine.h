#pragma once
#include "StateMachine.h"
#include "../../Common/Vector3.h"
#include "../../Common/Matrix4.h"
namespace NCL {
	namespace CSC8503 {
		class GameObject;
		class RotateStateMachine :
			public StateMachine
		{
		private:
			GameObject* m_Object;
			GameObject* m_ObjectToMove;
			NCL::Maths::Vector3 m_Position;
			float angle;
		public:
			RotateStateMachine(GameObject* obj, GameObject* obj2, NCL::Maths::Vector3 position);
			~RotateStateMachine();
			virtual void Update(float dt);
			void Rotate(float angle, NCL::Maths::Vector3 pos);
		};
	}
}