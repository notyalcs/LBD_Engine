#pragma once
#include "Behaviour.h"
#include "AngularMotion.h"
#include "LinearMotion.h"
#include "../Utilities/GameTime.h"

//Applies force to an object
class Physics : public Behaviour
{
private:
	LinearMotion _lm{ 1.0f };
	AngularMotion _am{};
	bool _gravity{ true };
	void Reflect(XMFLOAT3 normal);
public:
	// default mass of 1kg. 
	Physics() : _lm(1.0f) {};
	Physics(float mass) : _lm(mass) {};
	Physics(float mass, XMFLOAT3 position) : _lm(mass, position) {};

	void Update() override;
	void AddForce(XMFLOAT3 force);
	void SetMass(float mass) { _lm.state.mass = mass; _lm.state.inverseMass = 1.0f / mass; }
	void CollideWith(GameObject& other);
	XMFLOAT3 GetVelocity();
	void SetGravity(bool gravity);
};
