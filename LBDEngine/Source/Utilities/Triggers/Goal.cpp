#include "..\..\..\Headers\Utilities\Triggers\Goal.h"
#include "../../../Headers/Behaviours/Player.h"

void Goal::FireTrigger(GameObject* CollidedObject)
{
	if (CollidedObject->GetBehaviour<Player>()) {
		std::string playerName = CollidedObject->GetBehaviour<Player>()->GetName() + " Wins!\n";
		std::wstring tempName = std::wstring(playerName.begin(), playerName.end());
		LPCWSTR stemp = tempName.c_str();
		OutputDebugString(stemp);
	}
}
