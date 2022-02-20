#pragma once
#include "StateMachine.h"
#include "../../Common/Vector3.h"
namespace NCL {
	namespace CSC8503 {
		class GameObject;
		class SpringStateMachine : public StateMachine
		{
			private:
			GameObject* m_Object;
			NCL::Maths::Vector3 m_InitialPos;
			NCL::Maths::Vector3 m_Force;
			NCL::Maths::Vector3 m_Position;
		public:
			SpringStateMachine(GameObject *obj, NCL::Maths::Vector3 force, NCL::Maths::Vector3 direction);
			~SpringStateMachine();
			virtual void Update(float dt);
		};
	}
}