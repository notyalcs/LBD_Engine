#pragma once
#include "Utilities.h"
#include "DDSTextureLoader.h"

//Useful functions, enums, and other resources for rendering

enum MeshGeometryName {
	SHAPE
};

enum SubmeshGeometryName {
	BOX,
	CYLINDER,
	GRID,
	SPHERE,
	CUSTOM
};

enum TextureName {
	TEX_BRICK,
	TEX_CRATE,
	TEX_CHAIR,
	TEX_STONE,
	TEX_TILE,
	TEX_TREE
};

enum MaterialName {
	MAT_BRICK,
	MAT_CRATE,
	MAT_CHAIR,
	MAT_STONE,
	MAT_TILE,
	MAT_TREE
};

enum ShaderName {
	STANDARD_VS,
	OPAQUE_PS
};

enum PSOName {
	PSO_OPAQUE
};

/*
* Defines a subrange of vertices and indices in larger MeshGeometry buffers.
*/
struct SubmeshGeometry
{
	UINT IndexCount{ 0 };
	UINT StartIndexLocation{ 0 };
	UINT BaseVertexLocation{ 0 };

	BoundingBox Bounds;
};

/*
* A MeshGeometry has a vertex and an index buffer.
* These buffers can hold multiple smaller geometries (SubmeshGeometry).
* The reason for this is performance. There is overhead for switching buffers.
* SubmeshGeometries can be retrieved through the Submeshes map.
*/
struct MeshGeometry
{
	ComPtr<ID3DBlob> VertexBufferCPU{ nullptr };
	ComPtr<ID3DBlob> IndexBufferCPU{ nullptr };

	ComPtr<ID3D12Resource> VertexBufferGPU{ nullptr };
	ComPtr<ID3D12Resource> IndexBufferGPU{ nullptr };

	ComPtr<ID3D12Resource> VertexBufferUploader{ nullptr };
	ComPtr<ID3D12Resource> IndexBufferUploader{ nullptr };

	UINT VertexByteStride{ 0 };
	UINT VertexBufferByteSize{ 0 };
	DXGI_FORMAT IndexFormat{ DXGI_FORMAT_R16_UINT };
	UINT IndexBufferByteSize{ 0 };

	std::unordered_map<SubmeshGeometryName, SubmeshGeometry> Submeshes;

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const
	{
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		vertexBufferView.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
		vertexBufferView.StrideInBytes = VertexByteStride;
		vertexBufferView.SizeInBytes = VertexBufferByteSize;

		return vertexBufferView;
	}

	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const
	{
		D3D12_INDEX_BUFFER_VIEW indexBufferView;
		indexBufferView.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
		indexBufferView.Format = IndexFormat;
		indexBufferView.SizeInBytes = IndexBufferByteSize;

		return indexBufferView;
	}

	// For freeing memory after uploading to the GPU.
	void DeleteUploaders()
	{
		VertexBufferUploader.Reset();
		IndexBufferUploader.Reset();
	}
};

struct Texture
{
	ComPtr<ID3D12Resource> Resource{ nullptr };
	ComPtr<ID3D12Resource> UploadHeap{ nullptr };
};

struct Material
{

	// Index into constant buffer corresponding to this material.
	int MatCBIndex = -1;

	// Index into SRV heap for diffuse texture.
	int DiffuseSrvHeapIndex = -1;

	// Index into SRV heap for normal texture.
	int NormalSrvHeapIndex = -1;

	// Dirty flag indicating the material has changed and we need to update the constant buffer.
	// Because we have a material constant buffer for each FrameResource, we have to apply the
	// update to each FrameResource.  Thus, when we modify a material we should set 
	// _dirtyFrames = NUMBER_OF_FRAME_RESOURCES so that each frame resource gets the update.
	int DirtyFrames = NUMBER_OF_FRAME_RESOURCES;

	// Material constant buffer data used for shading.
	XMFLOAT4 DiffuseAlbedo{ 1.0f, 1.0f, 1.0f, 1.0f };
	XMFLOAT3 FresnelR0{ 0.01f, 0.01f, 0.01f };
	float Roughness{ 0.25f };
	XMFLOAT4X4 MatTransform = MathHelper::CreateIdentity4x4();
};

class Render
{
public:
	Render() = delete;
	Render(const Render& rhs) = delete;
	void operator=(const Render& rhs) = delete;

	static void AddGeometry(MeshGeometryName meshGeometryName, std::unique_ptr<MeshGeometry>&& meshGeometry) { _geometries.insert(std::make_pair(meshGeometryName, std::move(meshGeometry))); }
	static void AddMaterial(MaterialName materialName, XMFLOAT4 diffuseAlbedo, XMFLOAT3 fresnelR0, float roughness);
	static void AddPSO(PSOName psoName, ID3D12Device* device, std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout, ID3D12RootSignature* rootSignature);
	static void AddShader(std::wstring filePath, std::string entryPoint, ShaderName shaderName, std::string target);
	static void AddTexture(std::wstring filePath, TextureName textureName, ID3D12Device* device, ID3D12GraphicsCommandList* graphicsCommandList);

	static std::unordered_map<MeshGeometryName, std::unique_ptr<MeshGeometry>>& GetGeometries() { return _geometries; }
	static std::unordered_map<MaterialName, std::unique_ptr<Material>>& GetMaterials() { return _materials; }
	static std::unordered_map<PSOName, ComPtr<ID3D12PipelineState>>& GetPSOs() { return _Psos; }
	static std::unordered_map<ShaderName, ComPtr<ID3DBlob>>& GetShaders() { return _shaders; }
	static std::unordered_map<TextureName, std::unique_ptr<Texture>>& GetTextures() { return _textures; }

private:
	static int _materialIndex;

	static std::unordered_map<MeshGeometryName, std::unique_ptr<MeshGeometry>> _geometries;
	static std::unordered_map<MaterialName, std::unique_ptr<Material>> _materials;
	static std::unordered_map<PSOName, ComPtr<ID3D12PipelineState>> _Psos;
	static std::unordered_map<ShaderName, ComPtr<ID3DBlob>> _shaders;
	static std::unordered_map<TextureName, std::unique_ptr<Texture>> _textures;
};
