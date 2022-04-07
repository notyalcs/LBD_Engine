#define NOMINMAX
#include "../../Headers/Behaviours/Physics.h"
#include "../../Headers/Utilities/Constants.h"

void Physics::Update()
{
	// gravity calculation
	if (_gravity)
	{
		XMFLOAT3 fg{ 0.0f, _lm.state.mass * GRAVITY * GRAVITY_ADJUST, 0.0f };
		AddForce(std::forward<XMFLOAT3>(fg));
	}

	// Linear Motion
	XMStoreFloat3(&_lm.state.position, XMLoadFloat3(&_lm.state.position) + (XMLoadFloat3(&_lm.state.velocity) * GameTime::GetDeltaTime()));

	// angular motion
}

void Physics::CollideWith(GameObject& other)
{
	// Simulating a closed system with instantaneous reactions.
	Physics* otherPhysics = other.GetBehaviour<Physics>();
	if (otherPhysics)
	{
		// only calculates the forces on THIS object. The other object will also collide
		// on the same frame, and it will calculate this object's forces on itself.

		// linear component
		FXMVECTOR iVOther = XMLoadFloat3(&otherPhysics->_lm.state.velocity);
		FXMVECTOR iVThis = XMLoadFloat3(&_lm.state.velocity);
		float massO{ otherPhysics->_lm.state.mass };
		float massT{ _lm.state.mass };

		XMFLOAT3 fVThis{ 0.0f, 0.0f, 0.0f };
		// some magic math assuming that pi = pf and Ki = Kf. Only works for fully elastic collisions
		// but a good approximation of the real thing with elasticity adjustment.
		XMStoreFloat3(&fVThis, ((massT - massO) * iVThis + 2 * massO * iVOther) / (massT + massO));

		float totalElasticity = _elasticity * otherPhysics->_elasticity;
		// p = mv
		XMStoreFloat3(&_lm.state.momentum, totalElasticity * XMLoadFloat3(&fVThis) * _lm.state.mass);
		_lm.state.Recalculate();

		// angular component
		// TODO: angular momentum
		return;
	}

	// linear component
	// other object has no physics component and therefore cannot move. Reflect myself along their face.
	Reflect({ 0.0f, 1.0f, 0.0f });

	// angular component
	// TODO: angular momentum
}

void Physics::AddForce(XMFLOAT3&& force)
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
}

void Physics::Reflect(XMFLOAT3&& normal)
{
	XMStoreFloat3(&_lm.state.momentum, _elasticity * XMVector3Reflect(XMLoadFloat3(&_lm.state.momentum), XMLoadFloat3(&normal)));
	// recalculate velocity state
	_lm.state.Recalculate();
}

void Physics::SetVelocity(XMFLOAT3&& velocity)
{
	XMStoreFloat3(&_lm.derivative.velocity, XMLoadFloat3(&velocity));
	XMStoreFloat3(&_lm.state.momentum, XMLoadFloat3(&velocity) * _lm.state.mass);
	_lm.state.Recalculate();
	// using GameTime::GetDeltaTime() is possibly a bug here, as this isn't actually when this object is being updated.
	XMStoreFloat3(&_lm.derivative.force, XMLoadFloat3(&_lm.state.momentum) * GameTime::GetDeltaTime());
}