#include "OscillateStateMachine.h"
#include "State.h"
#include "GameObject.h"
#include "../../Common/Window.h"

float clamp(int x, int min, int max)
{
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}
NCL::CSC8503::OscillateStateMachine::OscillateStateMachine(GameObject* obj)
{
	m_Object = obj;
	State* A = new State([this](float dt)->void
		{
			xangle = zangle = 0.0f;
			if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT))
			{
				xangle -= 10.0f * dt;
				if (xangle <= 45.0f && xangle >= -45.0f)
				{
					Quaternion currentorientation = m_Object->GetTransform().GetOrientation();
					Matrix4 rotation = Matrix4::Rotation(-10.0f * dt, Vector3(1, 0, 0)) * Matrix4(currentorientation);
					m_Object->GetTransform().SetOrientation(Quaternion(rotation));
				}
				xangle = clamp(xangle, -45.0f, 45.0f);

			}
			else if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT))
			{
				xangle += 10.0f * dt;
				if (xangle <= 45.0f && xangle >= -45.0f)
				{
					Quaternion currentorientation = m_Object->GetTransform().GetOrientation();
					Matrix4 rotation = Matrix4::Rotation(10.0f * dt, Vector3(1, 0, 0)) * Matrix4(currentorientation);
					m_Object->GetTransform().SetOrientation(Quaternion(rotation));
				}
				xangle = clamp(xangle, -45.0f, 45.0f);
			}
			else if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP))
			{
				zangle -= 10.0f * dt;
				if (zangle <= 45.0f && zangle >= -45.0f)
				{
					Quaternion currentorientation = m_Object->GetTransform().GetOrientation();
					Matrix4 rotation = Matrix4::Rotation(-10.0f * dt, Vector3(0, 0, 1)) * Matrix4(currentorientation);
					m_Object->GetTransform().SetOrientation(Quaternion(rotation));
				}
				zangle = clamp(zangle, -45.0f, 45.0f);
			}
			else if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN))
			{
				zangle += 10.0f * dt;
				if (zangle <= 45.0f && zangle >= -45.0f)
				{
					Quaternion currentorientation = m_Object->GetTransform().GetOrientation();
					Matrix4 rotation = Matrix4::Rotation(10.0f * dt, Vector3(0, 0, 1)) * Matrix4(currentorientation);
					m_Object->GetTransform().SetOrientation(Quaternion(rotation));
				}
				zangle = clamp(zangle, -45.0f, 45.0f);
			}
			else
			{
				return;
			}
		}, "InitialState"
	);
	AddState(A);
}

NCL::CSC8503::OscillateStateMachine::~OscillateStateMachine()
{
}

void NCL::CSC8503::OscillateStateMachine::Update(float dt)
{
	StateMachine::Update(dt);
}
