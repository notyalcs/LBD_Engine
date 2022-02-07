#include "../../Headers/Behaviours/Physics.h"

void Physics::Update()
{
	Force.y = GRAVITY * _gravityScale * GameTime::GetDeltaTime();
}
