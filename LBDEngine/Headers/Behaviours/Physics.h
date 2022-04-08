#pragma once
#include "Behaviour.h"
#include "AngularMotion.h"
#include "LinearMotion.h"
#include "../Utilities/MathHelper.h"
#include "../Utilities/GameTime.h"

//Applies force to an object
class Physics : public Behaviour
{
private:
	LinearMotion _lm{ 1.0f };
	AngularMotion _am{};
	bool _gravity{ true };
	float _elasticity{ 0.85f };
	void Reflect(XMFLOAT3 normal);
public:
	// default mass of 1kg. 
	Physics() : _lm(1.0f) {};
	Physics(float mass) : _lm(mass) {};
	Physics(float mass, XMFLOAT3 position) : _lm(mass, position) {};

	void Update() override;

	/*
	* BUG: Relies on GameTime::GetDeltaTime(), which is 0 if the game hasn't gone through
	* the creation of a single frame. It's significantly faster to only calculate everything
	* when a force is actually added though, so I'm leaving it like this.
	*/
	void AddForce(XMFLOAT3 force);
	void SetMass(float mass) { _lm.state.mass = mass; _lm.state.inverseMass = 1.0f / mass; }
	void CollideWith(GameObject& other);
	XMFLOAT3 GetVelocity() { return _lm.state.velocity; }
	void SetGravity(bool gravity) { _gravity = gravity; }
	
	/*
	 * Set a hardness value between [0, 1]
	 * An elasticity of 0 (fully inelastic) means that the object loses all energy upon collision with another.
	 * An elasticity of 1 (fully elastic) means that the object conserves all energy upon collision with another.
	 * Vastly oversimplifying how this works but honestly for a game engine I think
	 * this is a pretty good approximation of otherwise complicated things. Rigidbodies
	 * are already vastly oversimplifying physics.
	 */
	void SetElasticity(float elasticity) 
	{
		_elasticity = MathHelper::Clamp(elasticity, 0.0f, 1.0f);
	}
};
