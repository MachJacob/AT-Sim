#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_Direct3D = 0;
	m_camera = 0;
	m_model = 0;
	m_colorShader = 0;
	m_textureShader = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialise(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	result = m_Direct3D->Initialise(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		return false;
	}

	char model[] = "../Sim/data/cube.txt";
	char file[] = "../Sim/data/stone01.tga";
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			for (int k = 0; k < 1; k++)
			{
				models.push_back(std::make_unique<ModelClass>());

				models[i * 100 + j * 10 + k]->SetPosition(i * 5, k * 5, j * 5);

				result = models[i * 100 + j * 10 + k]->Initialise(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), model, file);
				if (!result)
				{
					MessageBox(hwnd, L"Could not initialise the model object.", L"Error", MB_OK);
					return false;
				}
			}
		}
	}

	m_lightShader = new LightShaderClass;
	if (!m_lightShader)
	{
		return false;
	}

	result = m_lightShader->Initialise(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	m_light = new LightClass;
	if (!m_light)
	{
		return false;
	}

	m_light->SetAmbientColour(0.0f, 0.15f, 0.15f, 1.0f);
	m_light->SetDiffuseColour(0.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetDirection(1.0f, -1.0f, 1.0f);

	m_textureShader = new TextureShaderClass;
	if (!m_textureShader)
	{
		return false;
	}

	result = m_textureShader->Initialise(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialise texture shader object", L"Error", MB_OK);
		return false;
	}

	/*m_colorShader = new ColourShaderClass;
	if (!m_colorShader)
	{
		return false;
	}

	result = m_colorShader->Initialise(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		return false;
	}*/

	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_light)
	{
		delete m_light;
		m_light = 0;
	}

	if (m_lightShader)
	{
		m_lightShader->Shutdown();
		delete m_lightShader;
		m_lightShader = 0;
	}

	if (m_textureShader)
	{
		m_textureShader->Shutdown();
		delete m_textureShader;
		m_textureShader = 0;
	}
	/*if (m_colorShader)
	{
		m_colorShader->Shutdown();
		delete m_colorShader;
		m_colorShader = 0;
	}*/

	if (m_model)
	{
		m_model->Shutdown();
		delete m_model;
		m_model = 0;
	}

	for (int i = 0; i < models.size(); i++)
	{
		models[i]->Shutdown();
	}

	if (m_model2)
	{
		m_model2->Shutdown();
		delete m_model2;
		m_model2 = 0;
	}

	if (m_camera)
	{
		delete m_camera;
		m_camera = 0;
	}
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;
	
	static float rotation = 0.0f;

	rotation += (float)XM_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation = -360.0f;
	}
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	m_Direct3D->BeginScene(0.1f, 0.1f, 0.1f, 1.0f);

	m_camera->Render();

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjetionMatrix(projectionMatrix);

	for (int i = 0; i < models.size(); i++)
	{
		models[i]->Render(m_Direct3D->GetDeviceContext());

		/*result = m_textureShader->Render(m_Direct3D->GetDeviceContext(), models[i]->GetVertexCount(), models[i]->GetInstanceCount(), worldMatrix, viewMatrix, projectionMatrix, models[i]->GetTexture());
		if (!result)
		{
			return false;
		}*/

		result = m_lightShader->Render(m_Direct3D->GetDeviceContext(), models[i]->GetVertexCount(), models[i]->GetInstanceCount(), worldMatrix, 
			viewMatrix, projectionMatrix, models[i]->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColour(), m_light->GetAmbientColour());
		if (!result)
		{
			return false;
		}
	}
	

	/*result = m_colorShader->Renderer(m_Direct3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}*/

	m_Direct3D->EndScene();

	return true;
}

CameraClass* GraphicsClass::GetCamera()
{
	return m_camera;
}

void GraphicsClass::SetCamera(CameraClass* _cam)
{
	m_camera = _cam;
}