#include "../../Headers/Behaviours/Mesh.h"

UINT Mesh::_currentObjectCBIndex{ 0 };

void Mesh::Start()
{
	ObjCBIndex = _currentObjectCBIndex++;

	_recordedWorldTransform = MathHelper::CreateIdentity4x4();
}

void Mesh::Update()
{
	if (HasWorldTransformChanged())
	{
		XMMATRIX currentMatrix;
		auto currentWorldTransform{ GetGameObject()->GetWorldTransform() };
		memcpy(&currentMatrix, &currentWorldTransform, sizeof(XMMATRIX));
		XMStoreFloat4x4(&_recordedWorldTransform, currentMatrix);

		SetDirtyFrames(NUMBER_OF_FRAME_RESOURCES);
	}
}

bool Mesh::HasWorldTransformChanged()
{
	XMMATRIX currentMatrix;
	auto currentWorldTransform{ GetGameObject()->GetWorldTransform() };
	memcpy(&currentMatrix, &currentWorldTransform, sizeof(XMMATRIX));

	XMFLOAT4X4 current;
	XMStoreFloat4x4(&current, currentMatrix);

	bool hasChanged{ false };

	for (auto r = 0; r < 4; ++r)
	{
		for (auto c = 0; c < 4; ++c)
		{
			if (current.m[r][c] != _recordedWorldTransform.m[r][c]) {
				hasChanged = true;

				break;
			}
		}
	}

	return hasChanged;
}
