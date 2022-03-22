#define NOMINMAX
#include "../../Headers/Behaviours/Physics.h"

void Physics::Update()
{
	// linear motion
	// delta p, or change in momentum (impulse)
	XMFLOAT3 impulse{ 0.0f, 0.0f, 0.0f };
	XMStoreFloat3(&impulse, XMLoadFloat3(&_lm.derivative.force) * GameTime::GetDeltaTime());
	XMStoreFloat3(&_lm.state.momentum, XMLoadFloat3(&_lm.state.momentum) + XMLoadFloat3(&impulse));
	// recalculate velocity state
	_lm.state.Recalculate();
	XMStoreFloat3(&_lm.state.position, XMLoadFloat3(&_lm.state.position) + (XMLoadFloat3(&_lm.state.velocity) * GameTime::GetDeltaTime()));

	// clear force, instantaneous reactions.
	_lm.derivative.force = { 0.0f, 0.0f, 0.0f };

	// angular motion
}

void CollideWith(Physics& other)
{
	// TODO: collide
}

void Physics::AddForce(XMFLOAT3 force)
{
	_lm.derivative.force = force;
	// not sure if I actually need this but maybe. Not expensive and only runs when a force is added.
	XMFLOAT3 a{ 0.0f, 0.0f, 0.0f };
	XMStoreFloat3(&a, XMLoadFloat3(&_lm.derivative.force) * _lm.state.inverseMass);
	XMStoreFloat3(&_lm.derivative.velocity, XMLoadFloat3(&a) * GameTime::GetDeltaTime());
}

XMFLOAT3 Physics::GetVelocity()
{
	return _lm.state.velocity;
}