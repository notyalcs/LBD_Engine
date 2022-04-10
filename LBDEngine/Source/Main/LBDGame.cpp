#include "../../Headers/Main/LBDGame.h"

void LBDGame::StartGame()
{
	CreatePlayer();
	CreateEnemy();
	BuildRenderItems();
}

void LBDGame::CreatePlayer()
{
	_player = CreateDynamicMeshObject("shape", "sphere", "stone", 3.0f, XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 2.0f, 0.0f), XMLoadFloat4x4(&MathHelper::CreateIdentity4x4()));
	_player->AddBehaviour<Controller>();
	_player->AddBehaviour<Player>();
	_player->GetBehaviour<Physics>()->SetElasticity(0.0f);
}

void LBDGame::CreateEnemy()
{
	GameObject* _enemy = CreateDynamicMeshObject("shape", "shrek", "shrek", 3.0f, XMMatrixScaling(0.0002f, 0.0002f, 0.0002f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(2.5f, 1.0f, 0.0f), XMLoadFloat4x4(&MathHelper::CreateIdentity4x4()));
	GameObject* _flag = CreateDynamicMeshObject("shape", "sphere", "stone", 3.0f, XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.5f, 225.0f), XMLoadFloat4x4(&MathHelper::CreateIdentity4x4()));
	//_flag->GetBehaviour<Physics>()->SetGravity(false);
	//_enemy->GetBehaviour<Physics>()->SetGravity(false);
	static_cast<EnemyAI*>(_enemy->AddBehaviour<EnemyAI>())->_flag = _flag;
}

/*
* Configure the placement of objects here.
*/
void LBDGame::BuildRenderItems()
{
	// Starting platform.
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.5f, 1.0f, 0.35f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, 0.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	
	// Small corridor leading to challenge area 1.
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.2f, 1.0f, 0.4f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, 11.25f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

	// -- CHALLENGE AREA 1 --
	auto offsetZ1 = 39.75f;
	// Floor
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(1.0f, 1.0f, 1.5f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, offsetZ1), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	// Obstacles
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(15.0f, 1.0f, 3.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-2.5f, 0.5f, offsetZ1 - 15.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(15.0f, 1.0f, 3.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(2.5f, 0.5f, offsetZ1 - 5.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(15.0f, 1.0f, 3.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-2.5f, 0.5f, offsetZ1 + 5.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	// Platforms leading to challenge area 2.
	auto platform{ CreateMeshObject("shape", "box", "brick", XMMatrixScaling(5.0f, 1.0f, 5.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-2.0f, 2.0f, offsetZ1 + 15.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f)) };
	auto platformBehaviour{ platform->AddBehaviour<Platform>() };
	static_cast<Platform*>(platformBehaviour)->AddMovementPoint({ -2.0f, 2.0f, offsetZ1 + 15.0f });
	static_cast<Platform*>(platformBehaviour)->AddMovementPoint({ 2.0f, 2.0f, offsetZ1 + 15.0f });
	platform = CreateMeshObject("shape", "box", "brick", XMMatrixScaling(5.0f, 1.0f, 5.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 4.0f, offsetZ1 + 25.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	platformBehaviour = platform->AddBehaviour<Platform>();
	static_cast<Platform*>(platformBehaviour)->AddMovementPoint({ 2.0f, 4.0f, offsetZ1 + 25.0f });
	static_cast<Platform*>(platformBehaviour)->AddMovementPoint({ -2.0f, 4.0f, offsetZ1 + 25.0f });
	platform = CreateMeshObject("shape", "box", "brick", XMMatrixScaling(5.0f, 1.0f, 5.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(2.0f, 7.0f, offsetZ1 + 35.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	platformBehaviour = platform->AddBehaviour<Platform>();
	static_cast<Platform*>(platformBehaviour)->AddMovementPoint({ 2.0f, 7.0f, offsetZ1 + 35.0f });
	static_cast<Platform*>(platformBehaviour)->AddMovementPoint({ -2.0f, 7.0f, offsetZ1 + 35.0f });

	// Small corridor leading to challenge area 2.
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.2f, 1.0f, 0.4f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, offsetZ1 + 28.5f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

	// -- CHALLENGE AREA 2 --
	auto offsetZ2 = 104.25f;
	// Floor
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(2.0f, 1.0f, 2.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, offsetZ2), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	// Walls
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(30.0f, 5.0f, 2.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 2.5f, offsetZ2 - 20.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(25.0f, 5.0f, 2.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-7.5f, 2.5f, offsetZ2 - 10.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(5.0f, 5.0f, 2.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(17.5f, 2.5f, offsetZ2 - 10.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(25.0f, 5.0f, 2.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(7.5f, 2.5f, offsetZ2), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(30.0f, 5.0f, 2.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-5.0f, 2.5f, offsetZ2 + 10.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "box", "brick", XMMatrixScaling(30.0f, 5.0f, 2.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(5.0f, 2.5f, offsetZ2 + 20.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

	// Small corridor leading to challenge area 3.
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.2f, 1.0f, 0.4f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, offsetZ2 + 36.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

	// -- CHALLENGE AREA 3 --
	auto offsetZ3 = 176.25f;
	// Floor
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(3.0f, 1.0f, 0.15f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, offsetZ3 - 27.75f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.2f, 1.0f, 1.7f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-28.0f, 0.0f, offsetZ3), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.2f, 1.0f, 1.7f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(28.0f, 0.0f, offsetZ3), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(3.0f, 1.0f, 0.15f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, offsetZ3 + 27.75f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	// Platforms
	platform = CreateMeshObject("shape", "box", "brick", XMMatrixScaling(5.0f, 1.0f, 5.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(-12.0f, -1.0f, offsetZ3 - 17.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	platformBehaviour = platform->AddBehaviour<Platform>();
	static_cast<Platform*>(platformBehaviour)->AddMovementPoint({ -12.0f, -1.0f, offsetZ3 - 17.0f });
	static_cast<Platform*>(platformBehaviour)->AddMovementPoint({ 12.0f, -1.0f, offsetZ3 - 17.0f });
	platform = CreateMeshObject("shape", "box", "brick", XMMatrixScaling(5.0f, 1.0f, 5.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, -1.0f, offsetZ3 + 5.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	platformBehaviour = platform->AddBehaviour<Platform>();
	static_cast<Platform*>(platformBehaviour)->AddMovementPoint({ 0.0f, -1.0f, offsetZ3 + 5.0f });
	static_cast<Platform*>(platformBehaviour)->AddMovementPoint({ 0.0f, -1.0f, offsetZ3 - 5.0f });
	platform = CreateMeshObject("shape", "box", "brick", XMMatrixScaling(5.0f, 1.0f, 5.0f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(12.0f, -1.0f, offsetZ3 + 17.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));
	platformBehaviour = platform->AddBehaviour<Platform>();
	static_cast<Platform*>(platformBehaviour)->AddMovementPoint({ 12.0f, -1.0f, offsetZ3 + 17.0f });
	static_cast<Platform*>(platformBehaviour)->AddMovementPoint({ -12.0f, -1.0f, offsetZ3 + 17.0f });

	// Small corridor leading to the goal.
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.2f, 1.0f, 0.4f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, offsetZ3 + 36.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

	// -- GOAL --
	CreateMeshObject("shape", "grid", "tile", XMMatrixScaling(0.5f, 1.0f, 0.35f), XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f), XMMatrixTranslation(0.0f, 0.0f, 223.5f), XMMatrixScaling(1.0f, 1.0f, 1.0f));

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
