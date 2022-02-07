#pragma once
#include "../Utilities/GameObject.h"

class GameObject;

class Behaviour {
public:
	virtual ~Behaviour() = default;
	virtual void Start() {}
	virtual void Update() {}

	GameObject* GetGameObject()
	{
		return _gameObject;
	}

	void SetGameObject(GameObject* gameObject)
	{
		_gameObject = gameObject;
	}

private:
	GameObject* _gameObject;

};
