#include "../../Headers/Behaviours/Player.h"

void Player::Start()
{
	_controller = GetGameObject()->GetBehaviour<Controller>();
	GetGameObject()->GetBehaviour<PhysicsBody>()->GetPhysics()->SetMass(80.0f);
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

	auto physics = GetGameObject()->GetBehaviour<PhysicsBody>()->GetPhysics();

	if (_controller->GetKey(VK_SPACE))
	{
		if (GetGameObject()->GetBehaviour<Physics>()->GetVelocity().y == 0.0f) {
			physics->SetVelocity({ 0.0f, JumpHeight, 0.0f });
		}
	}

	if (!GOD_MODE)
	{
		auto cameraLook = mainCamera->GetLook3f();
		auto cameraRight = mainCamera->GetRight3f();
		
		if (_controller->GetKey('W') && _controller->GetKey('A'))
		{
			physics->SetVelocityNotY({ (-1 * cameraRight.x + cameraLook.x) * GameTime::GetDeltaTime() * MovementSpeed, 0.0f, (-1 * cameraRight.z + cameraLook.z) * GameTime::GetDeltaTime() * MovementSpeed });
		}
		else if (_controller->GetKey('W') && _controller->GetKey('D'))
		{
			physics->SetVelocityNotY({ (cameraRight.x + cameraLook.x) * GameTime::GetDeltaTime() * MovementSpeed, 0.0f, (cameraRight.z + cameraLook.z) * GameTime::GetDeltaTime() * MovementSpeed });
		}
		else if (_controller->GetKey('S') && _controller->GetKey('A'))
		{
			physics->SetVelocityNotY({ -1 * (cameraRight.x + cameraLook.x) * GameTime::GetDeltaTime() * MovementSpeed, 0.0f, (-1 * cameraRight.z + cameraLook.z) * GameTime::GetDeltaTime() * MovementSpeed });
		}
		else if (_controller->GetKey('S') && _controller->GetKey('D'))
		{
			physics->SetVelocityNotY({ (cameraRight.x + -1 * cameraLook.x) * GameTime::GetDeltaTime() * MovementSpeed, 0.0f, (cameraRight.z + -1 * cameraLook.z) * GameTime::GetDeltaTime() * MovementSpeed });
		}
		else if (_controller->GetKey('W'))
		{
			physics->SetVelocityNotY({ cameraLook.x * GameTime::GetDeltaTime() * MovementSpeed, 0.0f, cameraLook.z * GameTime::GetDeltaTime() * MovementSpeed });
		}
		else if (_controller->GetKey('S'))
		{
			physics->SetVelocityNotY({ -1 * cameraLook.x * GameTime::GetDeltaTime() * MovementSpeed, 0.0f, -1 * cameraLook.z * GameTime::GetDeltaTime() * MovementSpeed });
		}
		else if (_controller->GetKey('A'))
		{
			physics->SetVelocityNotY({ -1 * cameraRight.x * GameTime::GetDeltaTime() * MovementSpeed, 0.0f, -1 * cameraRight.z * GameTime::GetDeltaTime() * MovementSpeed });
		}
		else if (_controller->GetKey('D'))
		{
			physics->SetVelocityNotY({ cameraRight.x * GameTime::GetDeltaTime() * MovementSpeed, 0.0f, cameraRight.z * GameTime::GetDeltaTime() * MovementSpeed });
		}

		mainCamera->UpdateOrbit(body, 5.0f);
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
