#pragma once
#include "../Utilities/Utilities.h"
#include "Application.h"
#include "../Utilities/FrameResource.h"

#include "../Utilities/GeometryGenerator.h"
#include "../Utilities/GameObject.h"
#include "../Utilities/WavefrontReader.h"
#include "../Behaviours/Mesh.h"
#include "../Utilities/GltfReader.h"
#include "LBDGame.h"
#include "GameState.h"
#include "../Utilities/GameCameras.h"
#include <thread>

namespace LBD {
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

		struct ThreadObject
		{
			char* buf;
			SOCKET s;
			int slen;
			sockaddr_in si_other;
		};

	private:
		float _deltaAccumulator{ 0.0f };
		std::vector<std::unique_ptr<FrameResource>> _frameResources;
		FrameResource* _currentFrameResource = nullptr;
		int _currentFrameResourceIndex = 0;

		UINT _CbvSrvDescriptorSize = 0;

		ComPtr<ID3D12RootSignature> _rootSignature = nullptr;
		ComPtr<ID3D12DescriptorHeap> _SrvDescriptorHeap = nullptr;
		std::vector<D3D12_INPUT_ELEMENT_DESC> _inputLayout;

		PassConstants _mainPassCB;

		void OnExit() override;
		void OnResize() override;
		void Update() override;
		void Draw() override;

		void UpdateObjectCBs();
		void UpdateMaterialBuffer();
		void UpdateMainPassCB();
		void HandleFixedUpdate();

		void LoadRenderData();
		void InitializeRootSignature();
		void BuildDescriptorHeaps();
		void BuildInputLayout();
		void BuildShapeGeometry();
		void AddPSOs();
		void BuildFrameResources();

		void InitToServer(int& playerNum);
		static void GetFromServer(char* buf, SOCKET s, int slen, sockaddr_in si_other);

		void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<Mesh*>& ritems);
		std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

		static std::string _playerPos;

		static int _playerNum;
		std::thread _inputLoop;
		static std::vector<GameObject*> _players;
		static bool _isExiting;
	};
}
