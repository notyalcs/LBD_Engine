#include "..\..\..\Headers\Utilities\Triggers\Lava.h"
#include "../../../Headers/Behaviours/Player.h"

namespace LBD {
	void Lava::FireTrigger(GameObject* collidedObject)
	{
		if (collidedObject->GetBehaviour<PhysicsBody>()) {

			auto player = collidedObject->GetBehaviour<Player>();

			if (player)
			{
				player->SetDead(true);
				std::string playerName = player->GetName() + " has died!\n";
				std::wstring tempName = std::wstring(playerName.begin(), playerName.end());
				LPCWSTR stemp = tempName.c_str();
				OutputDebugString(stemp);
			}

			float XDead = -0.05f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.1f)));
			float YDead = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.05f)));
			float ZDead = -0.05f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.1f)));
			collidedObject->GetBehaviour<PhysicsBody>()->GetPhysics()->SetVelocity({ XDead, YDead, ZDead });
		}
	}
}
