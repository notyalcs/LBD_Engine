#include "../../Headers/Utilities/Render.h"

namespace LBD {

	int Render::_materialIndex{ 0 };

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> Render::_geometries;
	std::unordered_map<std::string, std::unique_ptr<Material>> Render::_materials;
	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> Render::_Psos;
	std::unordered_map<std::string, ComPtr<ID3DBlob>> Render::_shaders;
	std::unordered_map<std::string, std::unique_ptr<Texture>> Render::_textures;

	void Render::AddMaterial(std::string materialName, XMFLOAT4 diffuseAlbedo, XMFLOAT3 fresnelR0, float roughness) {
		auto material{ std::make_unique<Material>() };
		material->MatCBIndex = _materialIndex;
		material->DiffuseSrvHeapIndex = _materialIndex++;
		material->DiffuseAlbedo = diffuseAlbedo;
		material->FresnelR0 = fresnelR0;
		material->Roughness = roughness;

		_materials.insert(std::make_pair(materialName, std::move(material)));
	}

	void Render::AddPSO(std::string psoName, ID3D12Device* device, std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout, ID3D12RootSignature* rootSignature) {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePsoDesc{};
		opaquePsoDesc.InputLayout = { inputLayout.data(), (UINT)inputLayout.size() };
		opaquePsoDesc.pRootSignature = rootSignature;
		opaquePsoDesc.VS =
		{
			reinterpret_cast<BYTE*>(_shaders.at("standard_vs")->GetBufferPointer()),
			_shaders.at("standard_vs")->GetBufferSize()
		};
		opaquePsoDesc.PS =
		{
			reinterpret_cast<BYTE*>(_shaders.at("opaque_ps")->GetBufferPointer()),
			_shaders.at("opaque_ps")->GetBufferSize()
		};

		auto rast = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		rast.FillMode = (WIREFRAME_STATE) ? D3D12_FILL_MODE_WIREFRAME : D3D12_FILL_MODE_SOLID;

		opaquePsoDesc.RasterizerState = rast;
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

	void Render::AddShader(std::wstring filePath, std::string entryPoint, std::string shaderName, std::string target) {
		ComPtr<ID3DBlob> byteCode;
		Utilities::ThrowIfFailed(D3DCompileFromFile(filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), target.c_str(), NULL, 0, byteCode.GetAddressOf(), NULL));

		_shaders.insert(std::make_pair(shaderName, byteCode));
	}

	void Render::AddTexture(std::wstring filePath, std::string textureName, ID3D12Device* device, ID3D12GraphicsCommandList* graphicsCommandList) {
		auto texture{ std::make_unique<Texture>() };
		Utilities::ThrowIfFailed(CreateDDSTextureFromFile12(device, graphicsCommandList, filePath.c_str(), texture->Resource, texture->UploadHeap));

		_textures.insert(std::make_pair(textureName, std::move(texture)));
	}
}
