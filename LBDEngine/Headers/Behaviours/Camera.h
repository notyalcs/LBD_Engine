#pragma once
#include "../Utilities/Utilities.h"
#include "Behaviour.h"

class Camera : public Behaviour
{
public:
	// Get/Set world camera position.
	XMFLOAT3 GetPosition() const { return _position; }
	void SetPosition(const XMFLOAT3& v);

	// Get camera basis vectors.
	XMFLOAT3 GetRight3f() const { return _right; }
	XMFLOAT3 GetUp3f() const { return _up; }
	XMFLOAT3 GetLook3f() const { return _lookAt; }

	// Get frustum properties.
	float GetNearZ() const { return _nearZ; }
	float GetFarZ() const { return _farZ; }
	float GetAspect() const { return _aspect; }
	float GetFovY() const { return _FovY; }
	float GetFovX() const;

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth() const { return _aspect * _nearWindowHeight; }
	float GetNearWindowHeight() const { return _nearWindowHeight; }
	float GetFarWindowWidth() const { return _aspect * _farWindowHeight; }
	float GetFarWindowHeight() const { return _farWindowHeight; }

	// Set frustum.
	void SetLens(float fovY, float aspect, float zn, float zf);

	// Define camera space via LookAt parameters.
	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

	// Get View/Proj matrices.
	XMMATRIX GetView() const;
	XMMATRIX GetProjection() const;

	XMFLOAT4X4 GetView4x4f() const;
	XMFLOAT4X4 GetProjection4x4f() const { return _projection; }

	// Strafe/Walk the camera a distance d.
	void Strafe(float d);
	void Walk(float d);

	// Rotate the camera.
	void Pitch(float angle);
	void RotateY(float angle);

	// After modifying camera position/orientation, call to rebuild the view matrix.
	void UpdateViewMatrix();

private:
	// Camera coordinate system with coordinates relative to world space.
	XMFLOAT3 _position = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 _right = { 1.0f, 0.0f, 0.0f };
	XMFLOAT3 _up = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 _lookAt = { 0.0f, 0.0f, 1.0f };

	// Cache frustum properties.
	float _nearZ = 0.0f;
	float _farZ = 0.0f;
	float _aspect = 0.0f;
	float _FovY = 0.0f;
	float _nearWindowHeight = 0.0f;
	float _farWindowHeight = 0.0f;

	bool _viewDirty = true;

	// Cache View/Proj matrices.
	XMFLOAT4X4 _view = MathHelper::CreateIdentity4x4();
	XMFLOAT4X4 _projection = MathHelper::CreateIdentity4x4();

};
