#include "../../Headers/Behaviours/Controller.h"

void Controller::Update()
{
	ResetController();

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

	_isMouseDown = GetAsyncKeyState(VK_LBUTTON);
	_lastMousePosition = _currentMousePosition;
	GetCursorPos(&_currentMousePosition);
}

void Controller::ResetController()
{
	_horizontalAxis = 0.0f;
	_verticalAxis = 0.0f;
}
