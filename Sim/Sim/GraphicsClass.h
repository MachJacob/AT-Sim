#pragma once
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "D3dClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ColourShaderClass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float	SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialise(int, int, HWND);
	void Shutdown();
	bool Frame();

	void SetCamera(CameraClass*);
	CameraClass* GetCamera();

private:

	bool Render();

private:
	D3DClass* m_Direct3D;
	CameraClass* m_camera;
	ModelClass* m_model;
	ColourShaderClass* m_colorShader;
};
#endif // _GRAPHICSCLASS_H_
