#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::Tick(float dt)
{
	xPos += (dt * 5) * direction;
	time += dt;
	if (time >= 10)
	{
		direction *= -1;
		time = 0;
	}
}
