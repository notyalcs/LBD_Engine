#pragma once
#include "Behaviour.h"
#include "../Behaviours/Pathfinding.h"
#include "../Utilities/MyGrid.h"
#include "../Utilities/Node.h"
#include "../Utilities/GameTime.h"
#include "../Main/LBDGame.h"
#include "../Main/GameState.h"
#include "../Utilities/Utilities.h"

class EnemyAI : public Behaviour {
public:

	GameObject* _enemy;
	GameObject* _flag;

	std::vector<Node> pathToGoal;
	Node* nextNode;

	void Start();

	void Update();
};