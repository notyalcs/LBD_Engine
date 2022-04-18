#pragma once
#include "../Trigger.h"
#include "../GameObject.h"
#include "../../Behaviours/Player.h"
#include "../../Behaviours/EnemyAI.h"
#include "../../Main/Application.h"

namespace LBD {

	class Goal : public Trigger {
		void FireTrigger(GameObject* CollidedObject) override;
	};
}