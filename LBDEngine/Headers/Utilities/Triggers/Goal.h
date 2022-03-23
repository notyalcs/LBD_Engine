#pragma once
#include "..\Trigger.h"
#include "..\GameObject.h"

class Goal : public Trigger {
	void FireTrigger(GameObject* CollidedObject) override;
};