#pragma once
#include "Behaviour.h"
#include "Controller.h"
#include "../Utilities/GameCameras.h"
#include "../Utilities/GameTime.h"
#include "PhysicsBody.h"
#include "../Main/GameState.h"

class Player : public Behaviour
{
public:
	void Start() override;
	void Update() override;
	void IsOnFloor();
	bool GetFloored() { return _floored; }

private:
	const float JumpHeight{ 0.015f };
	const float MovementSpeed{ 4.0f };

	Controller* _controller;
	bool _floored;
};
