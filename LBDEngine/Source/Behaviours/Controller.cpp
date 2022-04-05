#include "../../Headers/Behaviours/Controller.h"
#include "../../Headers/Behaviours/PhysicsBody.h"

void Controller::Update()
{
	ResetController();

	auto PhysicsBehaviour = GetGameObject()->GetBehaviour<PhysicsBody>()->GetPhysics();\

	if (GetAsyncKeyState('W') & HighBitMask)
	{
		_verticalAxis += 1.0f;
	}

	if (GetAsyncKeyState('S') & HighBitMask)
	{
		_verticalAxis -= 1.0f;
	}

	if (GetAsyncKeyState('A') & HighBitMask)
	{
		_horizontalAxis -= 1.0f;
	}

	if (GetAsyncKeyState('D') & HighBitMask)
	{
		_horizontalAxis += 1.0f;
	}

	if (GetAsyncKeyState(VK_SPACE) & HighBitMask) {
		if (GetGameObject()->GetBehaviour<Player>()->GetFloored()) {
  			PhysicsBehaviour->AddForce({ 0.0f, 10.0f * GameTime::GetDeltaTime(), 0.0f});
		}
	}

	PhysicsBehaviour->SetVelocity({ 0.0f, 0.0f, 0.0f });
	
	_isMouseDown = GetAsyncKeyState(VK_LBUTTON);
	_lastMousePosition = _currentMousePosition;
	GetCursorPos(&_currentMousePosition);
}

void Controller::ResetController()
{
	_horizontalAxis = 0.0f;
	_verticalAxis = 0.0f;
}
