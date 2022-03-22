#pragma once
#include "../Utilities/Utilities.h"

class LinearMotion
{
public:
	struct State 
	{
		// primary
		XMFLOAT3 position{ 0.0f, 0.0f, 0.0f }; // probably can be omitted, unused practically.
		XMFLOAT3 momentum{ 0.0f, 0.0f, 0.0f };

		// secondary
		XMFLOAT3 velocity{ 0.0f, 0.0f, 0.0f };

		// constant
		float mass{ 0.0f };
		float inverseMass{ 0.0f };

		State(float mass) : mass(mass), 
							inverseMass(1.0f / mass) {}
		State(float mass, XMFLOAT3 position) : mass(mass), 
											   inverseMass(1.0f / mass), 
											   position(position) {}

		void Recalculate()
		{
			XMStoreFloat3(&velocity, XMLoadFloat3(&momentum) * inverseMass);
		}
	};

	struct Derivative
	{
		XMFLOAT3 velocity{ 0.0f, 0.0f, 0.0f };
		XMFLOAT3 force{ 0.0f, 0.0f, 0.0f };
	};

	State state{ 1.0f };
	Derivative derivative{};

	LinearMotion(float mass) : state(mass) {}
	LinearMotion(float mass, XMFLOAT3 position) : state(mass, position) {}
};