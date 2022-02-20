#include "SpringStateMachine.h"
#include "State.h"
#include "GameObject.h"
#include "StateTransition.h"
#include "../../Common/Window.h"
using namespace NCL;
using namespace CSC8503;

NCL::CSC8503::SpringStateMachine::SpringStateMachine(GameObject *object, Vector3 force, Vector3 position)
{
	m_Object = object;
	m_Force = force;
	m_Position = position;
	 State* A = new State([](float dt)->void
		{
			return;
		}, "RestState"
	);

	 State* B = new State([this](float dt)->void
		{
			if (m_Object->GetPhysicsObject()->GetLinearVelocity().LengthSquared() <= 0.01f)
			{
				m_Object->GetPhysicsObject()->AddForceAtPosition(m_Force, m_Position, true);
			}
		}, "YeetState"
	);
	 StateTransition* stateAB = new StateTransition(A, B, [this](void)->bool
		{
			 if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Y))
			{				
				this->m_InitialPos = m_Object->GetTransform().GetPosition();
				return true;
			}
			return false;
		}
	);
	 StateTransition* stateBA = new StateTransition(B, A, [this](void)->bool
		{
			if ((m_Object->GetTransform().GetPosition()- this->m_InitialPos).Length() > 10.0f)
			{
				m_Object->GetPhysicsObject()->SetLinearVelocity(Vector3(0,0,0));
				m_Object->GetTransform().SetPosition(m_InitialPos);				
				return true;
			}
			return false;
		}
	);
	AddState(A);
	AddState(B);
	AddTransition(stateAB);
	AddTransition(stateBA);
}

NCL::CSC8503::SpringStateMachine::~SpringStateMachine()
{
}

void NCL::CSC8503::SpringStateMachine::Update(float dt)
{
	StateMachine::Update(dt);
}
