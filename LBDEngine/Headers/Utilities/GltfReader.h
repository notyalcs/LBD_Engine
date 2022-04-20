#pragma once

#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/GLTFResourceReader.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/Deserialize.h>
#include <GLTFSDK/MeshPrimitiveUtils.h>

#include "Utilities.h"

#include <filesystem>

#include <fstream>
#include <sstream>
#include <iostream>

#include <cassert>
#include <cstdlib>

#include <cstdint>
#include <DirectXMath.h>
#include <vector>
#include "Utilities.h"
#include "GeometryGenerator.h"

using namespace Microsoft::glTF;

//Taken directly from the example
// The glTF SDK is decoupled from all file I/O by the IStreamReader (and IStreamWriter)
// interface(s) and the C++ stream-based I/O library. This allows the glTF SDK to be used in
// sandboxed environments, such as WebAssembly modules and UWP apps, where any file I/O code
// must be platform or use-case specific.
class StreamReader : public IStreamReader
{
public:
    StreamReader(std::filesystem::path pathBase) : m_pathBase(std::move(pathBase))
    {
        assert(m_pathBase.has_root_path());
    }

    // Resolves the relative URIs of any external resources declared in the glTF manifest
    std::shared_ptr<std::istream> GetInputStream(const std::string& filename) const override;

    private:
        std::filesystem::path m_pathBase;
};

class GltfReader
{
	public:
        static GeometryObject ReadStaticMesh(std::string pathName);
};
