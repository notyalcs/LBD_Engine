#pragma once
#include "Behaviour.h"

class Collider : public Behaviour
{
public:
	void Update() override;

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

private:
	BoundingBox _boundingBox;

};
