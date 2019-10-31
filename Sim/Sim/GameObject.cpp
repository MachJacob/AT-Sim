#include "GameObject.h"

GameObject::GameObject()
{
	
}

GameObject::GameObject(const GameObject& other)
{

}

GameObject::~GameObject()
{

}

void GameObject::AddModel()
{
	
}

void GameObject::SetPos(float _posx, float _posy, float _posz)
{
	xPos = _posx;
	yPos = _posy;
	zPos = _posz;
}

void GameObject::SetPos(XMFLOAT3 _pos)
{
	xPos = _pos.x;
	yPos = _pos.y;
	zPos = _pos.z;
}

XMFLOAT3 GameObject::GetPos()
{
	return XMFLOAT3(xPos, yPos, zPos);
}

void GameObject::Tick(float dt)
{
	//game logic
}
