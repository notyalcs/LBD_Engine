#include "../../Headers/Utilities/WavefrontReader.h"

GeometryObject WavefrontReader::ReadFile(std::string path) {
	std::ifstream fin(path);

	if (!fin)
	{
		MessageBox(0, L"model not found", 0, 0);
		return GeometryGenerator::CreateBox(1, 1, 1, 0);
	}

	std::vector<GeometryVertex> verts{};

	std::vector<XMFLOAT3> positions{};
	std::vector<XMFLOAT2> texCoords{};
	std::vector<XMFLOAT3> normals{};

	std::vector<USHORT> indeces{};

	size_t vtIndex = 0;
	std::string currentLine;
	while (std::getline(fin, currentLine, '\n'))
	{
		std::istringstream iss(currentLine);
		std::string marker;
		iss >> marker;
		if (marker == "v")
		{
			float x;
			float y;
			float z;
			iss >> x;
			iss >> y;
			iss >> z;
			positions.emplace_back(XMFLOAT3(x, y, z));
		}
		else if (marker == "vt")
		{
			float x;
			float y;
			iss >> x;
			iss >> y;
			texCoords.emplace_back(XMFLOAT2{x, y});
		}
		else if (marker == "vn") {
			float x;
			float y;
			float z;
			iss >> x;
			iss >> y;
			iss >> z;
			normals.emplace_back(XMFLOAT3{x, y, z});
		}
		else if (marker == "f") {
			std::string tri1;
			iss >> tri1;
			std::stringstream elem1(tri1);

			std::string tri2;
			iss >> tri2;
			std::stringstream elem2(tri2);

			std::string tri3;
			iss >> tri3;
			std::stringstream elem3(tri3);

			//Waveform format is 1-indexed
			std::string v1s;
			std::getline(elem1, v1s, '/');
			int v1 = std::stoi(v1s) - 1;
			verts.emplace_back(GeometryVertex(positions.at(v1).x, positions.at(v1).y, positions.at(v1).z, 0, 0, 0, 0, 0, 0, 0, 0));
			std::getline(elem1, v1s, '/');
			verts.at(verts.size() - 1).TexC = texCoords.at(std::stoi(v1s) - 1);
			std::getline(elem1, v1s, '/');
			verts.at(verts.size() - 1).Normal = normals.at(std::stoi(v1s) - 1);
			indeces.push_back(static_cast<USHORT>(verts.size() - 1));

			std::string v2s;
			std::getline(elem2, v2s, '/');
			int v2 = std::stoi(v2s) - 1;
			verts.emplace_back(GeometryVertex(positions.at(v2).x, positions.at(v2).y, positions.at(v2).z, 0, 0, 0, 0, 0, 0, 0, 0));
			std::getline(elem2, v2s, '/');
			verts.at(verts.size() - 1).TexC = texCoords.at(std::stoi(v2s) - 1);
			std::getline(elem2, v2s, '/');
			verts.at(verts.size() - 1).Normal = normals.at(std::stoi(v2s) - 1);
			indeces.push_back(static_cast<USHORT>(verts.size() - 1));

			std::string v3s;
			std::getline(elem3, v3s, '/');
			int v3 = std::stoi(v3s) - 1;
			verts.emplace_back(GeometryVertex(positions.at(v3).x, positions.at(v3).y, positions.at(v3).z, 0, 0, 0, 0, 0, 0, 0, 0));
			std::getline(elem3, v3s, '/');
			verts.at(verts.size() - 1).TexC = texCoords.at(std::stoi(v3s) - 1);
			std::getline(elem3, v3s, '/');
			verts.at(verts.size() - 1).Normal = normals.at(std::stoi(v3s) - 1);
			indeces.push_back(static_cast<USHORT>(verts.size() - 1));
		}
	}

	GeometryObject meshData;
	meshData.Indices = indeces;
	meshData.Vertices = verts;

	return meshData;
}