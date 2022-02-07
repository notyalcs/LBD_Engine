#include "../../Headers/Behaviours/Camera.h"

void Camera::SetPosition(const XMFLOAT3& v)
{
	_position = v;
	_viewDirty = true;
}

float Camera::GetFovX()const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / _nearZ);
}

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	_FovY = fovY;
	_aspect = aspect;
	_nearZ = zn;
	_farZ = zf;

	_nearWindowHeight = 2.0f * _nearZ * tanf(0.5f * _FovY);
	_farWindowHeight = 2.0f * _farZ * tanf(0.5f * _FovY);

	XMMATRIX P = XMMatrixPerspectiveFovLH(_FovY, _aspect, _nearZ, _farZ);
	XMStoreFloat4x4(&_projection, P);
}

void Camera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&_position, pos);
	XMStoreFloat3(&_lookAt, L);
	XMStoreFloat3(&_right, R);
	XMStoreFloat3(&_up, U);

	_viewDirty = true;
}

void Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);

	_viewDirty = true;
}

XMMATRIX Camera::GetView()const
{
	assert(!_viewDirty);
	return XMLoadFloat4x4(&_view);
}

XMMATRIX Camera::GetProjection()const
{
	return XMLoadFloat4x4(&_projection);
}


XMFLOAT4X4 Camera::GetView4x4f()const
{
	assert(!_viewDirty);
	return _view;
}

void Camera::Strafe(float d)
{
	// _position += d*_right
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&_right);
	XMVECTOR p = XMLoadFloat3(&_position);
	XMStoreFloat3(&_position, XMVectorMultiplyAdd(s, r, p));

	_viewDirty = true;
}

void Camera::Walk(float d)
{
	// _position += d*_lookAt
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&_lookAt);
	XMVECTOR p = XMLoadFloat3(&_position);
	XMStoreFloat3(&_position, XMVectorMultiplyAdd(s, l, p));

	_viewDirty = true;
}

void Camera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&_right), angle);

	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), R));
	XMStoreFloat3(&_lookAt, XMVector3TransformNormal(XMLoadFloat3(&_lookAt), R));

	_viewDirty = true;
}

void Camera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&_right, XMVector3TransformNormal(XMLoadFloat3(&_right), R));
	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), R));
	XMStoreFloat3(&_lookAt, XMVector3TransformNormal(XMLoadFloat3(&_lookAt), R));

	_viewDirty = true;
}

void Camera::UpdateViewMatrix()
{
	if (_viewDirty)
	{
		XMVECTOR R = XMLoadFloat3(&_right);
		XMVECTOR U = XMLoadFloat3(&_up);
		XMVECTOR L = XMLoadFloat3(&_lookAt);
		XMVECTOR P = XMLoadFloat3(&_position);

		// Keep camera's axes orthogonal to each other and of unit length.
		L = XMVector3Normalize(L);
		U = XMVector3Normalize(XMVector3Cross(L, R));

		// U, L already ortho-normal, so no need to normalize cross product.
		R = XMVector3Cross(U, L);

		// Fill in the view matrix entries.
		float x = -XMVectorGetX(XMVector3Dot(P, R));
		float y = -XMVectorGetX(XMVector3Dot(P, U));
		float z = -XMVectorGetX(XMVector3Dot(P, L));

		XMStoreFloat3(&_right, R);
		XMStoreFloat3(&_up, U);
		XMStoreFloat3(&_lookAt, L);

		_view(0, 0) = _right.x;
		_view(1, 0) = _right.y;
		_view(2, 0) = _right.z;
		_view(3, 0) = x;

		_view(0, 1) = _up.x;
		_view(1, 1) = _up.y;
		_view(2, 1) = _up.z;
		_view(3, 1) = y;

		_view(0, 2) = _lookAt.x;
		_view(1, 2) = _lookAt.y;
		_view(2, 2) = _lookAt.z;
		_view(3, 2) = z;

		_view(0, 3) = 0.0f;
		_view(1, 3) = 0.0f;
		_view(2, 3) = 0.0f;
		_view(3, 3) = 1.0f;

		_viewDirty = false;
	}
}
