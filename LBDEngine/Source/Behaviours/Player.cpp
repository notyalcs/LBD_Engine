#include "../../Headers/Behaviours/Player.h"

void Player::Start()
{
	_controller = GetGameObject()->GetBehaviour<Controller>();
}

void Player::Update()
{
	isOnFloor();
	if (_controller->GetIsMouseDown()) {
		// Make each pixel correspond to a quarter of a degree.
		auto lastMousePosition{ _controller->GetLastMousePosition() };
		auto currentMousePosition{ _controller->GetCurrentMousePosition() };
		float deltaX = XMConvertToRadians(0.25f * static_cast<float>(currentMousePosition.x - lastMousePosition.x));
		float deltaY = XMConvertToRadians(0.25f * static_cast<float>(currentMousePosition.y - lastMousePosition.y));

		GameCameras::GetMainCamera()->Pitch(deltaY);
		GameCameras::GetMainCamera()->RotateY(deltaX);
	}

	const float deltaTime = GameTime::GetDeltaTime();

	auto body = GetGameObject()->GetTranslation();
	GameCameras::GetMainCamera()->SetPosition({ body._41, body._42, body._43 });

	GameCameras::GetMainCamera()->UpdateViewMatrix();
}

void Player::isOnFloor()
{
	auto boxCast{ GetGameObject()->GetBehaviour<Collider>()->GetBoundingBox()};
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
