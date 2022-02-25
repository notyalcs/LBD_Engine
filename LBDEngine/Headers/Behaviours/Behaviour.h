#pragma once
#include "../Utilities/GameObject.h"

class GameObject;

class Behaviour {
public:
	virtual ~Behaviour() = default;

	//Called when the object is first created
	virtual void Start() {}

	//Called each frame
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
	//The game object that the behaviour is a part of
	GameObject* _gameObject;

};
