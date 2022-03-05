#include "../../Headers/Utilities/Render.h"

int Render::_materialIndex{ 0 };

std::unordered_map<MeshGeometryName, std::unique_ptr<MeshGeometry>> Render::_geometries;
std::unordered_map<MaterialName, std::unique_ptr<Material>> Render::_materials;
std::unordered_map<PSOName, ComPtr<ID3D12PipelineState>> Render::_Psos;
std::unordered_map<ShaderName, ComPtr<ID3DBlob>> Render::_shaders;
std::unordered_map<TextureName, std::unique_ptr<Texture>> Render::_textures;

void Render::AddMaterial(MaterialName materialName, XMFLOAT4 diffuseAlbedo, XMFLOAT3 fresnelR0, float roughness) {
	auto material{ std::make_unique<Material>() };
	material->MatCBIndex = _materialIndex;
	material->DiffuseSrvHeapIndex = _materialIndex++;
	material->DiffuseAlbedo = diffuseAlbedo;
	material->FresnelR0 = fresnelR0;
	material->Roughness = roughness;

	_materials.insert(std::make_pair(materialName, std::move(material)));
}

void Render::AddPSO(PSOName psoName, ID3D12Device* device, std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout, ID3D12RootSignature* rootSignature) {
	D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePsoDesc{};
	opaquePsoDesc.InputLayout = { inputLayout.data(), (UINT)inputLayout.size() };
	opaquePsoDesc.pRootSignature = rootSignature;
	opaquePsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(_shaders.at(STANDARD_VS)->GetBufferPointer()),
		_shaders.at(STANDARD_VS)->GetBufferSize()
	};
	opaquePsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(_shaders.at(OPAQUE_PS)->GetBufferPointer()),
		_shaders.at(OPAQUE_PS)->GetBufferSize()
	};

	//my thing
	D3D12_RASTERIZER_DESC rast;
	rast = {
		D3D12_FILL_MODE_WIREFRAME,
		D3D12_CULL_MODE_BACK,
		false, 0, 0, 0, true, false, false, false };

//	opaquePsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(rast);
	opaquePsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	opaquePsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	opaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	opaquePsoDesc.SampleMask = UINT_MAX;
	opaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	opaquePsoDesc.NumRenderTargets = 1;
	opaquePsoDesc.RTVFormats[0] = BACK_BUFFER_FORMAT;
	opaquePsoDesc.SampleDesc.Count = 1;
	opaquePsoDesc.SampleDesc.Quality = 0;
	opaquePsoDesc.DSVFormat = DEPTH_STENCIL_FORMAT;

	ComPtr<ID3D12PipelineState> pipelineState;
	Utilities::ThrowIfFailed(device->CreateGraphicsPipelineState(&opaquePsoDesc, IID_PPV_ARGS(pipelineState.GetAddressOf())));
	_Psos.insert(std::make_pair(psoName, std::move(pipelineState)));
}

void Render::AddShader(std::wstring filePath, std::string entryPoint, ShaderName shaderName, std::string target) {
	ComPtr<ID3DBlob> byteCode;
	Utilities::ThrowIfFailed(D3DCompileFromFile(filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), target.c_str(), NULL, 0, byteCode.GetAddressOf(), NULL));
	
	_shaders.insert(std::make_pair(shaderName, byteCode));
}

void Render::AddTexture(std::wstring filePath, TextureName textureName, ID3D12Device* device, ID3D12GraphicsCommandList* graphicsCommandList) {
	auto texture{ std::make_unique<Texture>() };
	Utilities::ThrowIfFailed(CreateDDSTextureFromFile12(device, graphicsCommandList, filePath.c_str(), texture->Resource, texture->UploadHeap));

	_textures.insert(std::make_pair(textureName, std::move(texture)));
}
