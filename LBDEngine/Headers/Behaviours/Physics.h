#pragma once
#include "Behaviour.h"
#include "../Utilities/GameTime.h"

class Physics : public Behaviour
{
public:
	void Update() override;

	float _gravityScale{ 1.0f };
	XMFLOAT3 Force{ 0.0f, 0.0f, 0.0f };
	float _mass;

};
