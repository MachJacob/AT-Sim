#pragma once
#include "inputclass.h"
#include "graphicsclass.h"
#include "GameObject.h"
#include <vector>
class Game
{
public:
	Game();
	Game(const Game&);
	~Game();

	bool Initialise(GraphicsClass*, InputClass*);
	void Shutdown();

	bool Tick(float);
	bool Render();
	bool AddModel(char*, char*, GameObject*);

private:
	InputClass* m_input;
	GraphicsClass* m_graphics;
	CameraClass* m_camera;

	float space = 0.0f;

	std::vector<std::unique_ptr<GameObject>> objects;
};

