#include "DXUT.h"
#include "Soft351Renderer.h"


Soft351Renderer::Soft351Renderer()
{
}


Soft351Renderer::~Soft351Renderer()
{
}

bool Soft351Renderer::Setup(
	int screenWidth, 
	int screenHeight, 
	HWND hwnd) 
{
	bool result;

	/*m_dx11 = new Soft351D3D11;
	if (!m_dx11)
	{
		return false;
	}

	result = m_dx11->Setup(
		screenWidth,
		screenHeight,
		VSYNC_ENABLED,
		hwnd,
		FULL_SCREEN,
		SCREEN_DEPTH,
		SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX11", L"Error", MB_OK);
		return false;
	}*/

	m_pCamera = new Soft351Camera;
	if (!m_pCamera)
		return false;

	m_pCamera->transform.SetPosition(0.0f, 1.5f, -5.0f);

	// Shader
	HRESULT hr = S_OK;

	/*m_pShader = new Soft351Shader();
	result = m_pShader->CreatVertexShader(
		m_dx11->GetDevice(),
		L"Thing3D_VS.hlsl",
		"Thing3D_VS");

	if (FAILED(result))
		return false;

	result = m_pShader->CreatPixelShader(
		m_dx11->GetDevice(),
		L"Thing3D_PS.hlsl",
		"Thing3D_PS");

	if (FAILED(result))
		return false;
	
	m_pShader->CreateConstandBuffersAndSampler(
		m_dx11->GetDevice()
	);

	// mesh

	ID3D11Device* device = m_dx11->GetDevice();

	V(m_mesh.Create(
		device,
		L"Media\\Tiny\\tiny.sdkmesh",
		true
	));
	*/
	return true;
}

void Soft351Renderer::Release()
{
	if (m_pCamera)
		delete m_pCamera;

	/*if (m_pShader)
		delete m_pShader;

	// release dx11 class
	if (m_dx11) {
		m_dx11->Release();
		delete m_dx11;
		m_dx11 = NULL;
	}*/
}

bool Soft351Renderer::Frame()
{
	bool result;

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	result = Render(rotation);
	if (!result) {
		return false;
	}

	return true;
}

bool Soft351Renderer::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	//m_dx11->StartScene(0.25f, 0.25f, 1.0f, 1.0f);
	//float deltaTime = m_dx11->DeltaTime();

	// Generate the view matrix based on the camera's position.
	m_pCamera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	/*m_dx11->GetWorldMatrix(worldMatrix);
	m_pCamera->GetViewMatrix(viewMatrix);
	m_dx11->GetProjectionMatrix(projectionMatrix);*/

	/*m_mesh1->Render(m_dx11->GetDeviceContext());

	result = m_shaderBasic->Render(
		m_dx11->GetDeviceContext(),
		m_mesh1->IndexCount(),
		worldMatrix,
		viewMatrix,
		projectionMatrix
	);

	/*for (int i = 0; i < 10; i++) {

	m_dx11->GetWorldMatrix(worldMatrix);
	}*/


	//m_dx11->EndScene();
	return true;
}
