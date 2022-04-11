#include "..\..\..\Headers\Utilities\Triggers\Lava.h"
#include "../../../Headers/Behaviours/Player.h"

void Lava::FireTrigger(GameObject* CollidedObject)
{
	if (CollidedObject->GetBehaviour<Player>()) {
		std::string playerName = CollidedObject->GetBehaviour<Player>()->GetName() + " has died!\n";
		std::wstring tempName = std::wstring(playerName.begin(), playerName.end());
		LPCWSTR stemp = tempName.c_str();
		OutputDebugString(stemp);

		CollidedObject->GetBehaviour<Player>()->SetDead(true);
		float XDead = -0.05f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.1f)));
		float YDead = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.05f)));
		float ZDead = -0.05f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.1f)));
		CollidedObject->GetBehaviour<PhysicsBody>()->GetPhysics()->SetVelocity({ XDead, YDead, ZDead});
	}
}
