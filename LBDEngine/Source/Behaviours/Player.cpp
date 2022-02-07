#include "../../Headers/Behaviours/Player.h"

void Player::Start()
{
	_controller = GetGameObject()->GetBehaviour<Controller>();
}

void Player::Update()
{
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

	GameCameras::GetMainCamera()->Walk(_controller->GetVerticalAxis() * VERTICAL_MOVEMENT_SPEED * deltaTime);
	GameCameras::GetMainCamera()->Strafe(_controller->GetHorizontalAxis() * HORIZONTAL_MOVEMENT_SPEED * deltaTime);

	GameCameras::GetMainCamera()->UpdateViewMatrix();
}
