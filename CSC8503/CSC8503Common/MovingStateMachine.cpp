#include "MovingStateMachine.h"
#include "State.h"
#include "GameObject.h"
#include "StateTransition.h"

NCL::CSC8503::MovingStateMachine::MovingStateMachine(GameObject* obj, NCL::Maths::Vector3 final, bool shouldRotate)
{
	m_Object = obj;
	m_Rotate = shouldRotate;
	m_Initial = m_Object->GetTransform().GetPosition();
	m_Final = final;

	State* A = new State([this](float dt)->void
		{
			Vector3 move = (m_Final - m_Object->GetTransform().GetPosition());
			move.Normalise();
			m_Object->GetTransform().SetPosition(move * 10.0f * dt  + m_Object->GetTransform().GetPosition());
			if (m_Rotate)
			{
				Quaternion currentorientation = m_Object->GetTransform().GetOrientation();
				Matrix4 rotation = Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0)) * Matrix4(currentorientation);
				m_Object->GetTransform().SetOrientation(Quaternion(rotation));
			}
		}, "MovingToFinal"
	);

	State* B = new State([this](float dt)->void
		{
			Vector3 move = (m_Initial - m_Object->GetTransform().GetPosition());
			move.Normalise();
			m_Object->GetTransform().SetPosition(move * 10.0f * dt + m_Object->GetTransform().GetPosition());
			if (m_Rotate)
			{
				Quaternion currentorientation = m_Object->GetTransform().GetOrientation();
				Matrix4 rotation = Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0)) * Matrix4(currentorientation);
				m_Object->GetTransform().SetOrientation(Quaternion(rotation));
			}
		}, "ComingBack"
	);
	StateTransition* stateAB = new StateTransition(A, B, [this](void)->bool
		{
			if ((m_Object->GetTransform().GetPosition() - m_Final).Length() <= 0.1)
			{				
				return true;
			}
			return false;
		}
	);
	StateTransition* stateBA = new StateTransition(B, A, [this](void)->bool
		{
			if ((m_Object->GetTransform().GetPosition() - m_Initial).Length() <= 0.1)
			{
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

NCL::CSC8503::MovingStateMachine::~MovingStateMachine()
{
}

void NCL::CSC8503::MovingStateMachine::Update(float dt)
{
	StateMachine::Update(dt);
}
