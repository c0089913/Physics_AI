#include "AxisConstraint.h"
#include "GameObject.h"

void NCL::CSC8503::AxisConstraint::UpdateConstraint(float dt)
{
	NCL::Maths::Vector3 velocity = object->GetPhysicsObject()->GetLinearVelocity();
	velocity = velocity * m_Axis;
	object->GetPhysicsObject()->SetLinearVelocity(velocity);
}
