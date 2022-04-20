#include "../../Headers/Utilities/GltfReader.h"

//this one is directly from the MS example.
std::shared_ptr<std::istream> StreamReader::GetInputStream(const std::string& filename) const {
    // In order to construct a valid stream:
    // 1. The filename argument will be encoded as UTF-8 so use filesystem::u8path to
    //    correctly construct a path instance.
    // 2. Generate an absolute path by concatenating m_pathBase with the specified filename
    //    path. The filesystem::operator/ uses the platform's preferred directory separator
    //    if appropriate.
    // 3. Always open the file stream in binary mode. The glTF SDK will handle any text
    //    encoding issues for us.
    auto streamPath = m_pathBase / std::filesystem::path(filename);
    auto stream = std::make_shared<std::ifstream>(streamPath, std::ios_base::binary);

    // Check if the stream has no errors and is ready for I/O operations
    if (!stream || !(*stream))
    {
        throw std::runtime_error("Unable to create a valid input stream for uri: " + filename);
    }

    return stream;
}


//much of this is ripped directly from the GLTF-SDK demo because there's no documentation that I found.
GeometryObject GltfReader::ReadStaticMesh(std::string pathName) {
    //this part is also taken from the example
    std::filesystem::path path = pathName;

    if (path.is_relative())
    {
        auto pathCurrent = std::filesystem::current_path();

        // Convert the relative path into an absolute path by appending the command line argument to the current path
        pathCurrent /= path;
        pathCurrent.swap(path);
    }

    if (!path.has_filename())
    {
        throw std::runtime_error("Model path has no filename");
    }

    if (!path.has_extension())
    {
        throw std::runtime_error("Model path has no filename extension");
    }

    auto streamReader = std::make_unique<StreamReader>(path.parent_path());

    std::filesystem::path pathFile = path.filename();
    std::filesystem::path pathFileExt = pathFile.extension();

    std::string manifest;

    auto MakePathExt = [](const std::string& ext)
    {
        return "." + ext;
    };

    std::unique_ptr<GLTFResourceReader> resourceReader;

    // If the file has a '.gltf' extension then create a GLTFResourceReader
    if (pathFileExt == MakePathExt(GLTF_EXTENSION))
    {
        auto gltfStream = streamReader->GetInputStream(pathFile.string()); // Pass a UTF-8 encoded filename to GetInputString
        auto gltfResourceReader = std::make_unique<GLTFResourceReader>(std::move(streamReader));

        std::stringstream manifestStream;

        // Read the contents of the glTF file into a string using a std::stringstream
        manifestStream << gltfStream->rdbuf();
        manifest = manifestStream.str();

        resourceReader = std::move(gltfResourceReader);
    }

    // If the file has a '.glb' extension then create a GLBResourceReader. This class derives
    // from GLTFResourceReader and adds support for reading manifests from a GLB container's
    // JSON chunk and resource data from the binary chunk.
    if (pathFileExt == MakePathExt(GLB_EXTENSION))
    {
        auto glbStream = streamReader->GetInputStream(pathFile.string()); // Pass a UTF-8 encoded filename to GetInputString
        auto glbResourceReader = std::make_unique<GLBResourceReader>(std::move(streamReader), std::move(glbStream));

        manifest = glbResourceReader->GetJson(); // Get the manifest from the JSON chunk

        resourceReader = std::move(glbResourceReader);
    }

    if (!resourceReader)
    {
        throw std::runtime_error("Extension of GLTF file appears to be incorrect.");
    }

    Document document;

    try
    {
        document = Deserialize(manifest);
    }
    catch (const GLTFException& ex)
    {
        std::stringstream ss;

        ss << "Microsoft::glTF::Deserialize failed: ";
        ss << ex.what();

        throw std::runtime_error(ss.str());
    }

    //Ripped-off-from-example part ends

    MeshPrimitive meshPrim = document.meshes[0].primitives[0];

    //Load position coordinates
    std::vector<float> positionSeries = Microsoft::glTF::MeshPrimitiveUtils::GetPositions(document, *resourceReader, meshPrim);

    std::vector<XMFLOAT3> positions{};

    for (int i = 0; i < positionSeries.size(); i += 3)
    {
        positions.emplace_back(XMFLOAT3(positionSeries[i], positionSeries[i + 1], positionSeries[i + 2]));
    }

    //Load normal directions
    std::vector<float> normalSeries = Microsoft::glTF::MeshPrimitiveUtils::GetNormals(document, *resourceReader, meshPrim);

    std::vector<XMFLOAT3> normals{};

    for (int i = 0; i < normalSeries.size(); i += 3)
    {
        normals.emplace_back(XMFLOAT3(normalSeries[i], normalSeries[i + 1], normalSeries[i + 2]));
    }

    //Load texture coordinates, TexCoords 0 only for now
    std::vector<float> texSeries = Microsoft::glTF::MeshPrimitiveUtils::GetTexCoords_0(document, *resourceReader, meshPrim);

    std::vector<XMFLOAT2> texCoords{};

    for (int i = 0; i < texSeries.size(); i += 2)
    {
        texCoords.emplace_back(XMFLOAT2(texSeries[i], texSeries[i + 1]));
    }

    std::vector<uint16_t> indices = Microsoft::glTF::MeshPrimitiveUtils::GetIndices16(document, *resourceReader, meshPrim);

    std::vector<GeometryVertex> verts{};
    std::vector<uint16_t> resInd{};
    for (uint32_t i = 0; i < indices.size(); i++) {
        GeometryVertex v = GeometryVertex();
        v.Position = positions[indices[i]];//things seem to work when I do this
        v.Normal = normals[indices[i]];//but the way it's done looks very suspicious
        v.TexC = texCoords[indices[i]];//I might be missing something
        verts.emplace_back(v);
        resInd.emplace_back(i);
    }

    GeometryObject result = GeometryObject();
    result.Vertices = verts;
    result.Indices = resInd;

    return result;
}
