#pragma once
#include "../Utilities/Utilities.h"
#include "Behaviour.h"
#include "../Utilities/Render.h"

//The visual part of an object. The vertices that get drawn
class Mesh : public Behaviour
{
public:
	UINT GetDirtyFrames()
	{
		return _dirtyFrames;
	}

	void SetDirtyFrames(UINT dirtyFrames)
	{
		_dirtyFrames = dirtyFrames;
	}

	XMFLOAT4X4& GetTextureTransform()
	{
		return _textureTransform;
	}

	void SetTextureTransform(XMMATRIX textureTransform)
	{
		XMStoreFloat4x4(&_textureTransform, textureTransform);
	}

	// Index into GPU constant buffer corresponding to the ObjectCB for this render item.
	UINT ObjCBIndex = -1;

	Material* Mat = nullptr;
	MeshGeometry* Geo = nullptr;

	// Primitive topology.
	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// DrawIndexedInstanced parameters.
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	int BaseVertexLocation = 0;

private:
	// The number of frame resources where the object data has changed.
	UINT _dirtyFrames{ NUMBER_OF_FRAME_RESOURCES };

	XMFLOAT4X4 _textureTransform{ MathHelper::CreateIdentity4x4() };

};
