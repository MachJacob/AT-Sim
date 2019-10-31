#include "Spawner.h"

Spawner::Spawner()
{
	type = 0;
	spawned = 0;
}

Spawner::Spawner(int _int)
{
	type = _int;
	spawned = 0;
}

Spawner::~Spawner()
{
}

void Spawner::Tick(float dt)
{
	spawn += dt;
	if (type == 0 && spawned <= 3 && spawn >= 5)
	{
		//spawn slime
	}
	
	if (type == 1 && spawned <= 3 && spawn >= 10)
	{
		//spawn soldier
	}
}
