#include "../../Headers/Main/GameData.h"

std::vector<std::unique_ptr<GameObject>> GameData::_gameObjects;
std::vector<GameObject*> GameData::_gameObjectsToStart;

GameObject* GameData::CreateGameObject()
{
	_gameObjects.push_back(std::make_unique<GameObject>());

	auto gameObject{ _gameObjects.back().get() };
	_gameObjectsToStart.push_back(gameObject);
	return gameObject;
}
