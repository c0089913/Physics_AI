#include "RampStateMachine.h"
#include "State.h"
#include "GameObject.h"
#include "StateTransition.h"
#include "../../Common/Window.h"
NCL::CSC8503::RampStateMachine::RampStateMachine(GameObject* obj, Matrix4 rotation)
{
	m_Object = obj;
	m_Rotation = rotation;

	 State* A = new State([](float dt)->void
		{
			return;
		}, "Original"
	);

	 State* B = new State([this](float dt)->void
		{
			return;
		}, "Titled"
	);
	 StateTransition* stateAB = new StateTransition(A, B, [this](void)->bool
		{
		//	if (m_Object->pushed)
			 if(Window::GetKeyboard()->KeyPressed(KeyboardKeys::H))
			{	
				Quaternion orientation = m_Object->GetTransform().GetOrientation();

				this->m_InitialOrientation = orientation;
				/*orientation = orientation + Quaternion(Vector3(0,0,1),3.14f*0.75f);			
				orientation.Normalise();*/
				Matrix4 rotation = Matrix4::Rotation(135, Vector3(0, 0, 1));
				m_Object->GetTransform().SetOrientation(Quaternion(m_Rotation));

				return true;
			}
			return false;
		}
	);
	 StateTransition* stateBA = new StateTransition(B, A, [this](void)->bool
		{
			//if (m_Object->pushed)
			if(Window::GetKeyboard()->KeyPressed(KeyboardKeys::H))
			{
				m_Object->GetTransform().SetOrientation(this->m_InitialOrientation);				
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

NCL::CSC8503::RampStateMachine::~RampStateMachine()
{
}

void NCL::CSC8503::RampStateMachine::Update(float dt)
{
	StateMachine::Update(dt);
}
