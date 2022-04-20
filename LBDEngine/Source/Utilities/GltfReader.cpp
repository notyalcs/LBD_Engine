/*#include "../../Headers/Utilities/GltfReader.h"


//much of this is ripped directly from the GLTF-SDK demo because there's no documentation that I found.
GeometryObject GltfReader::ReadStaticMesh(std::string pathName) {

    
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

    auto streamReader = std::make_unique<IStreamReader>(path.parent_path());

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

    
}

*/