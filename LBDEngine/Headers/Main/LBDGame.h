#pragma once
#include "Game.h"
#include "../Behaviours/Controller.h"
#include "../Behaviours/Player.h"
#include "../Behaviours/Physics.h"

class LBDGame
{
public:
	void StartGame();

private:
	// Player already exists inside of the gameObjects vector, but it's helpful to have a direct pointer to it.
	GameObject* _player;

	void BuildRenderItems();
	void CreatePlayer();

	GameObject* CreateMeshObject(std::string meshGeometryName, std::string submeshGeometryName, std::string materialName, XMMATRIX scale, XMMATRIX rotation, XMMATRIX translation, XMMATRIX textureTransform);
	GameObject* CreateDynamicMeshObject(std::string meshGeometryName, std::string submeshGeometryName, std::string materialName, float mass, XMMATRIX scale, XMMATRIX rotation, XMMATRIX translation, XMMATRIX textureTransform);

};
