#pragma once
#include "GameObject.h"
class Entity :
	public GameObject
{
public:
	Entity();
	~Entity();
private:
	void Tick(float) override;
	int direction = 1;
	float time = 0;
};

