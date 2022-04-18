#pragma once
#include "..\Trigger.h"
#include "..\GameObject.h"

namespace LBD {
	class Lava : public Trigger {
		void FireTrigger(GameObject* CollidedObject) override;
	};
}
