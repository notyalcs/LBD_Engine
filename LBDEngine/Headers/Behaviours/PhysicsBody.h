#pragma once
#include "Behaviour.h"
#include "../Main/Game.h"
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

private:
	Collider* _collider;
	Mesh* _mesh;
	Physics* _physics;

};
