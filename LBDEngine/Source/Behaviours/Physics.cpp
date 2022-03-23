#define NOMINMAX
#include "../../Headers/Behaviours/Physics.h"
#include "../../Headers/Utilities/Constants.h"

void Physics::Update()
{
	// gravity calculation
	if (_gravity)
	{
		XMFLOAT3 fg{ 0.00000005f, _lm.state.mass * GRAVITY * 0.00000005f, 0.0f };
		AddForce(fg);
	}

	// angular motion
}

void Physics::CollideWith(GameObject& other)
{
	// Simulating a closed system with instantaneous reactions. Probably doesn't handle 
	Physics* otherPhysics = other.GetBehaviour<Physics>();
	if (otherPhysics)
	{
		// add other force to myself, add my force to the other thing
		// not really how it works...
		XMFLOAT3 forceOther{ otherPhysics->_lm.derivative.force };
		otherPhysics->AddForce(_lm.derivative.force);
		AddForce(forceOther);
		return;
	}

	// other object has no physics component and therefore cannot move. Reflect myself along their face.
	// simplifying a very complex system really.
	Reflect({ 0.0f, 1.0f, 0.0f });
}

void Physics::AddForce(XMFLOAT3 force)
{
	XMStoreFloat3(&_lm.derivative.force, XMLoadFloat3(&_lm.derivative.force) + XMLoadFloat3(&force));
	// not sure if I actually need this but maybe. Not expensive and only runs when a force is added.
	XMFLOAT3 a{ 0.0f, 0.0f, 0.0f };
	XMStoreFloat3(&a, XMLoadFloat3(&_lm.derivative.force) * _lm.state.inverseMass);
	XMStoreFloat3(&_lm.derivative.velocity, XMLoadFloat3(&a) * GameTime::GetDeltaTime());

	// linear motion
	// delta p, or change in momentum (impulse)
	XMFLOAT3 impulse{ 0.0f, 0.0f, 0.0f };
	XMStoreFloat3(&impulse, XMLoadFloat3(&_lm.derivative.force) * GameTime::GetDeltaTime());
	XMStoreFloat3(&_lm.state.momentum, XMLoadFloat3(&_lm.state.momentum) + XMLoadFloat3(&impulse));
	// recalculate velocity state
	_lm.state.Recalculate();
	XMStoreFloat3(&_lm.state.position, XMLoadFloat3(&_lm.state.position) + (XMLoadFloat3(&_lm.state.velocity) * GameTime::GetDeltaTime()));
}

void Physics::Reflect(XMFLOAT3 normal)
{
	XMStoreFloat3(&_lm.state.momentum, XMVector3Reflect(XMLoadFloat3(&_lm.state.momentum), XMLoadFloat3(&normal)));
	// recalculate velocity state
	_lm.state.Recalculate();
	XMStoreFloat3(&_lm.state.position, XMLoadFloat3(&_lm.state.position) + (XMLoadFloat3(&_lm.state.velocity) * GameTime::GetDeltaTime()));
}

XMFLOAT3 Physics::GetVelocity()
{
	return _lm.state.velocity;
}

void Physics::SetGravity(bool gravity)
{
	_gravity = gravity;
}