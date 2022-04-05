#include "../../Headers/Main/Game.h"

std::vector<std::unique_ptr<GameObject>> Game::_gameObjects;

bool Game::Initialize()
{
	if (!Application::Initialize()) return false;

	Utilities::ThrowIfFailed(_graphicsCommandList->Reset(_commandAllocator.Get(), nullptr));

	_CbvSrvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	LoadRenderData();
	InitializeRootSignature();
	BuildDescriptorHeaps();
	BuildInputLayout();
	BuildShapeGeometry();
	BuildRenderItems();
	CreatePlayer();
	CreateEnemy();
	BuildFrameResources();
	AddPSOs();

	// Execute the initialization commands.
	Utilities::ThrowIfFailed(_graphicsCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { _graphicsCommandList.Get() };
	_commandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	FlushCommandQueue();

	GameCameras::GetMainCamera()->SetPosition(INITIAL_CAMERA_POSITION);

	return true;
}

void Game::OnResize()
{
	Application::OnResize();

	GameCameras::GetMainCamera()->SetLens(0.25f * XM_PI, GetAspectRatio(), 1.0f, 1000.0f);
}

void Game::Update()
{
	auto toStartCopy{ _gameObjectsToStart };
	_gameObjectsToStart.clear();
	
	// Starting is done using a copy because GameObjects could be created in another
	// GameObject's Start function, and iterating shouldn't be done on an updating vector.
	for (auto& gameObject : toStartCopy)
	{
		gameObject->Start();
	}

	for (auto& gameObject : _gameObjects)
	{
		gameObject->Update();
	}
	GameCameras::GetMainCamera()->UpdateViewMatrix();

	// Cycle through the circular frame resource array.
	_currentFrameResourceIndex = (_currentFrameResourceIndex + 1) % NUMBER_OF_FRAME_RESOURCES;
	_currentFrameResource = _frameResources.at(_currentFrameResourceIndex).get();

	// If the GPU is not finished with the current frame resource, wait.
	if (_currentFrameResource->Fence != 0 && _fence->GetCompletedValue() < _currentFrameResource->Fence)
	{
		HANDLE hEvent = CreateEventEx(nullptr, NULL, false, EVENT_ALL_ACCESS);
		Utilities::ThrowIfFailed(_fence->SetEventOnCompletion(_currentFrameResource->Fence, hEvent));
		WaitForSingleObject(hEvent, INFINITE);
		CloseHandle(hEvent);
	}

	UpdateObjectCBs();
	UpdateMaterialBuffer();
	UpdateMainPassCB();
	UpdateAI();
}

void Game::Draw()
{
	auto& cmdListAlloc = _currentFrameResource->CmdListAlloc;

	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	Utilities::ThrowIfFailed(cmdListAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	Utilities::ThrowIfFailed(_graphicsCommandList->Reset(cmdListAlloc.Get(), Render::GetPSOs().at("opaque").Get()));

	_graphicsCommandList->RSSetViewports(1, &_viewport);
	_graphicsCommandList->RSSetScissorRects(1, &_scissorRect);

	auto resourceBarrier{ CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET) };
	// Indicate a state transition on the resource usage.
	_graphicsCommandList->ResourceBarrier(1, &resourceBarrier);

	// Clear the back buffer and depth buffer.
	_graphicsCommandList->ClearRenderTargetView(GetCurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	_graphicsCommandList->ClearDepthStencilView(GetDepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	auto currentBackBufferView{ GetCurrentBackBufferView() };
	auto depthStencilView{ GetDepthStencilView() };
	_graphicsCommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);

	ID3D12DescriptorHeap* descriptorHeaps[] = { _SrvDescriptorHeap.Get() };
	_graphicsCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	_graphicsCommandList->SetGraphicsRootSignature(_rootSignature.Get());

	auto passCB = _currentFrameResource->PassCB->Resource();
	_graphicsCommandList->SetGraphicsRootConstantBufferView(1, passCB->GetGPUVirtualAddress());

	// Bind all the materials used in this scene.  For structured buffers, we can bypass the heap and 
	// set as a root descriptor.
	auto matBuffer = _currentFrameResource->MaterialBuffer->Resource();
	_graphicsCommandList->SetGraphicsRootShaderResourceView(2, matBuffer->GetGPUVirtualAddress());

	// Bind all the textures used in this scene.  Observe
	// that we only have to specify the first descriptor in the table.  
	// The root signature knows how many descriptors are expected in the table.
	_graphicsCommandList->SetGraphicsRootDescriptorTable(3, _SrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

	DrawRenderItems(_graphicsCommandList.Get(), GetBehavioursOfType<Mesh>());

	// Indicate a state transition on the resource usage.
	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	_graphicsCommandList->ResourceBarrier(1, &resourceBarrier);

	// Done recording commands.
	Utilities::ThrowIfFailed(_graphicsCommandList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { _graphicsCommandList.Get() };
	_commandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Swap the back and front buffers
	Utilities::ThrowIfFailed(_swapChain->Present(0, 0));
	_currentBackBuffer = (_currentBackBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;

	// Advance the fence value to mark commands up to this fence point.
	_currentFrameResource->Fence = ++_currentFence;

	// Add an instruction to the command queue to set a new fence point. 
	// Because we are on the GPU timeline, the new fence point won't be 
	// set until the GPU finishes processing all the commands prior to this Signal().
	_commandQueue->Signal(_fence.Get(), _currentFence);
}

GameObject* Game::CreateGameObject()
{
	_gameObjects.push_back(std::make_unique<GameObject>());

	auto gameObject{ _gameObjects.back().get() };
	_gameObjectsToStart.push_back(gameObject);
	return gameObject;
}

void Game::CreatePlayer()
{
	_player = CreateDynamicMeshObject("shape", "sphere", "stone", 3.0f, XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 2.0f, -10.0f), XMLoadFloat4x4(&MathHelper::CreateIdentity4x4()));
	_player->AddBehaviour<Controller>();
	_player->AddBehaviour<Player>();
	_player->GetBehaviour<Physics>()->SetElasticity(0.9f);
}


std::vector<Node> pathToGoal;
Node* nextNode;
GameObject* _enemy;
void Game::CreateEnemy()
{
	_enemy = CreateDynamicMeshObject("shape", "sphere", "stone", 3.0f, XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.5f, 10.0f), XMLoadFloat4x4(&MathHelper::CreateIdentity4x4()));
	GameObject* _flag = CreateDynamicMeshObject("shape", "sphere", "stone", 3.0f, XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.5f, -12.5f), XMLoadFloat4x4(&MathHelper::CreateIdentity4x4()));

	MyGrid myGrid{};
	auto behaviours = Game::GetBehavioursOfType<Collider>();
	for (auto behaviour : behaviours) {
		if (behaviour->GetGameObject() == _enemy || behaviour->GetGameObject() == _flag) continue;
		XMFLOAT3 location = behaviour->GetBoundingBox().Center;
		myGrid.locations.push_back(Vector3{ location.x, location.y, location.z });
		XMFLOAT3 size = behaviour->GetBoundingBox().Extents;
		myGrid.sizes.push_back(Vector3{ size.x, size.y, size.z });
	}

	myGrid.Start();

	Pathfinding path{};
	path.grid = myGrid;
	path.FindPath(Vector3{ _enemy->GetTranslation()._41,_enemy->GetTranslation()._42,_enemy->GetTranslation()._43 },
		Vector3{ _flag->GetTranslation()._41, _flag->GetTranslation()._42, _flag->GetTranslation()._43 });

	pathToGoal = path.pathToGoal;
	nextNode = &pathToGoal[0];

	/*std::vector<GameObject*> enemies;
	for (Node &obj : pathToGoal) {
		enemies.push_back(CreateDynamicMeshObject("shape", "sphere", "stone", 3.0f, XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(obj.worldPosition.x, obj.worldPosition.y, obj.worldPosition.z), XMLoadFloat4x4(&MathHelper::CreateIdentity4x4())));
	}*/

}
void Game::UpdateAI() {
	float range = 1.0f;
	float directionalForce = 0.000002f;
	float upwardForce = 0.0000148f;


	if (Vector3(_enemy->GetTranslation()._41, _enemy->GetTranslation()._42, _enemy->GetTranslation()._43)
	< Vector3(nextNode->worldPosition.x + range, nextNode->worldPosition.y + range, nextNode->worldPosition.z + range) &&
	Vector3(nextNode->worldPosition.x - range, nextNode->worldPosition.y - range, nextNode->worldPosition.z - range)
	< Vector3(_enemy->GetTranslation()._41, _enemy->GetTranslation()._42, _enemy->GetTranslation()._43)) {
		
		nextNode++;
	}
	else {
		Vector3 direction{ nextNode->worldPosition.x - _enemy->GetTranslation()._41,
			nextNode->worldPosition.y - _enemy->GetTranslation()._42 , nextNode->worldPosition.z - _enemy->GetTranslation()._43 };
		XMMATRIX position{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, nextNode->worldPosition.x, 0.0f, nextNode->worldPosition.z, 1.0f };

		_enemy->SetTranslation(position);
		_enemy->GetBehaviour<Mesh>()->SetDirtyFrames(3);
		//_enemy->GetBehaviour<PhysicsBody>()->AddForce(XMFLOAT3(direction.x * directionalForce, upwardForce, direction.z * directionalForce));
	}
}

void Game::UpdateObjectCBs()
{
	auto currObjectCB = _currentFrameResource->ObjectCB.get();
	for (auto& item : GetBehavioursOfType<Mesh>())
	{
		auto dirtyFrames{ item->GetDirtyFrames() };
		// Only update the cbuffer data if the constants have changed.  
		// This needs to be tracked per frame resource.
		if (dirtyFrames > 0)
		{
			XMMATRIX world = item->GetGameObject()->GetWorldTransform();

			XMMATRIX texTransform = XMLoadFloat4x4(&item->GetTextureTransform());

			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.TextureTransform, XMMatrixTranspose(texTransform));
			objConstants.MaterialIndex = item->Mat->MatCBIndex;

			currObjectCB->CopyData(item->ObjCBIndex, objConstants);

			// Next FrameResource need to be updated too.
			item->SetDirtyFrames(dirtyFrames - 1);
		}
	}
}

void Game::UpdateMaterialBuffer()
{
	auto currMaterialBuffer = _currentFrameResource->MaterialBuffer.get();
	for (auto& e : Render::GetMaterials())
	{
		// Only update the cbuffer data if the constants have changed.  If the cbuffer
		// data changes, it needs to be updated for each FrameResource.
		Material* mat = e.second.get();
		if (mat->DirtyFrames > 0)
		{
			XMMATRIX matTransform = XMLoadFloat4x4(&mat->MatTransform);

			MaterialData matData;
			matData.DiffuseAlbedo = mat->DiffuseAlbedo;
			matData.FresnelR0 = mat->FresnelR0;
			matData.Roughness = mat->Roughness;
			XMStoreFloat4x4(&matData.MatTransform, XMMatrixTranspose(matTransform));
			matData.DiffuseMapIndex = mat->DiffuseSrvHeapIndex;

			currMaterialBuffer->CopyData(mat->MatCBIndex, matData);

			// Next FrameResource need to be updated too.
			mat->DirtyFrames--;
		}
	}
}

void Game::UpdateMainPassCB()
{
	XMMATRIX view = GameCameras::GetMainCamera()->GetView();
	XMMATRIX projection = GameCameras::GetMainCamera()->GetProjection();

	XMMATRIX viewProjection = XMMatrixMultiply(view, projection);
	auto viewDeterminant{ XMMatrixDeterminant(view) };
	auto projectionDeterminant{ XMMatrixDeterminant(projection) };
	auto viewProjectionDeterminant{ XMMatrixDeterminant(viewProjection) };

	XMMATRIX invView = XMMatrixInverse(&viewDeterminant, view);
	XMMATRIX invProj = XMMatrixInverse(&projectionDeterminant, projection);
	XMMATRIX invViewProj = XMMatrixInverse(&viewProjectionDeterminant, viewProjection);

	XMStoreFloat4x4(&_mainPassCB.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&_mainPassCB.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&_mainPassCB.Proj, XMMatrixTranspose(projection));
	XMStoreFloat4x4(&_mainPassCB.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&_mainPassCB.ViewProj, XMMatrixTranspose(viewProjection));
	XMStoreFloat4x4(&_mainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
	_mainPassCB.EyePosW = GameCameras::GetMainCamera()->GetPosition();
	_mainPassCB.RenderTargetSize = XMFLOAT2((float)_screenWidth, (float)_screenHeight);
	_mainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / _screenWidth, 1.0f / _screenHeight);
	_mainPassCB.NearZ = 1.0f;
	_mainPassCB.FarZ = 1000.0f;
	_mainPassCB.TotalTime = GameTime::GetTotalTime();
	_mainPassCB.DeltaTime = GameTime::GetDeltaTime();
	_mainPassCB.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
	_mainPassCB.Lights[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
	_mainPassCB.Lights[0].Strength = { 0.8f, 0.8f, 0.8f };
	_mainPassCB.Lights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
	_mainPassCB.Lights[1].Strength = { 0.4f, 0.4f, 0.4f };
	_mainPassCB.Lights[2].Direction = { 0.0f, -0.707f, -0.707f };
	_mainPassCB.Lights[2].Strength = { 0.2f, 0.2f, 0.2f };

	auto currPassCB = _currentFrameResource->PassCB.get();
	currPassCB->CopyData(0, _mainPassCB);
}

void Game::LoadRenderData()
{
	// Add textures.
	Render::AddTexture(TEXT("./Textures/grasscube1024.dds"), "tile", _device.Get(), _graphicsCommandList.Get());
	Render::AddTexture(TEXT("./Textures/bricks.dds"), "brick", _device.Get(), _graphicsCommandList.Get());
	Render::AddTexture(TEXT("./Textures/stone.dds"), "stone", _device.Get(), _graphicsCommandList.Get());
	Render::AddTexture(TEXT("./Textures/chair_shitty.dds"), "chair", _device.Get(), _graphicsCommandList.Get());
	Render::AddTexture(TEXT("./Textures/WoodCrate01.dds"), "crate", _device.Get(), _graphicsCommandList.Get());

	// Add materials.
	// Right now, the order of the materials must be the same as the order of the textures.
	// This can be refactored if necessary.
	Render::AddMaterial("tile", { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.02f, 0.02f, 0.02f }, 0.3f);
	Render::AddMaterial("chair", { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.05f, 0.05f, 0.05f }, 0.85f);
	Render::AddMaterial("stone", { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.05f, 0.05f, 0.05f }, 0.3f);
	Render::AddMaterial("brick", { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.02f, 0.02f, 0.02f }, 0.1f);
	Render::AddMaterial("crate", { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.05f, 0.05f, 0.05f }, 0.2f);

	// Add shaders.
	Render::AddShader(TEXT("./Shaders/Default.hlsl"), "VS", "standard_vs", "vs_5_1");
	Render::AddShader(TEXT("./Shaders/Default.hlsl"), "PS", "opaque_ps", "ps_5_1");
}

void Game::InitializeRootSignature()
{
	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 0, 0);

	CD3DX12_ROOT_PARAMETER rootParameter[4];

	// Order most-to-least-frequently-used for better performance.
	rootParameter[0].InitAsConstantBufferView(0);
	rootParameter[1].InitAsConstantBufferView(1);
	rootParameter[2].InitAsShaderResourceView(0, 1);
	rootParameter[3].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);

	auto staticSamplers = GetStaticSamplers();

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc{ 4, rootParameter, static_cast<UINT>(staticSamplers.size()), staticSamplers.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT };

	ComPtr<ID3DBlob> serializedRootSignature;
	ComPtr<ID3DBlob> error;
	HRESULT hResult = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSignature.GetAddressOf(), error.GetAddressOf());
	if (error != nullptr) OutputDebugStringA(static_cast<char*>(error->GetBufferPointer()));
	Utilities::ThrowIfFailed(hResult);

	Utilities::ThrowIfFailed(_device->CreateRootSignature(0, serializedRootSignature->GetBufferPointer(), serializedRootSignature->GetBufferSize(), IID_PPV_ARGS(_rootSignature.GetAddressOf())));
}

