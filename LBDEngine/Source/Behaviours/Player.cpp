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
		auto cameraLook = XMVector3Normalize({ mainCamera->GetLook3f().x, 0.0f, mainCamera->GetLook3f().z});
		float cameraLookX = XMVectorGetX(cameraLook);
		float cameraLookZ = XMVectorGetZ(cameraLook);

		auto cameraRight = XMVector3Normalize({ mainCamera->GetRight3f().x, 0.0f, mainCamera->GetRight3f().z });
		float cameraRightX = XMVectorGetX(cameraRight);
		float cameraRightZ = XMVectorGetZ(cameraRight);
		
		if (_controller->GetKey('W') && _controller->GetKey('A'))
		{
			auto xInput = (-1 * cameraRightX + cameraLookX) * GameTime::GetDeltaTime() * MovementSpeed;
			auto zInput = (-1 * cameraRightZ + cameraLookZ) * GameTime::GetDeltaTime() * MovementSpeed;

			physics->SetVelocityNotY({ xInput, 0.0f, zInput });
		}
		else if (_controller->GetKey('W') && _controller->GetKey('D'))
		{
			auto xInput = (cameraRightX + cameraLookX) * GameTime::GetDeltaTime() * MovementSpeed;
			auto zInput = (cameraRightZ + cameraLookZ) * GameTime::GetDeltaTime() * MovementSpeed;

			physics->SetVelocityNotY({ xInput, 0.0f, zInput });
		}
		else if (_controller->GetKey('S') && _controller->GetKey('A'))
		{
			auto xInput = -1 * (cameraRightX + cameraLookX) * GameTime::GetDeltaTime() * MovementSpeed;
			auto zInput = -1 * (cameraRightZ + cameraLookZ) * GameTime::GetDeltaTime() * MovementSpeed;

			physics->SetVelocityNotY({ xInput, 0.0f, zInput });
		}
		else if (_controller->GetKey('S') && _controller->GetKey('D'))
		{
			auto xInput = (cameraRightX + -1 * cameraLookX) * GameTime::GetDeltaTime() * MovementSpeed;
			auto zInput = (cameraRightZ + -1 * cameraLookZ) * GameTime::GetDeltaTime() * MovementSpeed;

			physics->SetVelocityNotY({ xInput , 0.0f, zInput });
		}
		else if (_controller->GetKey('W'))
		{
			auto xInput = cameraLookX * GameTime::GetDeltaTime() * MovementSpeed;
			auto zInput = cameraLookZ * GameTime::GetDeltaTime() * MovementSpeed;

			physics->SetVelocityNotY({ xInput, 0.0f, zInput });
		}
		else if (_controller->GetKey('S'))
		{
			auto xInput = -1 * cameraLookX * GameTime::GetDeltaTime() * MovementSpeed;
			auto zInput = -1 * cameraLookZ * GameTime::GetDeltaTime() * MovementSpeed;

			physics->SetVelocityNotY({ xInput, 0.0f, zInput });
		}
		else if (_controller->GetKey('A'))
		{
			auto xInput = -1 * cameraRightX * GameTime::GetDeltaTime() * MovementSpeed;
			auto zInput = -1 * cameraRightZ * GameTime::GetDeltaTime() * MovementSpeed;

			physics->SetVelocityNotY({ xInput, 0.0f, zInput });
		}
		else if (_controller->GetKey('D'))
		{
			auto xInput = cameraRightX * GameTime::GetDeltaTime() * MovementSpeed;
			auto zInput = cameraRightZ * GameTime::GetDeltaTime() * MovementSpeed;

			physics->SetVelocityNotY({ xInput, 0.0f, zInput });
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
