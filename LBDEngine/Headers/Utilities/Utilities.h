#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <WindowsX.h>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>
#include "d3dx12.h"
#include <ddstream.h>
#include "Constants.h"
#include "MathHelper.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

//Useful functions

class Utilities
{
public:

	//Print Debug works as your console.log
	static void PrintDebug(double value) {
		OutputDebugString((std::to_wstring(value) + TEXT(" ")).c_str());
	}

	static void PrintDebugLine(double value) {
		OutputDebugString((std::to_wstring(value) + TEXT("\n")).c_str());
	}

	static void PrintDebug(std::wstring value) {
		OutputDebugString((value + TEXT(" ")).c_str());
	}

	static void PrintDebugLine(std::wstring value) {
		OutputDebugString((value + TEXT("\n")).c_str());
	}

    static void ThrowIfFailed(HRESULT hResult) {
        if (FAILED(hResult)) {
            exit(U_R_DUM);
        }
    }

	static UINT CalcConstantBufferByteSize(UINT byteSize)
	{
		// Constant buffers must be a multiple of the minimum hardware
		// allocation size (usually 256 bytes).  So round up to nearest
		// multiple of 256.  We do this by adding 255 and then masking off
		// the lower 2 bytes which store all bits < 256.
		// Example: Suppose byteSize = 300.
		// (300 + 255) & ~255
		// 555 & ~255
		// 0x022B & ~0x00ff
		// 0x022B & 0xff00
		// 0x0200
		// 512
		return (byteSize + 255) & ~255;
	}

	static ComPtr<ID3D12Resource> CreateDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* cmdList,
		const void* initData,
		UINT64 byteSize,
		Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer) {
		ComPtr<ID3D12Resource> defaultBuffer;

		auto defaultHeapProperties{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT) };
		auto defaultDescBuffer{ CD3DX12_RESOURCE_DESC::Buffer(byteSize) };

		// Create the actual default buffer resource.
		ThrowIfFailed(device->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&defaultDescBuffer,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(defaultBuffer.GetAddressOf())));

		auto uploadHeapProperties{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) };
		auto uploadDescBuffer{ CD3DX12_RESOURCE_DESC::Buffer(byteSize) };

		// In order to copy CPU memory data into our default buffer, we need to create
		// an intermediate upload heap. 
		ThrowIfFailed(device->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&uploadDescBuffer,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(uploadBuffer.GetAddressOf())));


		// Describe the data we want to copy into the default buffer.
		D3D12_SUBRESOURCE_DATA subResourceData = {};
		subResourceData.pData = initData;
		subResourceData.RowPitch = byteSize;
		subResourceData.SlicePitch = subResourceData.RowPitch;

		// Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
		// will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
		// the intermediate upload heap data will be copied to mBuffer.
		auto defaultBufferBarrier{ CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST) };
		cmdList->ResourceBarrier(1, &defaultBufferBarrier);
		UpdateSubresources<1>(cmdList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);
		defaultBufferBarrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		cmdList->ResourceBarrier(1, &defaultBufferBarrier);

		// Note: uploadBuffer has to be kept alive after the above function calls because
		// the command list has not been executed yet that performs the actual copy.
		// The caller can Release the uploadBuffer after it knows the copy has been executed.


		return defaultBuffer;
	}
};

struct Light
{
    XMFLOAT3 Strength = { 0.5f, 0.5f, 0.5f };
    float FalloffStart = 1.0f;                          // point/spot light only
    XMFLOAT3 Direction = { 0.0f, -1.0f, 0.0f };// directional/spot light only
    float FalloffEnd = 10.0f;                           // point/spot light only
    XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };  // point/spot light only
    float SpotPower = 64.0f;                            // spot light only
};
