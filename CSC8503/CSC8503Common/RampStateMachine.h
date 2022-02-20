#pragma once
#include "StateMachine.h"
#include "../../Common/Quaternion.h"
#include "../../Common/Matrix4.h"
namespace NCL {
	namespace CSC8503 {
		class GameObject;
		class RampStateMachine :
			public StateMachine
		{
		private:
			GameObject* m_Object;
			NCL::Maths::Quaternion m_InitialOrientation;
			NCL::Maths::Matrix4 m_Rotation;
		public:
			RampStateMachine(GameObject* obj, NCL::Maths::Matrix4 rotation);
			~RampStateMachine();
			virtual void Update(float dt);
		};
	}
}

