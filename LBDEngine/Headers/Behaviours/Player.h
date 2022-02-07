#pragma once
#include "Behaviour.h"
#include "Controller.h"
#include "../Utilities/GameCameras.h"
#include "../Utilities/GameTime.h"

class Player : public Behaviour
{
public:
	void Start() override;
	void Update() override;

private:
	Controller* _controller;

};
