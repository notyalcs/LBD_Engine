#include "../../Headers/Behaviours/Collider.h"

void Collider::Update()
{
	auto translation{ GetGameObject()->GetTranslation() };
	_boundingBox.Center = { translation._41, translation._42, translation._43 };
}

void Collider::FireTrigger(GameObject* collidedObject) {
	for (auto& trigger : Collider::_associatedTriggers) {
		trigger->FireTrigger(collidedObject);
	}
}
