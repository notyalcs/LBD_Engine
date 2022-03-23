#pragma once
#include "GameObject.h"

//Adds collision to an object.
//Allows collision between objects
class Trigger
{
public:
	virtual void FireTrigger(GameObject* CollidedObject) {};
private:
};
