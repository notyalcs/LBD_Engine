#pragma once
#include "Behaviour.h"
#include "../Utilities/Trigger.h"
#include "../Main/GameState.h"

//Adds collision to an object.
//Allows collision between objects
class Collider : public Behaviour
{
public:
	void Update() override;

	void FireTrigger(GameObject*);

	void Transform(XMMATRIX transform)
	{
		_boundingBox.Transform(_boundingBox, transform);
	}

	BoundingBox GetBoundingBox()
	{
		return _boundingBox;
	}

	void SetBoundingBox(BoundingBox boundingBox)
	{
		_boundingBox = boundingBox;
	}

	void SetIsTrigger(bool status) {
		_isTrigger = status;
	}

	bool GetIsTrigger() {
		return _isTrigger;
	}

	void SetTriggered(bool status) {
		_triggered = status;
	}

	bool GetTriggered() {
		return _triggered;
	}

	void IsOnFloor();
	bool GetFloored() { return _floored; }

private:
	BoundingBox _boundingBox;
	bool _isTrigger;
	bool _triggered;
	std::vector<Trigger> _associatedTriggers;
	bool _floored;
};
