#include "SystemClass.h"

SystemClass::SystemClass()
{
	m_input = 0;
	m_graphics = 0;
}
SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}

bool SystemClass::Initialise()
{
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	InitialiseWindows(screenWidth, screenHeight);

	m_input = new InputClass();
	if (!m_input)
	{
		return false;
	}

	m_input->Initialise();

	m_graphics = new GraphicsClass();
	if (!m_graphics)
	{
		return false;
	}

	result = m_graphics->Initialise(screenWidth, screenHeight, m_hwnd);

	/*if (!result)
	{
		return false;
	}
	return true;*/
	return result;
}

void SystemClass::Shutdown()
{
	if (m_graphics)
	{
		m_graphics->Shutdown();
		delete m_graphics;
		m_graphics = 0;
	}

	if (m_input)
	{
		delete m_input;
		m_input = 0;
	}

	ShutDownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	//init memory structure
	ZeroMemory(&msg, NULL, 0, 0, PM_REMOVE);
	
	done = false;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
	return;
}

bool SystemClass::Frame()
{
	bool result;

	//if user presses escape, exit the application
	if (m_input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	if (m_input->IsKeyDown(VK_RIGHT))
	{
		CameraClass* cam = m_graphics->GetCamera();
		cam->SetPosition(cam->GetPosition().x + 1, cam->GetPosition().y, cam->GetPosition().z);
	}

	if (m_input->IsKeyDown(VK_LEFT))
	{
		CameraClass* cam = m_graphics->GetCamera();
		cam->SetPosition(cam->GetPosition().x - 1, cam->GetPosition().y, cam->GetPosition().z);
	}	
	if (m_input->IsKeyDown(VK_UP))
	{
		CameraClass* cam = m_graphics->GetCamera();
		cam->SetPosition(cam->GetPosition().x, cam->GetPosition().y, cam->GetPosition().z + 1);
	}

	if (m_input->IsKeyDown(VK_DOWN))
	{
		CameraClass* cam = m_graphics->GetCamera();
		cam->SetPosition(cam->GetPosition().x, cam->GetPosition().y, cam->GetPosition().z - 1);
	}
	if (m_input->IsKeyDown(VK_NUMPAD8))
	{
		CameraClass* cam = m_graphics->GetCamera();
		cam->SetPosition(cam->GetPosition().x, cam->GetPosition().y + 1, cam->GetPosition().z);
	}
	if (m_input->IsKeyDown(VK_NUMPAD2))
	{
		CameraClass* cam = m_graphics->GetCamera();
		cam->SetPosition(cam->GetPosition().x, cam->GetPosition().y - 1, cam->GetPosition().z);
	}

	result = m_graphics->Frame();
	if (!result)
	{
		return false;
	}
	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_KEYDOWN:
	{
		m_input->KeyDown((unsigned int)wparam);
		return 0;
	}
	case WM_KEYUP:
	{
		m_input->KeyUp((unsigned int)wparam);
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

void SystemClass::InitialiseWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	ApplicationHandle = this;

	m_hinstance = GetModuleHandle(NULL);

	m_applicationName = L"Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		//if fullscreen set screen to max size of desktop
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//place window at top left corner
		posX = posY = 0;
	}
	else
	{
		//if windowed set resolution to 800x600
		screenWidth = 800;
		screenHeight = 600;

		//place window in middle of screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, 
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	ShowCursor(false);

	return;
}

void SystemClass::ShutDownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	//remove window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//remove app instance
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}