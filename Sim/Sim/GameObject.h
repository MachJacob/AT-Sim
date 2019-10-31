#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class GameObject
{
public:
	
	GameObject();
	GameObject(const GameObject&);
	~GameObject();

	void AddModel();
	void SetPos(float, float, float);
	void SetPos(XMFLOAT3);
	XMFLOAT3 GetPos();

	virtual void Tick(float);

protected:
	float xPos, yPos, zPos;
	float xScale, yScale, zScale;
	float xRot, yRot, zRot;
};

