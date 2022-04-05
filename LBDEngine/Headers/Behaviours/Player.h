#pragma once
#include "Behaviour.h"
#include "Controller.h"
#include "../Utilities/GameCameras.h"
#include "../Utilities/GameTime.h"
#include "PhysicsBody.h"

class Player : public Behaviour
{
public:
	void Start() override;
	void Update() override;
	void isOnFloor();
	bool GetFloored() { return _floored; }
private:
	Controller* _controller;
	bool _floored;
};
