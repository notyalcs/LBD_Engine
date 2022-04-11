#include "../../Headers/Behaviours/PhysicsBody.h"

void PhysicsBody::Start()
{
	_collider = GetGameObject()->GetBehaviour<Collider>();
	_mesh = GetGameObject()->GetBehaviour<Mesh>();
	_physics = GetGameObject()->GetBehaviour<Physics>();
}

void PhysicsBody::Update()
{
	auto boxCast{ _collider->GetBoundingBox() };
	XMFLOAT4X4 translationPostForce;
	auto velocity{ _physics->GetVelocity() };

	XMStoreFloat4x4(&translationPostForce, XMLoadFloat4x4(&GetGameObject()->GetTranslation()) * XMMatrixTranslation(velocity.x, velocity.y, velocity.z));
	boxCast.Center = { translationPostForce._41, translationPostForce._42, translationPostForce._43 };
	bool isColliding{ false };
	for (auto& other : GameState::GetBehavioursOfType<Collider>()) {
		if (_collider != other && boxCast.Intersects(other->GetBoundingBox())) {
			if (other->GetIsTrigger()) {
				other->FireTrigger(GetGameObject());
			}
			else
			{
				_physics->CollideWith(*other->GetGameObject());
				isColliding = true;
			}
		}
	}
	if (!isColliding) {
		GetGameObject()->SetTranslation(XMLoadFloat4x4(&translationPostForce));
	} else if (_collider->GetFloored()) {
		auto horizontalVelocity = XMLoadFloat4x4(&GetGameObject()->GetTranslation()) * XMMatrixTranslation(velocity.x, 0.0f, velocity.z);

		XMStoreFloat4x4(&translationPostForce, XMLoadFloat4x4(&GetGameObject()->GetTranslation()) * XMMatrixTranslation(velocity.x, 0.0, velocity.z));
		boxCast.Center = { translationPostForce._41, translationPostForce._42, translationPostForce._43 };
		isColliding = false;
		for (auto& other : GameState::GetBehavioursOfType<Collider>()) {
			if (_collider != other && boxCast.Intersects(other->GetBoundingBox())) {
				if (other->GetIsTrigger()) {
					other->FireTrigger(GetGameObject());
				}
				else
				{
					_physics->CollideWith(*other->GetGameObject());
					isColliding = true;
				}
			}
		}

		if (!isColliding)
		{
			GetGameObject()->SetTranslation(XMLoadFloat4x4(&translationPostForce));
		}
	}
}

void PhysicsBody::AddForce(XMFLOAT3 force)
{
	_physics->AddForce(force);
}
