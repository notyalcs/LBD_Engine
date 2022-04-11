#pragma once
#include "Behaviour.h"
#include "../Main/GameState.h"
#include "../Utilities/GameTime.h"
#include "Collider.h"
#include "Mesh.h"
#include "Physics.h"

/*
* Defines a dynamic object in the world,
* one which has a collider, mesh, and physics.
* Applies physics and collides with other colliders.
*/
class PhysicsBody : public Behaviour
{
public:
	void Start() override;
	void Update() override;

	/*
	* BUG: Relies on GameTime::GetDeltaTime(), which is 0 if the game hasn't gone through
	* the creation of a single frame. It's significantly faster to only calculate everything
	* when a force is actually added though, so I'm leaving it like this.
	*/
	void AddForce(XMFLOAT3 force);

	Physics* GetPhysics() {
		return _physics;
	}

private:
	Collider* _collider{ nullptr };
	Mesh* _mesh{ nullptr };
	Physics* _physics{ nullptr };

};
