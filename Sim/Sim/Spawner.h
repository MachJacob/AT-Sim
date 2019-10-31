#pragma once
#include "GameObject.h"
class Spawner :
	public GameObject
{
public:
	Spawner();
	Spawner(int);
	~Spawner();

	virtual void Tick(float) override;

private:
	bool type;
	int spawned;
	float spawn;
};

