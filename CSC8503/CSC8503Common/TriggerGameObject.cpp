#include "TriggerGameObject.h"

NCL::CSC8503::TriggerGameObject::TriggerGameObject(std::function<void(GameObject *)> func, string name):GameObject(name)
{
	m_TriggerFunction = func;
}

void NCL::CSC8503::TriggerGameObject::OnCollisionBegin(GameObject* otherObject)
{
	m_TriggerFunction(otherObject);
}
