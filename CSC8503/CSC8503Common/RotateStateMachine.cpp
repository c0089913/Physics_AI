#include "RotateStateMachine.h"
#include "State.h"
#include "GameObject.h"
#include "../../Common/Window.h"
#include "OBBVolume.h"

NCL::CSC8503::RotateStateMachine::RotateStateMachine(GameObject* obj, GameObject* obj2, NCL::Maths::Vector3 position)
{
	m_Object = obj;
	m_ObjectToMove = obj2;
	m_Position = position;
	angle = 0.0f;
	State* A = new State([this](float dt)->void
		{
			if (Window::GetKeyboard()->KeyDown(KeyboardKeys::J))
			{
				angle -= 40.0f * dt;
				//angle = angle < -180.0f ? 180.0f : (angle > 180.0f ? -180.0f: angle);
				Rotate(-40.0f * dt, m_Position);

			}
			else if (Window::GetKeyboard()->KeyDown(KeyboardKeys::L))
			{
				angle += 40.0f * dt;
				//angle = angle < -180.0f ? 180.0f : (angle > 180.0f ? -180.0f : angle);
				Rotate(40.0f * dt, m_Position);
			}
			Vector3 pos = m_Object->GetTransform().GetPosition() + 
				Matrix3(m_Object->GetTransform().GetOrientation()) * ((NCL::OBBVolume*)m_Object->GetBoundingVolume())->GetHalfDimensions();

			m_ObjectToMove->GetTransform().SetPosition(pos);
			m_ObjectToMove->GetTransform().SetOrientation(m_Object->GetTransform().GetOrientation());

		}, "RotateState"
	);	
	AddState(A);
}

NCL::CSC8503::RotateStateMachine::~RotateStateMachine()
{
}

void NCL::CSC8503::RotateStateMachine::Update(float dt)
{
	StateMachine::Update(dt);
}

void NCL::CSC8503::RotateStateMachine::Rotate(float angle, NCL::Maths::Vector3 pos)
{
	Vector3 position = m_Object->GetTransform().GetPosition();
	m_Object->GetTransform().SetPosition(position - pos);

	Quaternion currentorientation = m_Object->GetTransform().GetOrientation();
	Matrix4 rotation = Matrix4::Rotation(angle, Vector3(0, 1, 0)) * Matrix4(currentorientation);
	m_Object->GetTransform().SetOrientation(Quaternion(rotation));

	position = m_Object->GetTransform().GetPosition();
	m_Object->GetTransform().SetPosition(position + pos);
}
