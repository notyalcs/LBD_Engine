#include "../../Headers/Utilities/GameCameras.h"

Camera* GameCameras::GetMainCamera()
{
	auto camera{ _mainCameraObject.GetBehaviour<Camera>()};
	if (!camera)
	{
		camera = dynamic_cast<Camera*>(_mainCameraObject.AddBehaviour<Camera>());
	}

	return camera;
}

GameObject GameCameras::_mainCameraObject;
