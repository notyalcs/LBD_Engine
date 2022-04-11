#include "../../../Headers/Utilities/Triggers/Goal.h"

void Goal::FireTrigger(GameObject* CollidedObject)
{
	auto player{ CollidedObject->GetBehaviour<Player>() };
	auto enemy{ CollidedObject->GetBehaviour<EnemyAI>() };
	if (player || enemy) {
		std::string playerName;

		if (player)
		{
			playerName = player->GetName() + " Wins!";
		}
		else
		{
			playerName = enemy->name + " Wins!";
		}

		std::wstring tempName = std::wstring(playerName.begin(), playerName.end());
		LPCWSTR stemp = tempName.c_str();

		auto hWnd{ Application::GetApplicationInstance()->GetMainWindow() };

		MessageBox(hWnd, stemp, TEXT("Winner Declared"), NULL);
		DestroyWindow(hWnd);
	}
}
