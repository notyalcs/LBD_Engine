#include "../../Headers/Behaviours/EnemyAI.h"

void EnemyAI::Start() {
	_enemy = GetGameObject();

	MyGrid myGrid{};
	auto behaviours = GameState::GetBehavioursOfType<Collider>();
	for (auto behaviour : behaviours) {
		if (behaviour->GetGameObject() == _enemy) {
			myGrid.enemyRadiusX = behaviour->GetBoundingBox().Extents.x;
			myGrid.enemyRadiusY = behaviour->GetBoundingBox().Extents.y;
			myGrid.enemyRadiusZ = behaviour->GetBoundingBox().Extents.z;
			continue;
		}
		if (behaviour->GetGameObject() == _flag) {
			continue;
		}
		XMFLOAT3 location = behaviour->GetBoundingBox().Center;
		myGrid.locations.push_back(Vector3{ location.x, location.y, location.z });
		XMFLOAT3 size = behaviour->GetBoundingBox().Extents;
		myGrid.sizes.push_back(Vector3{ size.x, size.y, size.z });
	}
	myGrid.currentLocation = Vector3{ _enemy->GetTranslation()._41, _enemy->GetTranslation()._42, _enemy->GetTranslation()._43 };
	myGrid.Commence();

	Pathfinding path{};
	path.grid = myGrid;
	path.FindPath(Vector3{ _enemy->GetTranslation()._41,_enemy->GetTranslation()._42,_enemy->GetTranslation()._43 },
		Vector3{ _flag->GetTranslation()._41, _flag->GetTranslation()._42, _flag->GetTranslation()._43 });

	pathToGoal = path.pathToGoal;
	if (path.pathToGoal.empty()) return;
	nextNode = &pathToGoal[0];


}

void EnemyAI::Update() {
	if (pathToGoal.empty()) return;

	float movementSpeed = 7.0;
	float range = 1.0;

	XMMATRIX position{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, nextNode->worldPosition.x, 0.1f, nextNode->worldPosition.z, 1.0f };
	Node* endNode = &pathToGoal[pathToGoal.size() - 1];
	if (nextNode > endNode) {
		//Start();
		return;
	}

	XMFLOAT4X4 pos;
	XMStoreFloat4x4(&pos, position);

	XMMATRIX point = Utilities::MoveTowards(_enemy->GetTranslation(), pos, movementSpeed * GameTime::GetDeltaTime());

	XMFLOAT4X4 deltaPosition;
	XMStoreFloat4x4(&deltaPosition, point);

	XMFLOAT4X4 currentTranslation = _enemy->GetTranslation();

	_enemy->SetTranslation(XMMatrixTranslation(currentTranslation._41 + deltaPosition._41, currentTranslation._42 + deltaPosition._42, currentTranslation._43 + deltaPosition._43));

	if (Vector3(_enemy->GetTranslation()._41, _enemy->GetTranslation()._42, _enemy->GetTranslation()._43)
		< Vector3(nextNode->worldPosition.x + range, nextNode->worldPosition.y + range, nextNode->worldPosition.z + range) &&
		Vector3(nextNode->worldPosition.x - range, nextNode->worldPosition.y - range, nextNode->worldPosition.z - range)
		< Vector3(_enemy->GetTranslation()._41, _enemy->GetTranslation()._42, _enemy->GetTranslation()._43)) {

		nextNode++;
	}
}