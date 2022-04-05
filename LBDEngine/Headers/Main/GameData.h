#pragma once
#include <vector>
#include "../Utilities/GameObject.h"
#include "../Utilities/Render.h"

class GameData
{
public:
	template<typename T>
	static std::vector<T*> GetBehavioursOfType()
	{
		std::vector<T*> behaviours;
		for (auto& gameObject : _gameObjects)
		{
			auto behaviour = dynamic_cast<T*>(gameObject->GetBehaviour<T>());
			if (behaviour)
			{
				behaviours.push_back(behaviour);
			}
		}

		return behaviours;
	}

	static GameObject* CreateGameObject();

	static std::vector<std::unique_ptr<GameObject>>& GetGameObjects()
	{
		return _gameObjects;
	}

	static std::vector<GameObject*>& GetGameObjectsToStart()
	{
		return _gameObjectsToStart;
	}

private:
	//Everything that exists in the world is a game object
	static std::vector<std::unique_ptr<GameObject>> _gameObjects;

	static std::vector<GameObject*> _gameObjectsToStart;

};
