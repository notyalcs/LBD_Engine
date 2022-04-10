#include "../../Headers/Utilities/GameObject.h"

void GameObject::Start() {
	for (auto& behaviour : _behaviours)
	{
		behaviour->Start();
	}
}

void GameObject::Update()
{
	for (auto& behaviour : _behaviours)
	{
		behaviour->Update();
	}
}

void GameObject::FixedUpdate()
{
	for (auto& behaviour : _behaviours)
	{
		behaviour->FixedUpdate();
	}
}