void Game::BuildDescriptorHeaps()
{
	// Create the SRV heap.
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 4;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	Utilities::ThrowIfFailed(_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_SrvDescriptorHeap)));

	// Initialize the heap with descriptors.
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor{ _SrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart() };
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;

	for (auto& texture : Render::GetTextures()) {
		auto& resource = texture.second->Resource;
		srvDesc.Format = resource->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = resource->GetDesc().MipLevels;
		_device->CreateShaderResourceView(resource.Get(), &srvDesc, hDescriptor);
		hDescriptor.Offset(1, _CbvSrvDescriptorSize);
	}
}

void Game::BuildInputLayout()
{
	_inputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}

/*
* Create object shapes here.
*/
void Game::BuildShapeGeometry()
{
	GeometryObject data[] =
	{
		GeometryGenerator::CreateBox(1.0f, 1.0f, 1.0f, 3),
		GeometryGenerator::CreateGrid(20.0f, 30.0f, 60, 40),
		GeometryGenerator::CreateSphere(0.5f, 20, 20),
		GeometryGenerator::CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20),
		WavefrontReader::ReadFile("./Models/rudimentary_armchair_tris.obj")
	};
	std::string names[] =
	{
		"box", "grid", "sphere", "cylinder", "custom"
	};

	auto geometry = std::make_unique<MeshGeometry>();

	UINT vertexOffset{ 0 };
	UINT indexOffset{ 0 };
	UINT nameIndex{ 0 };
	std::vector<Vertex> vertices;
	std::vector<USHORT> indices;
	for (auto& d : data)
	{
		UINT indicesSize = static_cast<UINT>(d.Indices.size());
		
		SubmeshGeometry submesh;
		submesh.IndexCount = indicesSize;
		submesh.StartIndexLocation = indexOffset;
		submesh.BaseVertexLocation = vertexOffset;
		BoundingBox::CreateFromPoints(submesh.Bounds, d.Vertices.size(), &d.Vertices.at(0).Position, sizeof(GeometryVertex));

		for (auto& vertex : d.Vertices)
		{
			Vertex v;
			v.Pos = vertex.Position;
			v.Normal = vertex.Normal;
			v.TexC = vertex.TexC;
			vertices.push_back(v);
		}

		indices.insert(indices.end(), std::begin(d.Indices), std::end(d.Indices));

		geometry->Submeshes.insert(std::make_pair(names[nameIndex++], submesh));

		vertexOffset += static_cast<UINT>(d.Vertices.size());
		indexOffset += indicesSize;
	}

	const UINT vbByteSize = static_cast<UINT>(vertices.size()) * sizeof(Vertex);
	const UINT ibByteSize = static_cast<UINT>(indices.size()) * sizeof(USHORT);

	Utilities::ThrowIfFailed(D3DCreateBlob(vbByteSize, &geometry->VertexBufferCPU));
	CopyMemory(geometry->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	Utilities::ThrowIfFailed(D3DCreateBlob(ibByteSize, &geometry->IndexBufferCPU));
	CopyMemory(geometry->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geometry->VertexBufferGPU = Utilities::CreateDefaultBuffer(_device.Get(), _graphicsCommandList.Get(), vertices.data(), vbByteSize, geometry->VertexBufferUploader);
	geometry->IndexBufferGPU = Utilities::CreateDefaultBuffer(_device.Get(), _graphicsCommandList.Get(), indices.data(), ibByteSize, geometry->IndexBufferUploader);

	geometry->VertexByteStride = sizeof(Vertex);
	geometry->VertexBufferByteSize = vbByteSize;
	geometry->IndexFormat = DXGI_FORMAT_R16_UINT;
	geometry->IndexBufferByteSize = ibByteSize;

	Render::AddGeometry("shape", std::move(geometry));
}

void Game::AddPSOs()
{
	Render::AddPSO("opaque", _device.Get(), _inputLayout, _rootSignature.Get());
}

void Game::BuildFrameResources()
{
	for (int i = 0; i < NUMBER_OF_FRAME_RESOURCES; ++i)
	{
		_frameResources.push_back(std::make_unique<FrameResource>(_device.Get(), 1, static_cast<UINT>(GetBehavioursOfType<Mesh>().size()), static_cast<UINT>(Render::GetMaterials().size())));
	}
}

/*
* Configure the placement of objects here.
*/
void Game::BuildRenderItems()
{

	CreateDynamicMeshObject("shape", "box", "stone", 30.0f, XMMatrixScaling(1.0f, 3.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 24.0f, 6.0f), XMMatrixScaling(1.0f, 3.0f, 1.0f));

	CreateDynamicMeshObject("shape", "box", "crate", 10.0f, XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 1.0f, 0.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

	CreateDynamicMeshObject("shape", "custom", "chair", 5.0f, XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 1.0f, -5.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	

	CreateDynamicMeshObject("shape", "box", "crate", 1.0f, XMMatrixScaling(2.0f, 2.0f, 2.0f), XMMatrixRotationRollPitchYaw(1.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 6.0f, 0.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, 0.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	XMMATRIX brickTexTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 5; ++i)
	{
		CreateMeshObject("shape", "cylinder", "brick", XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-5.0f, 1.5f, -10.0f + i * 5.0f), brickTexTransform);
		CreateMeshObject("shape", "cylinder", "brick", XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(5.0f, 1.5f, -10.0f + i * 5.0f), brickTexTransform);
		CreateMeshObject("shape", "sphere", "stone", XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-5.0f, 3.5f, -10.0f + i * 5.0f), XMLoadFloat4x4(&MathHelper::CreateIdentity4x4()));
		CreateDynamicMeshObject("shape", "sphere", "stone", 0.5f, XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(5.0f, 20.5f, -10.0f + i * 5.0f), XMLoadFloat4x4(&MathHelper::CreateIdentity4x4()));
	}
}

void Game::CreateMeshObject(std::string meshGeometryName, std::string submeshGeometryName, std::string materialName, XMMATRIX scale, XMMATRIX rotation, XMMATRIX translation, XMMATRIX textureTransform)
{
	auto meshObject{ CreateGameObject() };
	auto mesh{ dynamic_cast<Mesh*>(meshObject->AddBehaviour<Mesh>()) };
	
	auto meshGeometry{ Render::GetGeometries().at(meshGeometryName).get() };
	auto& submeshGeometry{ meshGeometry->Submeshes.at(submeshGeometryName) };

	meshObject->SetScale(scale);
	meshObject->SetRotation(rotation);
	meshObject->SetTranslation(translation);
	mesh->SetTextureTransform(textureTransform);
	mesh->ObjCBIndex = _constantBufferIndex++;
	mesh->Mat = Render::GetMaterials().at(materialName).get();
	mesh->Geo = Render::GetGeometries().at(meshGeometryName).get();
	mesh->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mesh->IndexCount = submeshGeometry.IndexCount;
	mesh->StartIndexLocation = submeshGeometry.StartIndexLocation;
	mesh->BaseVertexLocation = submeshGeometry.BaseVertexLocation;

	auto collider{ dynamic_cast<Collider*>(meshObject->AddBehaviour<Collider>()) };
	collider->SetBoundingBox(submeshGeometry.Bounds);
	collider->Transform(meshObject->GetWorldTransform());
}

GameObject* Game::CreateDynamicMeshObject(std::string meshGeometryName, std::string submeshGeometryName, std::string materialName, float mass, XMMATRIX scale, XMMATRIX rotation, XMMATRIX translation, XMMATRIX textureTransform)
{
	auto meshObject{ CreateGameObject() };
	auto mesh{ dynamic_cast<Mesh*>(meshObject->AddBehaviour<Mesh>()) };

	auto meshGeometry{ Render::GetGeometries().at(meshGeometryName).get() };
	auto& submeshGeometry{ meshGeometry->Submeshes.at(submeshGeometryName) };

	meshObject->SetScale(scale);
	meshObject->SetRotation(rotation);
	meshObject->SetTranslation(translation);
	mesh->SetTextureTransform(textureTransform);
	mesh->ObjCBIndex = _constantBufferIndex++;
	mesh->Mat = Render::GetMaterials().at(materialName).get();
	mesh->Geo = Render::GetGeometries().at(meshGeometryName).get();
	mesh->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mesh->IndexCount = submeshGeometry.IndexCount;
	mesh->StartIndexLocation = submeshGeometry.StartIndexLocation;
	mesh->BaseVertexLocation = submeshGeometry.BaseVertexLocation;

	auto collider{ dynamic_cast<Collider*>(meshObject->AddBehaviour<Collider>()) };
	collider->SetBoundingBox(submeshGeometry.Bounds);
	collider->Transform(meshObject->GetWorldTransform());
	
	auto physics{ dynamic_cast<Physics*>(meshObject->AddBehaviour<Physics>()) };
	physics->SetMass(mass); // set mass to 500g, probably definitely add this into the parameter list
	auto physicsBody = dynamic_cast<PhysicsBody*>(meshObject->AddBehaviour<PhysicsBody>());
	return meshObject;
}

void Game::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<Mesh*>& ritems)
{
	UINT objCBByteSize = Utilities::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	auto objectCB = _currentFrameResource->ObjectCB->Resource();

	// For each render item...
	for (size_t i = 0; i < ritems.size(); ++i)
	{
		auto ri = ritems[i];

		auto vertexBufferView{ ri->Geo->GetVertexBufferView() };
		auto indexBufferView{ ri->Geo->GetIndexBufferView() };
		cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);
		cmdList->IASetIndexBuffer(&indexBufferView);
		cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + ri->ObjCBIndex * objCBByteSize;

		cmdList->SetGraphicsRootConstantBufferView(0, objCBAddress);
		cmdList->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
	}
}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> Game::GetStaticSamplers()
{
	// Applications usually only need a handful of samplers.  So just define them all up front
	// and keep them available as part of the root signature.  

	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };
}
