#pragma once

constexpr auto U_R_DUM = 666; // Error exit code.
constexpr int INITIAL_WINDOWED_WIDTH{ 852 };
constexpr int INITIAL_WINDOWED_HEIGHT{ 480 };
constexpr int REFRESH_RATE{ 60 };
constexpr LPCWSTR WINDOW_CLASS_NAME{ TEXT("MainWnd") };
constexpr LPCWSTR MAIN_WINDOW_NAME{ TEXT("My Game") };
constexpr D3D_FEATURE_LEVEL CURRENT_FEATURE_LEVEL{ D3D_FEATURE_LEVEL_12_1 };
constexpr int SWAP_CHAIN_BUFFER_COUNT{ 2 };
constexpr D3D_DRIVER_TYPE DRIVER_TYPE{ D3D_DRIVER_TYPE_HARDWARE };
constexpr DXGI_FORMAT BACK_BUFFER_FORMAT{ DXGI_FORMAT_R8G8B8A8_UNORM };
constexpr DXGI_FORMAT DEPTH_STENCIL_FORMAT{ DXGI_FORMAT_D24_UNORM_S8_UINT };
constexpr int MAX_LIGHTS{ 16 };
constexpr int NUMBER_OF_FRAME_RESOURCES{ 3 };
constexpr int MINIMUM_WINDOW_WIDTH{ 400 };
constexpr int MINIMUM_WINDOW_HEIGHT{ 300 };
constexpr float HORIZONTAL_MOVEMENT_SPEED{ 10.0f };
constexpr float VERTICAL_MOVEMENT_SPEED{ 10.0f };
constexpr DirectX::XMFLOAT3 INITIAL_CAMERA_POSITION{ 0.0f, 3.0f, -20.0f };
constexpr float GRAVITY{ -9.8f };
constexpr float GRAVITY_ADJUST{ 0.0005f };
constexpr unsigned int MAXIMUM_SUBDIVISIONS = 5u;
constexpr bool WIREFRAME_STATE = false;
constexpr float MAXIMUM_PLAYER_VELOCITY = 0.005f;
constexpr bool GOD_MODE{ false };
