#include "../../Headers/Behaviours/Collider.h"

void Collider::Update()
{
	auto translation{ GetGameObject()->GetTranslation() };
	_boundingBox.Center = { translation._41, translation._42, translation._43 };
}
