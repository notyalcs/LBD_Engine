#include "../../Headers/Main/LBDGame.h"

void LBDGame::StartGame()
{
	CreatePlayer();
	BuildRenderItems();
}

void LBDGame::CreatePlayer()
{
	_player = CreateDynamicMeshObject("shape", "sphere", "stone", 3.0f, XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 2.0f, 0.0f), XMLoadFloat4x4(&MathHelper::CreateIdentity4x4()));
	_player->AddBehaviour<Controller>();
	_player->AddBehaviour<Player>();
	_player->GetBehaviour<Physics>()->SetElasticity(0.0f);
}

/*
* Configure the placement of objects here.
*/
void LBDGame::BuildRenderItems()
{
	/*CreateDynamicMeshObject("shape", "box", "stone", 30.0f, XMMatrixScaling(1.0f, 3.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 24.0f, 6.0f), XMMatrixScaling(1.0f, 3.0f, 1.0f));

	CreateDynamicMeshObject("shape", "box", "crate", 10.0f, XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 1.0f, 0.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

	CreateDynamicMeshObject("shape", "custom", "chair", 5.0f, XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 1.0f, -5.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

	CreateDynamicMeshObject("shape", "box", "crate", 1.0f, XMMatrixScaling(2.0f, 2.0f, 2.0f), XMMatrixRotationRollPitchYaw(1.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 6.0f, 0.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, 0.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	XMMATRIX brickTexTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 5; ++i)
	{
		CreateMeshObject("shape", "cylinder", "brick", XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-5.0f, 1.5f, -10.0f + i * 5.0f), brickTexTransform);
		CreateMeshObject("shape", "cylinder", "brick", XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(5.0f, 1.5f, -10.0f + i * 5.0f), brickTexTransform);
		CreateMeshObject("shape", "sphere", "stone", XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-5.0f, 3.5f, -10.0f + i * 5.0f), XMLoadFloat4x4(&MathHelper::CreateIdentity4x4()));
		CreateDynamicMeshObject("shape", "sphere", "stone", 0.5f, XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(5.0f, 20.5f, -10.0f + i * 5.0f), XMLoadFloat4x4(&MathHelper::CreateIdentity4x4()));
	}*/

	// Starting platform.
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.5f, 1.0f, 0.35f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, 0.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	
	// Small corridor leading to challenge area 1.
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.2f, 1.0f, 0.4f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, 11.25f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

	// -- CHALLENGE AREA 1 --
	auto offsetZ1 = 47.25f;
	// Floor
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(1.0f, 1.0f, 2.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, offsetZ1), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	// Obstacles
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(15.0f, 1.0f, 3.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-2.5f, 0.5f, offsetZ1 - 20.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(15.0f, 1.0f, 3.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(2.5f, 0.5f, offsetZ1 - 10.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(15.0f, 1.0f, 3.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-2.5f, 0.5f, offsetZ1), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	// Staircase leading to second challenge area.
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(5.0f, 1.0f, 5.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(1.75f, 2.0f, offsetZ1 + 7.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(5.0f, 1.0f, 5.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-1.75f, 4.0f, offsetZ1 + 17.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(5.0f, 1.0f, 5.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(2.0f, 7.0f, offsetZ1 + 27.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

	// Small corridor leading to challenge area 2.
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.2f, 1.0f, 0.4f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, offsetZ1 + 36.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

	// -- CHALLENGE AREA 2 --
	auto offsetZ2 = 120.0f;
	// Floor
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(2.0f, 1.0f, 2.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, offsetZ2), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	// Walls
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(30.0f, 7.0f, 2.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 2.0f, offsetZ2 - 20.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(25.0f, 7.0f, 2.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-7.5f, 2.0f, offsetZ2 - 10.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(5.0f, 7.0f, 2.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(17.5f, 2.0f, offsetZ2 - 10.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(25.0f, 7.0f, 2.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(7.5f, 2.0f, offsetZ2), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(30.0f, 7.0f, 2.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-5.0f, 2.0f, offsetZ2 + 10.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(30.0f, 7.0f, 2.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(5.0f, 2.0f, offsetZ2 + 20.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

	// Small corridor leading to challenge area 3.
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.2f, 1.0f, 0.4f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, offsetZ2 + 40.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

	// -- CHALLENGE AREA 3 --
	auto offsetZ3 = 200.0f;
	// Floor
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(3.0f, 1.0f, 0.15f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, offsetZ3 - 28.5f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.2f, 1.0f, 1.7f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-20.5f, 0.0f, offsetZ3), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.2f, 1.0f, 1.7f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(20.5f, 0.0f, offsetZ3), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(3.0f, 1.0f, 0.15f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, offsetZ3 + 28.5f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	// Platforms
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(5.0f, 1.0f, 5.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, -1.0f, offsetZ3 - 15.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(5.0f, 1.0f, 5.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, -1.0f, offsetZ3), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(5.0f, 1.0f, 5.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, -1.0f, offsetZ3 + 15.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

	// Small corridor leading to the goal.
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.2f, 1.0f, 0.4f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, offsetZ3 + 40.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

	// -- GOAL --
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.5f, 1.0f, 0.35f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, 250.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

}

GameObject* LBDGame::CreateMeshObject(std::string meshGeometryName, std::string submeshGeometryName, std::string materialName, XMMATRIX scale, XMMATRIX rotation, XMMATRIX translation, XMMATRIX textureTransform)
{
	auto meshObject{ GameState::CreateGameObject() };
	auto mesh{ dynamic_cast<Mesh*>(meshObject->AddBehaviour<Mesh>()) };

	auto meshGeometry{ Render::GetGeometries().at(meshGeometryName).get() };
	auto& submeshGeometry{ meshGeometry->Submeshes.at(submeshGeometryName) };

	meshObject->SetScale(scale);
	meshObject->SetRotation(rotation);
	meshObject->SetTranslation(translation);
	mesh->SetTextureTransform(textureTransform);
	mesh->Mat = Render::GetMaterials().at(materialName).get();
	mesh->Geo = Render::GetGeometries().at(meshGeometryName).get();
	mesh->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mesh->IndexCount = submeshGeometry.IndexCount;
	mesh->StartIndexLocation = submeshGeometry.StartIndexLocation;
	mesh->BaseVertexLocation = submeshGeometry.BaseVertexLocation;

	auto collider{ dynamic_cast<Collider*>(meshObject->AddBehaviour<Collider>()) };
	collider->SetBoundingBox(submeshGeometry.Bounds);
	collider->Transform(meshObject->GetWorldTransform());

	return meshObject;
}

GameObject* LBDGame::CreateDynamicMeshObject(std::string meshGeometryName, std::string submeshGeometryName, std::string materialName, float mass, XMMATRIX scale, XMMATRIX rotation, XMMATRIX translation, XMMATRIX textureTransform)
{
	auto meshObject{ CreateMeshObject(meshGeometryName, submeshGeometryName, materialName, scale, rotation, translation, textureTransform) };
	auto meshGeometry{ Render::GetGeometries().at(meshGeometryName).get() };
	auto& submeshGeometry{ meshGeometry->Submeshes.at(submeshGeometryName) };

	auto collider{ dynamic_cast<Collider*>(meshObject->AddBehaviour<Collider>()) };
	collider->SetBoundingBox(submeshGeometry.Bounds);
	collider->Transform(meshObject->GetWorldTransform());

	auto physics{ dynamic_cast<Physics*>(meshObject->AddBehaviour<Physics>()) };
	physics->SetMass(mass); // set mass to 500g, probably definitely add this into the parameter list
	auto physicsBody = dynamic_cast<PhysicsBody*>(meshObject->AddBehaviour<PhysicsBody>());

	return meshObject;
}
