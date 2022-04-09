#pragma once
#include "Game.h"
#include "../Behaviours/Controller.h"
#include "../Behaviours/Player.h"
#include "../Behaviours/Physics.h"
#include <thread>


class LBDGame
{
public:
	void StartGame();

private:
	// Player already exists inside of the gameObjects vector, but it's helpful to have a direct pointer to it.
	void BuildRenderItems();
	void CreatePlayer(XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f));

	GameObject* CreateMeshObject(std::string meshGeometryName, std::string submeshGeometryName, std::string materialName, XMMATRIX scale, XMMATRIX rotation, XMMATRIX translation, XMMATRIX textureTransform);
	GameObject* CreateDynamicMeshObject(std::string meshGeometryName, std::string submeshGeometryName, std::string materialName, float mass, XMMATRIX scale, XMMATRIX rotation, XMMATRIX translation, XMMATRIX textureTransform);

	void InitToServer();
	static void GetFromServer(char* message, char* buf, SOCKET s, int slen, sockaddr_in si_other);
	static int ParseInt(char c);

	std::thread _inputLoop;

	std::vector<GameObject*> _players;
	int _playerNum;
};

