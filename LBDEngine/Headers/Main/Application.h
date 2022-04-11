#pragma once
#include "../Utilities/Utilities.h"
#include "../Utilities/GameTime.h"

//Initializes DirectX, does not need to be modified or worried about

class Application
{
protected:
    Application(HINSTANCE hInstance);
    Application(const Application& rhs) = delete;
    Application& operator=(const Application& rhs) = delete;
    virtual ~Application() { if (_device != nullptr) FlushCommandQueue(); }

public:
    static Application* GetApplicationInstance() { return _applicationInstance; }

    int Run();

    virtual bool Initialize();
    virtual LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HINSTANCE GetInstanceHandle() const { return _hInstance; }
    HWND GetMainWindow() const { return _hWnd; }
    float GetAspectRatio() const { return static_cast<float>(_screenWidth) / _screenHeight; }

protected:
    static Application* _applicationInstance;

    HINSTANCE _hInstance{ nullptr };
    HWND _hWnd{ nullptr };
    bool _isPaused{ false };
    bool _isMinimized{ false };
    bool _isMaximized{ false };
    bool _isResizing{ false };
    bool _isFullscreen{ false };

    ComPtr<IDXGIFactory4> _factory;
    ComPtr<IDXGISwapChain> _swapChain;
    ComPtr<ID3D12Device> _device;
    ComPtr<ID3D12Fence> _fence;
    UINT64 _currentFence = 0;

    ComPtr<ID3D12CommandQueue> _commandQueue;
    ComPtr<ID3D12CommandAllocator> _commandAllocator;
    ComPtr<ID3D12GraphicsCommandList> _graphicsCommandList;

    int _currentBackBuffer{ 0 };
    ComPtr<ID3D12Resource> _swapChainBuffer[SWAP_CHAIN_BUFFER_COUNT];
    ComPtr<ID3D12Resource> _depthStencilBuffer;

    ComPtr<ID3D12DescriptorHeap> _rtvHeap;
    ComPtr<ID3D12DescriptorHeap> _dsvHeap;

    D3D12_VIEWPORT _viewport{};
    D3D12_RECT _scissorRect{};

    UINT _rtvDescriptorSize{ 0 };
    UINT _dsvDescriptorSize{ 0 };
    UINT _cbvSrvUavDescriptorSize{ 0 };

    int _screenWidth{ INITIAL_WINDOWED_WIDTH };
    int _screenHeight{ INITIAL_WINDOWED_HEIGHT };

    virtual void Update() = 0;
    virtual void Draw() = 0;
    
    virtual void OnResize();
    virtual void OnExit() {}

    void Pause();
    void Unpause();

    bool InitializeMainWindow();
    bool InitializeDirect3D();
    void CreateCommandObjects();
    void CreateSwapChain();
    void CreateRtvAndDsvDescriptorHeaps();

    void FlushCommandQueue();

    ID3D12Resource* GetCurrentBackBuffer() const { return _swapChainBuffer[_currentBackBuffer].Get(); }
    D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferView() const { return CD3DX12_CPU_DESCRIPTOR_HANDLE{ _rtvHeap->GetCPUDescriptorHandleForHeapStart(), _currentBackBuffer, _rtvDescriptorSize }; }
    D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const { return _dsvHeap->GetCPUDescriptorHandleForHeapStart(); }

private:
    int _currentFrames{ 0 };
    float _currentFrameTime{ 0.0f };

    void UpdateTitleBar();

};
