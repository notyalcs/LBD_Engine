#pragma once
#include "../Utilities/Utilities.h"
#include "Application.h"
#include "../Utilities/FrameResource.h"
#include "../Behaviours/Behaviour.h"
#include "../Behaviours/Mesh.h"
#include "../Behaviours/Player.h"
#include "../Utilities/GeometryGenerator.h"
#include "../Utilities/GameObject.h"
#include "../Utilities/WavefrontReader.h"
#include "../Utilities/GltfReader.h"
#include "../Behaviours/Collider.h"
#include "../Behaviours/Physics.h"
#include "../Behaviours/PhysicsBody.h"

//Extends application, provides features to application
//

class Game : public Application
{
public:
	Game(HINSTANCE hInstance) : Application{ hInstance } {}
	Game(const Game& rhs) = delete;
	void operator=(const Game& rhs) = delete;
	~Game() override = default;

	template<typename T>
	static std::vector<T*> GetBehavioursOfType()
	{
		std::vector<T*> behaviours;
		for (auto& gameObject : _gameObjects)
		{
			auto behaviour = dynamic_cast<T*>(gameObject->GetBehaviour<T>());
			if (behaviour)
			{
				behaviours.push_back(behaviour);
			}
		}

		return behaviours;
	}

	bool Initialize() override;

	GameObject* CreateGameObject();

private:
	//Everything that exists in the world is a game object
	static std::vector<std::unique_ptr<GameObject>> _gameObjects;

	std::vector<GameObject*> _gameObjectsToStart;

	std::vector<std::unique_ptr<FrameResource>> _frameResources;
	FrameResource* _currentFrameResource = nullptr;
	int _currentFrameResourceIndex = 0;

	UINT _CbvSrvDescriptorSize = 0;

	ComPtr<ID3D12RootSignature> _rootSignature = nullptr;
	ComPtr<ID3D12DescriptorHeap> _SrvDescriptorHeap = nullptr;
	std::vector<D3D12_INPUT_ELEMENT_DESC> _inputLayout;

	PassConstants _mainPassCB;

	// Player already exists inside of the gameObjects vector, but it's helpful to have a direct pointer to it.
	GameObject* _player;

	// Provides an index for every object placed in the world.
	// Incremented after each object placed.
	int _constantBufferIndex{ 0 };

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
	void BuildRenderItems();
	void CreateMeshObject(std::string meshGeometryName, std::string submeshGeometryName, std::string materialName, XMMATRIX scale, XMMATRIX rotation, XMMATRIX translation, XMMATRIX textureTransform);
	void CreateDynamicMeshObject(std::string meshGeometryName, std::string submeshGeometryName, std::string materialName, float mass, XMMATRIX scale, XMMATRIX rotation, XMMATRIX translation, XMMATRIX textureTransform);
//	void CreateMeshObject(std::string objFile, MaterialName materialName, bool isDynamic, XMMATRIX scale, XMMATRIX rotation, XMMATRIX translation, XMMATRIX textureTransform);
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<Mesh*>& ritems);
	void CreatePlayer();

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();
	
};
