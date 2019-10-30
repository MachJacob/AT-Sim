#pragma once
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "D3dClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ColourShaderClass.h"
#include "TextureShaderClass.h"
#include "LightShaderClass.h"
#include "LightClass.h"
#include <vector>

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float	SCREEN_NEAR = 0.1f;

class GraphicsClass
{
private:
	struct ModelType
	{
		char* modelFilename;
		char* textureFileName;
		ModelClass* model;
	};
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialise(int, int, HWND);
	void Shutdown();
	bool Frame();

	void SetCamera(CameraClass*);
	CameraClass* GetCamera();

	bool AddModel(char*, char*, GameObject*);

private:

	bool Render(float);

private:
	D3DClass* m_Direct3D;
	CameraClass* m_camera;
	ModelClass* m_model;
	ModelClass* m_model2;
	ColourShaderClass* m_colorShader;
	TextureShaderClass* m_textureShader;

	LightShaderClass* m_lightShader;
	LightClass* m_light;

	std::vector<std::unique_ptr<ModelClass>> models;
	std::vector<std::unique_ptr<ModelType>> modelTypes;
};
#endif // _GRAPHICSCLASS_H_
