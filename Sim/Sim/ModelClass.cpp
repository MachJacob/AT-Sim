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
	InstanceType* instances;
	unsigned long* indices;
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

	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	//vertices[0].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + -1.0f);  // Bottom left.			FRONT
	//vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[1].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + -1.0f);  // Top left.
	//vertices[1].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[2].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + -1.0f);  // Bottom right.
	//vertices[2].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[3].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + -1.0f);  // top left				FRONT
	//vertices[3].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[4].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + -1.0f);  // Top right.
	//vertices[4].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[5].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + -1.0f);  // bottom right.
	//vertices[5].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[6].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + -1.0f);  // bottom left			LEFT
	//vertices[6].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[7].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + 1.0f);  // bottom left back
	//vertices[7].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[8].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + -1.0f);  // top left.
	//vertices[8].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[9].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + -1.0f);  // top left				LEFT
	//vertices[9].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[11].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + 1.0f);  // top left back
	//vertices[11].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[10].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + 1.0f);  // bottom left back.
	//vertices[10].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[12].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + -1.0f);  // top left			TOP
	//vertices[12].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[13].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + 1.0f);  // top left back
	//vertices[13].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[14].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + -1.0f);  // top right.
	//vertices[14].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[15].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + 1.0f);  // top left back			TOP
	//vertices[15].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[16].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + 1.0f);  // top right back
	//vertices[16].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[17].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + -1.0f);  // top right.
	//vertices[17].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[18].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + -1.0f);  // bot right 			RIGHT
	//vertices[18].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[19].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + -1.0f);  // top right 
	//vertices[19].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[20].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + 1.0f);  // top right back
	//vertices[20].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[21].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + -1.0f);  // bot right 			RIGHT
	//vertices[21].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[22].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + 1.0f);  // top right back
	//vertices[22].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[23].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + 1.0f);  // bot right back
	//vertices[23].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[24].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + 1.0f);  // top right back 			BACK
	//vertices[24].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[25].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + 1.0f);  // top left back
	//vertices[25].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[26].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + 1.0f);  // bot left back
	//vertices[26].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[27].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + 1.0f);  // bot left back 			BACK
	//vertices[27].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[28].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + 1.0f);  // bot right back
	//vertices[28].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[29].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + 1.0f);  // top right back
	//vertices[29].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[30].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + -1.0f);  // bot left 			BOTTOM
	//vertices[30].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[31].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + -1.0f);  // bot right
	//vertices[31].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[32].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + 1.0f);  // bot right back
	//vertices[32].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[33].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + -1.0f);  // bot left 			BOTTOM
	//vertices[33].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[34].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + 1.0f);  // bot right back
	//vertices[34].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[35].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + 1.0f);  // bot left back
	//vertices[35].texture = XMFLOAT2(0.0f, 0.0f);

	//vertices[0].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);  // Bottom left.			FRONT
	//vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[1].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);  // Top left.
	//vertices[1].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[2].position = XMFLOAT3(1.0f, -1.0f, -1.0f);  // Bottom right.
	//vertices[2].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[3].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);  // top left				FRONT
	//vertices[3].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[4].position = XMFLOAT3(1.0f, 1.0f, -1.0f);  // Top right.
	//vertices[4].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[4].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[5].position = XMFLOAT3(1.0f, -1.0f, -1.0f);  // bottom right.
	//vertices[5].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[5].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[6].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);  // bottom left			LEFT
	//vertices[6].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[6].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[7].position = XMFLOAT3(-1.0f, -1.0f, 1.0f);  // bottom left back
	//vertices[7].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[7].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[8].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);  // top left.
	//vertices[8].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[8].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[9].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);  // top left				LEFT
	//vertices[9].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[9].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[11].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);  // top left back
	//vertices[11].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[11].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[10].position = XMFLOAT3(-1.0f, -1.0f, 1.0f);  // bottom left back.
	//vertices[10].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[10].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[12].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);  // top left				TOP
	//vertices[12].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[12].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[13].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);  // top left back
	//vertices[13].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[13].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[14].position = XMFLOAT3(1.0f, 1.0f, -1.0f);  // top right.
	//vertices[14].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[14].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[15].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);  // top left back			TOP
	//vertices[15].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[15].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[16].position = XMFLOAT3(1.0f, 1.0f, 1.0f);  // top right back
	//vertices[16].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[16].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[17].position = XMFLOAT3(1.0f, 1.0f, -1.0f);  // top right.
	//vertices[17].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[17].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[18].position = XMFLOAT3(1.0f, -1.0f, -1.0f);  // bot right 			RIGHT
	//vertices[18].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[18].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[19].position = XMFLOAT3(1.0f, 1.0f, -1.0f);  // top right 
	//vertices[19].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[19].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[20].position = XMFLOAT3(1.0f, 1.0f, 1.0f);  // top right back
	//vertices[20].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[21].position = XMFLOAT3(1.0f, -1.0f, -1.0f);  // bot right 			RIGHT
	//vertices[21].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[21].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[22].position = XMFLOAT3(1.0f, 1.0f, 1.0f);  // top right back
	//vertices[22].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[21].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[23].position = XMFLOAT3(1.0f, -1.0f, 1.0f);  // bot right back
	//vertices[23].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[23].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[24].position = XMFLOAT3(1.0f, 1.0f, 1.0f);  // top right back 			BACK
	//vertices[24].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[24].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[25].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);  // top left back
	//vertices[25].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[25].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[26].position = XMFLOAT3(-1.0f, -1.0f, 1.0f);  // bot left back
	//vertices[26].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[26].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[27].position = XMFLOAT3(-1.0f, -1.0f, 1.0f);  // bot left back 		BACK
	//vertices[27].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[27].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[28].position = XMFLOAT3(1.0f, -1.0f, 1.0f);  // bot right back
	//vertices[28].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[28].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[29].position = XMFLOAT3(1.0f, 1.0f, 1.0f);  // top right back
	//vertices[29].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[29].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[30].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);  // bot left 			BOTTOM
	//vertices[30].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[30].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[31].position = XMFLOAT3(1.0f, -1.0f, -1.0f);  // bot right
	//vertices[31].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[31].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[32].position = XMFLOAT3(1.0f, -1.0f, 1.0f);  // bot right back
	//vertices[32].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[32].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[33].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);  // bot left 			BOTTOM
	//vertices[33].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[33].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[34].position = XMFLOAT3(1.0f, -1.0f, 1.0f);  // bot right back
	//vertices[34].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[34].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	//vertices[35].position = XMFLOAT3(-1.0f, -1.0f, 1.0f);  // bot left back
	//vertices[35].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[35].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//indices[0] = 0;
	//indices[1] = 1;
	//indices[2] = 2;
	//indices[3] = 3;
	//indices[4] = 4;
	//indices[5] = 5;
	//indices[6] = 6;
	//indices[7] = 7;
	//indices[8] = 8;
	//indices[9] = 9;
	//indices[10] = 10;
	//indices[11] = 11;
	//indices[12] = 12;
	//indices[13] = 13;
	//indices[14] = 14;
	//indices[15] = 15;
	//indices[16] = 16;
	//indices[17] = 17;
	//indices[18] = 18;
	//indices[19] = 19;
	//indices[20] = 20;
	//indices[21] = 21;
	//indices[22] = 22;
	//indices[23] = 23;
	//indices[24] = 24;
	//indices[25] = 25;
	//indices[26] = 26;
	//indices[27] = 27;
	//indices[28] = 28;
	//indices[29] = 29;
	//indices[30] = 30;
	//indices[31] = 31;
	//indices[32] = 32;
	//indices[33] = 33;
	//indices[34] = 34;
	//indices[35] = 35;

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

	delete[] vertices;
	vertices = 0;

	//delete[] indices;
	//indices = 0;

	m_instanceCount = 10000;

	instances = new InstanceType[m_instanceCount];

	//instances[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//instances[1].position = XMFLOAT3(3.0f, 0.0f, 0.0f);
	//instances[2].position = XMFLOAT3(0.0f, 3.0f, 0.0f);
	//instances[3].position = XMFLOAT3(0.0f, 0.0f, 3.0f);
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			int index = i * 100 + j;
			instances[index].position = XMFLOAT3(i * 2.0f, 0.0f, j * 2.0f);
		}
	}

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

	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] instances;
	instances = 0;

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
	return m_instanceCount;
}