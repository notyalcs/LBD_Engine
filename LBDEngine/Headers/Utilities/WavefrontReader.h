#pragma once

#include <cstdint>
#include <DirectXMath.h>
#include <vector>
#include "Utilities.h"
#include "GeometryGenerator.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

/// <summary>
/// For importing graphics files.
/// </summary>
class WavefrontReader
{
public:
	static GeometryObject ReadFile(std::string path);
};