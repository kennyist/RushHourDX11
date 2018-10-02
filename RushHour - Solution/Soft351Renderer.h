#ifndef SOFT351RENDERER_H
#define SOFT351RENDERER_H

#include <Windows.h>
//#include "Soft351D3D11.h"
#include "Soft351Camera.h"
//#include "Soft351Mesh.h"
//#include "Soft351Shader.h"
#include "Soft351GameObject.h"


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.01f;


class Soft351Renderer
{
	public:
		Soft351Renderer();
		~Soft351Renderer();

		bool Setup(int, int, HWND);
		void Release();
		bool Frame();

	private:
		bool Render(float);

		//Soft351D3D11* m_dx11;
		Soft351Camera* m_pCamera;
		//Soft351Shader *m_pShader;
		Soft351GameObject *m_gameobject;
		//CDXUTSDKMesh m_mesh;

};

#endif

