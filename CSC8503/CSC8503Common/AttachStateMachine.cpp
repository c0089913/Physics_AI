#include "AttachStateMachine.h"
#include "State.h"
#include "GameObject.h"
#include "StateTransition.h"
#include "../../Common/Window.h"
#include "OBBVolume.h"
#include "SphereVolume.h"
#include "CollisionDetection.h"


NCL::CSC8503::AttachStateMachine::AttachStateMachine(GameObject* obj, GameObject *test)
{
	m_Object = obj;
	m_AttachedObject = test;
	State* A = new State([this](float dt)->void
		{
			m_Object->GetRenderObject()->SetColour(Vector4(1,0,0,1));
			return;
		}, "InitState"
	);
	m_Attached = false;
	
	Vector3 position = m_Object->GetTransform().GetPosition();
	position.y = m_Object->GetTransform().GetPosition().y - ((OBBVolume*)m_Object->GetBoundingVolume())->GetHalfDimensions().y;
	position.y /= 2;

	m_ZoneVolume = new NCL::AABBVolume(((OBBVolume*)m_Object->GetBoundingVolume())->GetHalfDimensions());
	m_ZoneVolume->GetHalfDimensions().y = position.y;

	State* B = new State([this](float dt)->void
		{
			if (!m_Attached)
			{
				UpdateZoneTransform();
				NCL::CollisionDetection::CollisionInfo info;
				if (CollisionDetection::AABBSphereIntersection(*m_ZoneVolume, m_ZoneTransform,
					*(SphereVolume*)m_AttachedObject->GetBoundingVolume(), m_AttachedObject->GetTransform(), info))
				{
					m_Attached = true;
				}
			}
			else
			{
				Vector3 position = m_Object->GetTransform().GetPosition();
				position.y -= (((OBBVolume*)m_Object->GetBoundingVolume())->GetHalfDimensions().y +
					((SphereVolume*)m_AttachedObject->GetBoundingVolume())->GetRadius());
				m_AttachedObject->GetTransform().SetPosition(position);
			}
		}, "AttachState"
	);
	StateTransition* stateAB = new StateTransition(A, B, [this](void)->bool
		{
			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::H))
			{				
				m_Object->GetRenderObject()->SetColour(Vector4(0,0,1,1));
				return true;
			}
			return false;
		}
	);
	StateTransition* stateBA = new StateTransition(B, A, [this](void)->bool
		{
			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::H))
			{
				m_Attached = false;
				m_Object->GetRenderObject()->SetColour(Vector4(1,0,0,1));
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

NCL::CSC8503::AttachStateMachine::~AttachStateMachine()
{
	delete m_ZoneVolume;
}

void NCL::CSC8503::AttachStateMachine::Update(float dt)
{
	StateMachine::Update(dt);
}

void NCL::CSC8503::AttachStateMachine::UpdateZoneTransform()
{
	Vector3 position = m_Object->GetTransform().GetPosition();
	position.y -= ((OBBVolume*)m_Object->GetBoundingVolume())->GetHalfDimensions().y;
	position.y /= 2;
	m_ZoneTransform.SetPosition(position);
}
