#include "ModelClass.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_instanceBuffer = 0;
	m_Texture = 0;
	m_model = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}

bool ModelClass::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFilename)
{
	bool result;

	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	result = InitialiseBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
	ReleaseModel();
	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
	return;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ModelClass::InitialiseBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	//InstanceType* instances;
	//unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, instanceData;
	HRESULT result;
	int i;

	//m_vertexCount = 36;

	//m_indexCount = 36;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	//indices = new unsigned long[m_indexCount];
	//if (!indices)
	//{
	//	return false;
	//}

	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		//indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	/*instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	instanceData.pSysMem = indices;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}*/

	//delete[] indices;
	//indices = 0;

	//m_maxInstanceCount = 10000;
	//m_instances[0] = new InstanceType();
		//.push_back(InstanceType());
	m_instances[0].position = XMFLOAT3(9999.0f, 9999.0f, 9999.0f);
	//m_instances[1] = new InstanceType(); 
	//.push_back(InstanceType());
	m_instances[1].position = XMFLOAT3(9999.0f, 9999.0f, 9999.0f);
	m_instanceCount = 2;

	/*for (int i = 0; i < 100; i++) //GIGA CUBE
	{
		for (int j = 0; j < 100; j++)
		{
			for (int k = 0; k < 100; k++)
			{
				int index = i * 10000 + j * 100 + k;
				instances[index].position = XMFLOAT3(i * 3.0f, j * 3.0f, k * 3.0f);
			}
		}
	}*/

	instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_maxInstanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	instanceData.pSysMem = m_instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;

	//i = 99;
	/*delete[] instances;
	instances = 0;*/

	return true;
}

void ModelClass::ShutdownBuffers()
{
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	strides[0] = sizeof(VertexType);
	strides[1] = sizeof(InstanceType);

	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	deviceContext->IASetIndexBuffer(m_instanceBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;

	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	result = m_Texture->Initialise(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;

	fin.open(filename);

	if (fin.fail())
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_vertexCount;

	m_indexCount = m_vertexCount;

	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	fin.close();

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

void ModelClass::SetPosition(float _x, float _y, float _z)
{
	xPos = _x;
	yPos = _y;
	zPos = _z;
}

XMFLOAT3 ModelClass::GetPosition()
{
	return XMFLOAT3(xPos, yPos, zPos);
}

int ModelClass::GetVertexCount()
{
	return m_vertexCount;
}

int ModelClass::GetInstanceCount()
{
	return m_maxInstanceCount;
}

bool ModelClass::AddModel(ID3D11DeviceContext* deviceContext, GameObject* object)
{
	D3D11_MAPPED_SUBRESOURCE data;
	ZeroMemory(&data, sizeof(data));

	//m_instances[m_instanceCount] = new InstanceType();
	m_instanceCount++;
	size_t copySize = sizeof(InstanceType) * m_instanceCount;

	m_instances[m_instanceCount - 1].position = XMFLOAT3(9999.0f, 9999.0f, 9999.0f);
	m_instances[m_instanceCount - 1].scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_instances[m_instanceCount - 1].rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_instances[m_instanceCount - 1].object = object;

	HRESULT result;
	result = deviceContext->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(result))
	{
		return false;
	}
	memcpy(data.pData, m_instances, copySize);
	deviceContext->Unmap(m_instanceBuffer, 0);
	return true;
}

bool ModelClass::UpdateModels(ID3D11DeviceContext* deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE data;
	ZeroMemory(&data, sizeof(data)); 
	
	size_t copySize = sizeof(InstanceType) * m_instanceCount;
	for (int i = 0; i < m_instanceCount; i++)
	{
		if (m_instances[i].object)
		{
			m_instances[i].position = m_instances[i].object->GetPos();
		}
	}

	HRESULT result;
	result = deviceContext->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(result))
	{
		return false;
	}
	memcpy(data.pData, m_instances, copySize);
	deviceContext->Unmap(m_instanceBuffer, 0);
	return true;
}