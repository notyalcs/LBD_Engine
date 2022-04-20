#pragma once
#include "Behaviour.h"
#include "Controller.h"
#include "../Behaviours/Camera.h"
#include "../Utilities/GameCameras.h"
#include "../Utilities/GameTime.h"
#include "PhysicsBody.h"
#include "../Main/GameState.h"

namespace LBD {

	class Player : public Behaviour
	{
	public:
		void Start() override;
		void Update() override;
		void IsOnFloor();
		void SetFloored(bool floor) { _floored = floor; };
		bool GetFloored() { return _floored; };
		bool GetDead() { return _dead; }
		void SetDead(bool died) { _dead = died; }
		void SetName(std::string newName) { _name = newName; };
		std::string GetName() { return _name; };

	private:
		const float JumpHeight{ 0.04f };
		const float MovementSpeed{ 10.0f };

		std::string _name;

		Controller* _controller;
		bool _dead;
		bool _floored;
	};
}
