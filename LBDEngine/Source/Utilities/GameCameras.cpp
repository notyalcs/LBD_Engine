#include "../../Headers/Utilities/GameCameras.h"

namespace LBD {
	Camera* GameCameras::GetMainCamera()
	{
		auto camera{ _mainCameraObject.GetBehaviour<Camera>() };
		if (!camera)
		{
			camera = dynamic_cast<LBD::Camera*>(_mainCameraObject.AddBehaviour<Camera>());
		}

		return camera;
	}

	GameObject GameCameras::_mainCameraObject;
}
