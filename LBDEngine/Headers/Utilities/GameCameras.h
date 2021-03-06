#pragma once
#include "GameObject.h"
#include "../Behaviours/Camera.h"

namespace LBD {
//	class Camera;

	//Gives access to the main camera, usually the player one

	class GameCameras
	{
	public:
		GameCameras() = delete;
		GameCameras(const GameCameras& rhs) = delete;
		void operator=(const GameCameras& rhs) = delete;

		static LBD::Camera* GetMainCamera();

	private:
		static GameObject _mainCameraObject;

	};
}
