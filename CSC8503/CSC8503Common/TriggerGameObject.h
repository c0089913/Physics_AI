#pragma once
#include "GameObject.h"
#include<functional>
namespace NCL {
	namespace CSC8503 {
		class TriggerGameObject :
			public GameObject
		{
		public:
			TriggerGameObject(std::function<void(GameObject *)> func, string name = "");
			std::function<void(GameObject *)> m_TriggerFunction;
			void SetFunction(std::function<void(GameObject *)> func) { m_TriggerFunction = func; }
			virtual void OnCollisionBegin(GameObject* otherObject);
		};
	}
}