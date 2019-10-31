#include "Game.h"

Game::Game()
{
	m_input = 0;
	m_graphics = 0;
}

Game::Game(const Game& other)
{

}

Game::~Game()
{

}

bool Game::Initialise(GraphicsClass* _graphics, InputClass* _input)
{
	//initialise game
	m_graphics = _graphics;
	m_input = _input;
	m_camera = new CameraClass();
	if (!m_camera)
	{
		return false;
	}

	m_graphics->SetCamera(m_camera);

	m_camera->SetPosition(-5.0f, 5.0f, 0.0f);
	m_camera->SetRotation(0.0f, 90.0f, 0.0f);

	char model[] = "../Sim/data/cube.txt";
	char file[] = "../Sim/data/stone01.tga";

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			objects.push_back(std::make_unique<GameObject>());
			objects.back()->SetPos(i * 2, 0, j * 2);
			AddModel(model, file, objects.back().get());
		}
	}
	return true;
}

void Game::Shutdown()
{
	//kill everything
}

bool Game::Tick(float dt)
{
	//run game loop
	//if user presses escape, exit the application

	if (m_input->IsKeyDown('D')) //d
	{
		m_camera->SetRightSpeed(10 * dt);
	}

	if (m_input->IsKeyDown('A')) //a
	{
		m_camera->SetRightSpeed(-10 * dt);
	}
	if (m_input->IsKeyDown('W')) //w
	{
		m_camera->SetForwardSpeed(10 * dt);
	}

	if (m_input->IsKeyDown('S')) //s
	{
		m_camera->SetForwardSpeed(-10 * dt);
	}
	if (m_input->IsKeyDown('E')) //e
	{
		m_camera->SetPosition(m_camera->GetPosition().x, m_camera->GetPosition().y + 10 * dt, m_camera->GetPosition().z);
		//m_camera->SetUpSpeed(10 * dt);
	}
	if (m_input->IsKeyDown('Q')) //q
	{
		m_camera->SetPosition(m_camera->GetPosition().x, m_camera->GetPosition().y - 10 * dt, m_camera->GetPosition().z);
		//m_camera->SetUpSpeed(10 * dt);
	}
	if (m_input->IsKeyDown(VK_LEFT))
	{
		m_camera->SetRotation(m_camera->GetRotation().x, m_camera->GetRotation().y - 60 * dt, m_camera->GetRotation().z);
		m_camera->SetUpSpeed(10 * dt);
	}
	if (m_input->IsKeyDown(VK_RIGHT))
	{
		m_camera->SetRotation(m_camera->GetRotation().x, m_camera->GetRotation().y + 60 * dt, m_camera->GetRotation().z);
	}
	if (m_input->IsKeyDown(VK_UP))
	{
		m_camera->SetRotation(m_camera->GetRotation().x - 60 * dt, m_camera->GetRotation().y, m_camera->GetRotation().z);
	}
	if (m_input->IsKeyDown(VK_DOWN))
	{
		m_camera->SetRotation(m_camera->GetRotation().x + 60 * dt, m_camera->GetRotation().y, m_camera->GetRotation().z);
	}
	if (m_input->IsKeyDown('T'))
	{
		char model[] = "../Sim/data/cube.txt";
		char file[] = "../Sim/data/metal01.tga";
		objects.push_back(std::make_unique<Entity>());
		objects.back()->SetPos(0, 5, space);
		AddModel(model, file, objects.back().get());
		space += 3.0f;
	}
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Tick(dt);
	}
	return true;
}

bool Game::Render()
{
	//render game
	return true;
}

bool Game::AddModel(char* modelFilename, char* textureFilename, GameObject* object)
{
	m_graphics->AddModel(modelFilename, textureFilename, object);
	return false;
}
