#include "../../Headers/Behaviours/Mesh.h"

UINT Mesh::_currentObjectCBIndex{ 0 };

void Mesh::Start()
{
	ObjCBIndex = _currentObjectCBIndex++;
}
