#pragma once
#include "../Utilities/Utilities.h"
#include "Application.h"
#include "../Utilities/FrameResource.h"
#include "../Utilities/GeometryGenerator.h"
#include "../Utilities/GameObject.h"
#include "../Utilities/WavefrontReader.h"
#include "../Behaviours/Mesh.h"
#include "LBDGame.h"
#include "GameState.h"
#include "../Utilities/GameCameras.h"

//Extends application, provides features to application
//

class Game : public Application
{
public:
	Game(HINSTANCE hInstance) : Application{ hInstance } {}
	Game(const Game& rhs) = delete;
	void operator=(const Game& rhs) = delete;
	~Game() override = default;

	bool Initialize() override;

private:
	std::vector<std::unique_ptr<FrameResource>> _frameResources;
	FrameResource* _currentFrameResource = nullptr;
	int _currentFrameResourceIndex = 0;

	UINT _CbvSrvDescriptorSize = 0;

	ComPtr<ID3D12RootSignature> _rootSignature = nullptr;
	ComPtr<ID3D12DescriptorHeap> _SrvDescriptorHeap = nullptr;
	std::vector<D3D12_INPUT_ELEMENT_DESC> _inputLayout;

	PassConstants _mainPassCB;

	void OnResize() override;
	void Update() override;
	void Draw() override;

	void UpdateObjectCBs();
	void UpdateMaterialBuffer();
	void UpdateMainPassCB();

	void LoadRenderData();
	void InitializeRootSignature();
	void BuildDescriptorHeaps();
	void BuildInputLayout();
	void BuildShapeGeometry();
	void AddPSOs();
	void BuildFrameResources();
	
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<Mesh*>& ritems);

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();
	
};
