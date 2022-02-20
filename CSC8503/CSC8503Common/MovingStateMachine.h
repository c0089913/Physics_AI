#pragma once
#include "StateMachine.h"
#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8503 {
		class GameObject;
		class MovingStateMachine :
			public StateMachine
		{
		private:
			GameObject* m_Object;
			NCL::Maths::Vector3 m_Initial;
			NCL::Maths::Vector3 m_Final;
			bool m_Rotate;

		public:
			MovingStateMachine(GameObject* obj, NCL::Maths::Vector3 finalPos, bool shouldRotate);
			~MovingStateMachine();
			virtual void Update(float dt);
		};
	}
}