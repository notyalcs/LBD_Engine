#include "../../Headers/Behaviours/Collider.h"

void Collider::Update()
{
	auto translation{ GetGameObject()->GetTranslation() };
	_boundingBox.Center = { translation._41, translation._42, translation._43 };
	IsOnFloor();
}

void Collider::FireTrigger(GameObject* collidedObject) {
	for (auto& trigger : Collider::_associatedTriggers) {
		trigger.FireTrigger(collidedObject);
	}
}

void Collider::IsOnFloor()
{
	//auto boxCast{ GetGameObject()->GetBehaviour<Collider>()->GetBoundingBox() };
	auto boxCast{ _boundingBox };
	XMFLOAT4X4 translationPostForce;
	XMFLOAT3 velocity{ 0, -0.005f, 0 };
	XMStoreFloat4x4(&translationPostForce, XMLoadFloat4x4(&GetGameObject()->GetTranslation()) * XMMatrixTranslation(velocity.x, velocity.y, velocity.z));
	boxCast.Center = { translationPostForce._41, translationPostForce._42, translationPostForce._43 };
	bool isColliding{ false };
	for (auto& other : GameState::GetBehavioursOfType<Collider>()) {
		if (GetGameObject()->GetBehaviour<Collider>() != other && boxCast.Intersects(other->GetBoundingBox())) {
			isColliding = true;
		}
	}
	_floored = isColliding;
}
