#include "../../Headers/Main/LBDGame.h"

void LBDGame::StartGame()
{
	CreatePlayer(XMMatrixTranslation(-3.0f, 1.0f, -10.0f));
	CreatePlayer(XMMatrixTranslation(-1.0f, 1.0f, -10.0f));
	CreatePlayer(XMMatrixTranslation(1.0f, 1.0f, -10.0f));
	CreatePlayer(XMMatrixTranslation(3.0f, 1.0f, -10.0f));
	BuildRenderItems();
}

void LBDGame::CreatePlayer(XMMATRIX translation)
{
	GameObject* player;
	player = CreateDynamicMeshObject("shape", "sphere", "stone", 3.0f, XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), translation, XMLoadFloat4x4(&MathHelper::CreateIdentity4x4()));
	player->AddBehaviour<Controller>();
	player->AddBehaviour<Player>();
	player->GetBehaviour<Physics>()->SetElasticity(0.0f);
	_players.push_back(player);
}

/*
* Configure the placement of objects here.
*/
void LBDGame::BuildRenderItems()
{

	CreateDynamicMeshObject("shape", "box", "stone", 30.0f, XMMatrixScaling(1.0f, 3.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 24.0f, 6.0f), XMMatrixScaling(1.0f, 3.0f, 1.0f));

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
	}
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
