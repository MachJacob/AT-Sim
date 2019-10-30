#pragma once
#include <DirectXMath.h>

class GameObject
{
public:
	
	GameObject();
	GameObject(const GameObject&);
	~GameObject();

	void AddModel();
	void SetPos(float, float, float);
	XMFLOAT3 GetPos();

private:
	float xPos, yPos, zPos;
};

