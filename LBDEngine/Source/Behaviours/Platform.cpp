#include "../../Headers/Behaviours/Platform.h"

void Platform::Update()
{
	UpdateMovement();
}

void Platform::UpdateMovement()
{
	if (_movementPoints.empty()) return;

	auto& currentTranslation{ GetGameObject()->GetTranslation() };
	auto& currentTarget{ _movementPoints.at(_pointIndex) };

	if (_movementPoints.size() >= 2
		&& currentTranslation._41 == _movementPoints.at(_pointIndex)._41
		&& currentTranslation._42 == _movementPoints.at(_pointIndex)._42
		&& currentTranslation._43 == _movementPoints.at(_pointIndex)._43)
	{
		if (_isIndexingForward)
		{
			if (static_cast<size_t>(_pointIndex) + 1 >= _movementPoints.size())
			{
				_isIndexingForward = false;
				--_pointIndex;
			}
			else
			{
				++_pointIndex;
			}
		}
		else
		{
			if (_pointIndex - 1 <= 0)
			{
				_isIndexingForward = true;
				++_pointIndex;
			}
			else
			{
				--_pointIndex;
			}
		}
	}

	XMFLOAT4X4 deltaPosition;
	XMStoreFloat4x4(&deltaPosition, Utilities::MoveTowards(currentTranslation, _movementPoints.at(_pointIndex), _movementSpeed * GameTime::GetDeltaTime()));

	GetGameObject()->SetTranslation(XMMatrixTranslation(currentTranslation._41 + deltaPosition._41, currentTranslation._42 + deltaPosition._42, currentTranslation._43 + deltaPosition._43));
}
