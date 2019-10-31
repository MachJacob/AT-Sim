#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include "TextureClass.h"
#include <fstream>
#include <vector>
#include "GameObject.h"
#include <string>

using namespace std;
using namespace DirectX;

class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:

	struct InstanceType
	{
		XMFLOAT3 position;
		XMFLOAT3 rotation;
		XMFLOAT3 scale;
		GameObject* object;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	void SetPosition(float, float, float);

	XMFLOAT3 GetPosition();

	int GetVertexCount();
	int GetInstanceCount();
	ID3D11ShaderResourceView* GetTexture();

	bool AddModel(ID3D11DeviceContext*, GameObject*);
	bool UpdateModels(ID3D11DeviceContext*);

private:
	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	ID3D11Buffer* m_vertexBuffer, * m_instanceBuffer;
	int m_vertexCount, m_indexCount;

	int m_instanceCount;
	int m_maxInstanceCount = 10000;
	TextureClass* m_Texture;
	//std::vector<InstanceType> m_instances;
	InstanceType m_instances[10000];
	int lastInstance;

	ModelType* m_model;

	float xPos;
	float yPos;
	float zPos;
};

#endif // !_MODELCLASS_H_
