#include "ModelClass.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}

bool ModelClass::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename)
{
	bool result;

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
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = 36;

	m_indexCount = 36;

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

	vertices[0].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + -1.0f);  // Bottom left.			FRONT
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + -1.0f);  // Top left.
	vertices[1].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[2].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + -1.0f);  // Bottom right.
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[3].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + -1.0f);  // top left				FRONT
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + -1.0f);  // Top right.
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + -1.0f);  // bottom right.
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);
	
	vertices[6].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + -1.0f);  // bottom left			LEFT
	vertices[6].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[7].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + 1.0f);  // bottom left back
	vertices[7].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[8].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + -1.0f);  // top left.
	vertices[8].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[9].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + -1.0f);  // top left				LEFT
	vertices[9].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[11].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + 1.0f);  // top left back
	vertices[11].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[10].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + 1.0f);  // bottom left back.
	vertices[10].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[12].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + -1.0f);  // top left			TOP
	vertices[12].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[13].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + 1.0f);  // top left back
	vertices[13].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[14].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + -1.0f);  // top right.
	vertices[14].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[15].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + 1.0f);  // top left back			TOP
	vertices[15].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[16].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + 1.0f);  // top right back
	vertices[16].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[17].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + -1.0f);  // top right.
	vertices[17].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[18].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + -1.0f);  // bot right 			RIGHT
	vertices[18].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[19].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + -1.0f);  // top right 
	vertices[19].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[20].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + 1.0f);  // top right back
	vertices[20].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[21].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + -1.0f);  // bot right 			RIGHT
	vertices[21].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[22].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + 1.0f);  // top right back
	vertices[22].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[23].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + 1.0f);  // bot right back
	vertices[23].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[24].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + 1.0f);  // top right back 			BACK
	vertices[24].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[25].position = XMFLOAT3(xPos + -1.0f, yPos + 1.0f, zPos + 1.0f);  // top left back
	vertices[25].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[26].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + 1.0f);  // bot left back
	vertices[26].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[27].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + 1.0f);  // bot left back 			BACK
	vertices[27].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[28].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + 1.0f);  // bot right back
	vertices[28].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[29].position = XMFLOAT3(xPos + 1.0f, yPos + 1.0f, zPos + 1.0f);  // top right back
	vertices[29].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[30].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + -1.0f);  // bot left 			BOTTOM
	vertices[30].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[31].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + -1.0f);  // bot right
	vertices[31].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[32].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + 1.0f);  // bot right back
	vertices[32].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[33].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + -1.0f);  // bot left 			BOTTOM
	vertices[33].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[34].position = XMFLOAT3(xPos + 1.0f, yPos + -1.0f, zPos + 1.0f);  // bot right back
	vertices[34].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[35].position = XMFLOAT3(xPos + -1.0f, yPos + -1.0f, zPos + 1.0f);  // bot left back
	vertices[35].texture = XMFLOAT2(0.0f, 0.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 4;
	indices[5] = 5;
	indices[6] = 6;
	indices[7] = 7;
	indices[8] = 8;
	indices[9] = 9;
	indices[10] = 10;
	indices[11] = 11;
	indices[12] = 12;
	indices[13] = 13;
	indices[14] = 14;
	indices[15] = 15;
	indices[16] = 16;
	indices[17] = 17;
	indices[18] = 18;
	indices[19] = 19;
	indices[20] = 20;
	indices[21] = 21;
	indices[22] = 22;
	indices[23] = 23;
	indices[24] = 24;
	indices[25] = 25;
	indices[26] = 26;
	indices[27] = 27;
	indices[28] = 28;
	indices[29] = 29;
	indices[30] = 30;
	indices[31] = 31;
	indices[32] = 32;
	indices[33] = 33;
	indices[34] = 34;
	indices[35] = 35;

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

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
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
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

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