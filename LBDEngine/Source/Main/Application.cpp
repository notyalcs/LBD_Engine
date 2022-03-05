#include "../../Headers/Main/Application.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward messages to our own handler.
	return Application::GetApplicationInstance()->MessageHandler(hWnd, msg, wParam, lParam);
}

Application* Application::_applicationInstance{ nullptr };

Application::Application(HINSTANCE hInstance) : _hInstance{ hInstance }
{
	// Only one Application can be constructed.
	assert(_applicationInstance == nullptr);
	_applicationInstance = this;
}

int Application::Run()
{
	MSG msg{};

	GameTime::Initialize();

	while (msg.message != WM_QUIT)
	{
		// Process any windows messages.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Else, process game updates.
		else if (!_isPaused)
		{
			GameTime::Update();

			UpdateTitleBar();
			Update();
			Draw();
		}
	}

	return static_cast<int>(msg.wParam);
}

bool Application::Initialize()
{
	if (!InitializeMainWindow()) return false;
	if (!InitializeDirect3D()) return false;

	// Prepare the window by resizing once.
	OnResize();

	return true;
}

LRESULT Application::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	// WM_ACTIVATE is sent when the window is activated or deactivated.
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			Pause();
		}
		else
		{
			Unpause();
		}

		return 0;

	// WM_SIZE is sent when the user resizes the window.
	case WM_SIZE:
		_screenWidth = LOWORD(lParam);
		_screenHeight = HIWORD(lParam);

		if (_device)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				_isMinimized = true;
				_isMaximized = false;
				Pause();
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				_isMinimized = false;
				_isMaximized = true;
				Unpause();
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				// If restoring from minimized state.
				if (_isMinimized)
				{
					_isMinimized = false;
					Unpause();
					OnResize();
				}
				// If restoring from maximized state.
				else if (_isMaximized)
				{
					_isMaximized = false;
					Unpause();
					OnResize();
				}
				// Do nothing if resizing.
				// A lot of messages will be sent when the resize bars are being dragged so only reset the buffers after resizing is completed.
				// Otherwise, another form of resize is happening.
				else if (!_isResizing)
				{
					OnResize();
				}
			}
		}
		return 0;

	// When the user starts or finishes resizing the screen.
	case WM_ENTERSIZEMOVE:
		_isResizing = true;
		Pause();
		
		return 0;

	case WM_EXITSIZEMOVE:
		_isResizing = false;
		Unpause();
		OnResize();

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;

	// Prevents the window from being smaller than a certain size.
	case WM_GETMINMAXINFO:
	{
		auto minMaxInfo{ reinterpret_cast<MINMAXINFO*>(lParam) };
		minMaxInfo->ptMinTrackSize.x = MINIMUM_WINDOW_WIDTH;
		minMaxInfo->ptMinTrackSize.y = MINIMUM_WINDOW_HEIGHT;
	}

		return 0;

	// Update input.
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		SetCapture(hWnd);
		
		return 0;

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		ReleaseCapture();
		
		return 0;

	// Quit if Esc is pressed.
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}

		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Application::OnResize()
{
	// Finish before changing any resources.
	FlushCommandQueue();

	Utilities::ThrowIfFailed(_graphicsCommandList->Reset(_commandAllocator.Get(), nullptr));

	// Release resources before setting.
	for (int i{ 0 }; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
	{
		_swapChainBuffer[i].Reset();
	}

	_depthStencilBuffer.Reset();

	// Resize the swap chain.
	Utilities::ThrowIfFailed(_swapChain->ResizeBuffers(SWAP_CHAIN_BUFFER_COUNT, _screenWidth, _screenHeight, BACK_BUFFER_FORMAT, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	_currentBackBuffer = 0;

	// The Render Target View (RTV) heap stores views to each swap chain buffer.
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle{ _rtvHeap->GetCPUDescriptorHandleForHeapStart() };
	for (int i{ 0 }; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
	{
		Utilities::ThrowIfFailed(_swapChain->GetBuffer(i, IID_PPV_ARGS(&_swapChainBuffer[i])));
		_device->CreateRenderTargetView(_swapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, _rtvDescriptorSize);
	}

	// Create the depth stencil buffer.
	D3D12_RESOURCE_DESC depthStencilDesc{};
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = _screenWidth;
	depthStencilDesc.Height = _screenHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	// There needs to be two views for the same resource, SRV and DSV, so make it typeless.
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear{};
	optClear.Format = DEPTH_STENCIL_FORMAT;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;
	auto defaultHeapProperties{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT) };
	Utilities::ThrowIfFailed(_device->CreateCommittedResource(&defaultHeapProperties, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_COMMON, &optClear, IID_PPV_ARGS(_depthStencilBuffer.GetAddressOf())));

	// Create depth stencil view.
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = DEPTH_STENCIL_FORMAT;
	dsvDesc.Texture2D.MipSlice = 0;
	_device->CreateDepthStencilView(_depthStencilBuffer.Get(), &dsvDesc, GetDepthStencilView());
	auto resourceBarrierTransition{ CD3DX12_RESOURCE_BARRIER::Transition(_depthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE) };
	_graphicsCommandList->ResourceBarrier(1, &resourceBarrierTransition);

	// Execute the resize commands.
	Utilities::ThrowIfFailed(_graphicsCommandList->Close());
	ID3D12CommandList* commandLists[]{ _graphicsCommandList.Get() };
	_commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

	// Finish executing the commands.
	FlushCommandQueue();

	// Update the viewport to fit the new screen dimensions.
	_viewport.TopLeftX = 0;
	_viewport.TopLeftY = 0;
	_viewport.Width = static_cast<float>(_screenWidth);
	_viewport.Height = static_cast<float>(_screenHeight);
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;

	_scissorRect = { 0, 0, _screenWidth, _screenHeight };
}

void Application::Pause()
{
	_isPaused = true;
}

void Application::Unpause()
{
	_isPaused = false;
	GameTime::Update();
}

bool Application::InitializeMainWindow()
{
	WNDCLASS wndClass{};
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = NULL;
	wndClass.cbWndExtra = NULL;
	wndClass.hInstance = _hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = WINDOW_CLASS_NAME;

	if (!RegisterClass(&wndClass)) return false;

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT windowRect = { 0, 0, _screenWidth, _screenHeight };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);
	int width = windowRect.right - windowRect.left;
	int height = windowRect.bottom - windowRect.top;

	_hWnd = CreateWindow(WINDOW_CLASS_NAME, MAIN_WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, _hInstance, NULL);
	if (!_hWnd) return false;

	ShowWindow(_hWnd, SW_SHOW);
	UpdateWindow(_hWnd);

	return true;
}

bool Application::InitializeDirect3D()
{
	Utilities::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&_factory)));
	auto hResult = D3D12CreateDevice(nullptr, CURRENT_FEATURE_LEVEL, IID_PPV_ARGS(&_device));

	if (FAILED(hResult))
	{
		// Fall back to the WARP adapter.
		ComPtr<IDXGIAdapter> pWarpAdapter;
		Utilities::ThrowIfFailed(_factory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));
		Utilities::ThrowIfFailed(D3D12CreateDevice(pWarpAdapter.Get(), CURRENT_FEATURE_LEVEL, IID_PPV_ARGS(&_device)));
	}

	Utilities::ThrowIfFailed(_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));

	_rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	_dsvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	_cbvSrvUavDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CreateCommandObjects();
	CreateSwapChain();
	CreateRtvAndDsvDescriptorHeaps();

	return true;
}

