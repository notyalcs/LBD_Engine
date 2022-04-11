#pragma once
#include "..\Trigger.h"
#include "..\GameObject.h"

class Lava : public Trigger {
	void FireTrigger(GameObject* CollidedObject) override;
};