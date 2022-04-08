#include "../../Headers/Main/GameState.h"

std::vector<std::unique_ptr<GameObject>> GameState::_gameObjects;
std::vector<GameObject*> GameState::_gameObjectsToStart;

GameObject* GameState::CreateGameObject()
{
	_gameObjects.push_back(std::make_unique<GameObject>());

	auto gameObject{ _gameObjects.back().get() };
	_gameObjectsToStart.push_back(gameObject);

	return gameObject;
}