void Application::CreateCommandObjects()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc{};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	
	Utilities::ThrowIfFailed(_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_commandQueue)));
	Utilities::ThrowIfFailed(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(_commandAllocator.GetAddressOf())));
	Utilities::ThrowIfFailed(_device->CreateCommandList(NULL, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator.Get(), nullptr, IID_PPV_ARGS(_graphicsCommandList.GetAddressOf())));

	// Close the command list because we will reset it before use, and it must be closed before resetting.
	_graphicsCommandList->Close();
}

void Application::CreateSwapChain()
{
	_swapChain.Reset();

	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferDesc.Width = _screenWidth;
	swapChainDesc.BufferDesc.Height = _screenHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = REFRESH_RATE;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = BACK_BUFFER_FORMAT;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
	swapChainDesc.OutputWindow = _hWnd;
	swapChainDesc.Windowed = !_isFullscreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	Utilities::ThrowIfFailed(_factory->CreateSwapChain(_commandQueue.Get(), &swapChainDesc, _swapChain.GetAddressOf()));
}

void Application::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	Utilities::ThrowIfFailed(_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(_rtvHeap.GetAddressOf())));

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	Utilities::ThrowIfFailed(_device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(_dsvHeap.GetAddressOf())));
}

void Application::FlushCommandQueue()
{
	++_currentFence;

	// Set a new fence point in the command queue.
	Utilities::ThrowIfFailed(_commandQueue->Signal(_fence.Get(), _currentFence));

	// Wait until the GPU has completed commands up to this fence point.
	if (_fence->GetCompletedValue() < _currentFence)
	{
		auto eventHandle = CreateEventEx(nullptr, NULL, false, EVENT_ALL_ACCESS);

		Utilities::ThrowIfFailed(_fence->SetEventOnCompletion(_currentFence, eventHandle));

		// Wait for event.
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

void Application::UpdateTitleBar()
{
	++_currentFrames;
	_currentFrameTime += GameTime::GetDeltaTime();

	if (_currentFrameTime >= 1.0f)
	{
		auto windowText = std::wstring{MAIN_WINDOW_NAME} + TEXT(" FPS: ") + std::to_wstring(_currentFrames);

		SetWindowText(_hWnd, windowText.c_str());

		// Reset frame data.
		_currentFrames = 0;
		_currentFrameTime -= 1.0f;
	}
}
