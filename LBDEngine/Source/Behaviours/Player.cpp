#include "../../Headers/Behaviours/Player.h"

void Player::Start()
{
	_controller = GetGameObject()->GetBehaviour<Controller>();
}

void Player::Update()
{
	auto mainCamera{ GameCameras::GetMainCamera() };

	IsOnFloor();
	if (_controller->GetIsMouseDown()) {
		// Make each pixel correspond to a quarter of a degree.
		auto lastMousePosition{ _controller->GetLastMousePosition() };
		auto currentMousePosition{ _controller->GetCurrentMousePosition() };
		float deltaX = XMConvertToRadians(0.25f * static_cast<float>(currentMousePosition.x - lastMousePosition.x));
		float deltaY = XMConvertToRadians(0.25f * static_cast<float>(currentMousePosition.y - lastMousePosition.y));

		mainCamera->Pitch(deltaY);
		mainCamera->RotateY(deltaX);
	}

	const float deltaTime = GameTime::GetDeltaTime();

	auto body = GetGameObject()->GetTranslation();

	if (!GOD_MODE)
	{
		mainCamera->SetPosition({ body._41, body._42, body._43 });
	}
	else
	{
		mainCamera->Walk(_controller->GetVerticalAxis() * VERTICAL_MOVEMENT_SPEED * deltaTime);
		mainCamera->Strafe(_controller->GetHorizontalAxis() * HORIZONTAL_MOVEMENT_SPEED * deltaTime);
		auto position{ mainCamera->GetPosition() };
		auto rise{ _controller->GetKey('E') ? 1.0f : 0.0f + _controller->GetKey('Q') ? -1.0f : 0.0f };
		mainCamera->SetPosition({ position.x, position.y + rise * RISE_MOVEMENT_SPEED * deltaTime, position.z });
	}

	GameCameras::GetMainCamera()->UpdateViewMatrix();
}

void Player::IsOnFloor()
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
