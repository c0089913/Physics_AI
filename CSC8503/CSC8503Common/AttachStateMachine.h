#pragma once
#include "StateMachine.h"
#include "Transform.h"
#include "AABBVolume.h"
namespace NCL {
	namespace CSC8503 {
		class GameObject;
		class AttachStateMachine :
			public StateMachine
		{
		private:
			GameObject* m_Object;
			GameObject* m_AttachedObject;
			bool m_Attached;
			Transform m_ZoneTransform;
			AABBVolume *m_ZoneVolume;
		public:
			AttachStateMachine(GameObject* obj, GameObject *test);
			~AttachStateMachine();
			virtual void Update(float dt);
			void UpdateZoneTransform();
		};
	}
}