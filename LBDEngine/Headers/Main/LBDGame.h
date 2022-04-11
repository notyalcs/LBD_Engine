#pragma once
#include "Game.h"
#include "../Behaviours/Controller.h"
#include "../Behaviours/Player.h"
#include "../Behaviours/Physics.h"
#include "../Behaviours/Platform.h"
#include "../Behaviours/EnemyAI.h"
#include "../Utilities/Triggers/Goal.h"
#include "../Utilities/Triggers/Lava.h"


class LBDGame
{
public:
	std::vector<GameObject*> StartGame();
	int playerNum;

private:
	// Player already exists inside of the gameObjects vector, but it's helpful to have a direct pointer to it.
	void BuildRenderItems();
	void CreatePlayer(XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f));
	void CreateEnemy();

	GameObject* CreateMeshObject(std::string meshGeometryName, std::string submeshGeometryName, std::string materialName, XMMATRIX scale, XMMATRIX rotation, XMMATRIX translation, XMMATRIX textureTransform);
	GameObject* CreateDynamicMeshObject(std::string meshGeometryName, std::string submeshGeometryName, std::string materialName, float mass, XMMATRIX scale, XMMATRIX rotation, XMMATRIX translation, XMMATRIX textureTransform);
	GameObject* CreateTriggerZone(std::string meshGeometryName, std::string submeshGeometryName, std::string materialName, XMMATRIX scale, XMMATRIX rotation, XMMATRIX translation, XMMATRIX textureTransform, Trigger* trigger);
	std::vector<GameObject*> _players;
};

