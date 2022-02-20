#pragma once
#include "Constraint.h"
#include "Transform.h"
#include "GameObject.h"
namespace NCL {
	namespace CSC8503 {
		class GameObject;

		class FixedConstraint : public Constraint {
		public:
			FixedConstraint(GameObject* a, GameObject* b) {
				objectA = a;
				objectB = b;
				diff = (a->GetTransform().GetPosition() - b->GetTransform().GetPosition());
			}
			~FixedConstraint() {}

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* objectA;
			GameObject* objectB;
			Vector3 diff;
		};
	}
}