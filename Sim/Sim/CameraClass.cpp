#include "CameraClass.h"
CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}


XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	upVector = XMLoadFloat3(&up);

	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;
	positionVector = XMLoadFloat3(&position);

	lookAt.x = 1.0f;
	lookAt.y = 0.0f;
	lookAt.z = 0.0f;
	lookAtVector = XMLoadFloat3(&lookAt);

	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	XMMATRIX RotateY;
	RotateY = XMMatrixRotationY(yaw);
	camRight = XMVector3TransformCoord(DefaultRight, RotateY);
	camForward = XMVector3TransformCoord(DefaultForward, RotateY);
	
	positionVector = XMVectorAdd(positionVector, moveBackForward * camForward);
	positionVector = XMVectorAdd(positionVector, moveLeftRight * camRight);
	XMFLOAT3 vFor;
	XMStoreFloat3(&vFor, camForward);
	XMFLOAT3 vRig;
	XMStoreFloat3(&vRig, camRight);

	m_positionX += (vFor.x * moveBackForward) + (vRig.x * moveLeftRight);
	m_positionY += (vFor.y * moveBackForward) + (vRig.y * moveLeftRight);
	m_positionZ += (vFor.z * moveBackForward) + (vRig.z * moveLeftRight);

	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	moveBackForward = 0;
	moveLeftRight = 0;

	return;
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

void CameraClass::SetForwardSpeed(float _speed)
{
	moveBackForward += _speed;
}

void CameraClass::SetRightSpeed(float _speed)
{
	moveLeftRight += _speed;
}