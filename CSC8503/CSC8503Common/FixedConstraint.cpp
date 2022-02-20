#include "FixedConstraint.h"

void NCL::CSC8503::FixedConstraint::UpdateConstraint(float dt)
{
	Transform t = objectA->GetTransform();
	t.SetPosition(t.GetPosition() + this->diff);
	objectB->GetTransform().SetPosition(t.GetPosition());
	b->GetTransform().SetOrientation(t.GetOrientation());
}
