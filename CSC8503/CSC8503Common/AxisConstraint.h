#pragma once
#include "Constraint.h"
#include "../../Common/Vector3.h"
namespace NCL {
	namespace CSC8503 {
		class GameObject;

		class AxisConstraint : public Constraint {
		public:
			AxisConstraint(GameObject* a, NCL::Maths::Vector3 axis) {
				object = a;
				m_Axis = axis;
			}
			~AxisConstraint() {}

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* object;
			NCL::Maths::Vector3 m_Axis;

			float distance;
		};
	}
}