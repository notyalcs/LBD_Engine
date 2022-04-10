#pragma once
#include "../Utilities/Utilities.h"
#include "../Utilities/GameTime.h"
#include "Behaviour.h"

class Platform : public Behaviour
{
public:
	void Update() override;
	void AddMovementPoint(XMVECTOR point)
	{
		XMFLOAT4X4 newPoint;
		XMStoreFloat4x4(&newPoint, XMMatrixTranslationFromVector(point));

		_movementPoints.push_back(newPoint);
	}

private:
	const float _movementSpeed{ 5.0f };

	int _pointIndex{ 0 };
	bool _isIndexingForward{ true };
	std::vector<XMFLOAT4X4> _movementPoints;

	void UpdateMovement();

};

