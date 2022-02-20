#pragma once
#include "StateMachine.h"
namespace NCL {
	namespace CSC8503 {
		class GameObject;
		class OscillateStateMachine : public StateMachine
		{
		private:
			GameObject* m_Object;
			float xangle;
			float zangle;
		public:
			OscillateStateMachine(GameObject* obj);
			~OscillateStateMachine();
			virtual void Update(float dt);
		};
	}
}