#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <DirectXMath.h>
#include <cstdint>

#pragma comment(lib, "ws2_32.lib")

using namespace DirectX;

//Useful math

class MathHelper
{
public:
	// Returns a random float in the range [0, 1).
	static float RandomFloat() { return rand() / static_cast<float>(RAND_MAX); }

	// Returns a random float in the given range.
	static float RandomFloat(float fromInclusive, float toExclusive) { return fromInclusive + RandomFloat() * (toExclusive - fromInclusive); }

	// Returns a random int in the given range.
	static int RandomInt(int fromInclusive, int toExclusive) { return fromInclusive + rand() % ((toExclusive - fromInclusive) + 1); }

	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	static float AngleFromXY(float x, float y) { return std::atan2(y, x); }

	// Returns a clamped value between [a, b]. T should support operator<.
	template<class T>
	static T Clamp(T val, T min, T max) { return val < max ? min < val ? val : min : max; }

	static XMVECTOR SphericalToCartesian(float radius, float theta, float phi)
	{
		return XMVectorSet(
			radius * sinf(phi) * cosf(theta),
			radius * cosf(phi),
			radius * sinf(phi) * sinf(theta),
			1.0f);
	}

	static XMMATRIX InverseTranspose(CXMMATRIX M)
	{
		// Inverse-transpose is just applied to normals.  So zero out 
		// translation row so that it doesn't get into our inverse-transpose
		// calculation--we don't want the inverse-transpose of the translation.
		XMMATRIX A = M;
		A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		XMVECTOR det = XMMatrixDeterminant(A);
		return XMMatrixTranspose(XMMatrixInverse(&det, A));
	}

	static XMFLOAT4X4& CreateIdentity4x4()
	{
		static XMFLOAT4X4 I{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

		return I;
	}

	static XMFLOAT3X3& CreateIdentity3x3()
	{
		static XMFLOAT3X3 I{
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,};

		return I;
	}
};
