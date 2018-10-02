#ifndef _SOFT351SHADERABSTRACT_H
#define _SOFT351SHADERABSTRACT_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "DXUT.h"
#include "SDKmisc.h"
#include <string.h>
#include <wchar.h>
#include <exception>
using namespace DirectX;

// Vertex layout as described in the shader files
const D3D11_INPUT_ELEMENT_DESC VertexLayoutDesc[] =
{
	{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// Abstract class for all shaders
class Soft351ShaderAbstract
{
public:
	// --- Public structs
	// Matrix buffer layout
	struct CBMatrices
	{
		XMMATRIX matWorld;           
		XMMATRIX matWorldViewProjection;
	};

	// --- Constructors and deconstruction
	Soft351ShaderAbstract();
	~Soft351ShaderAbstract();

	// --- Public Methods
	void Release();
	virtual void ReleaseImplementation() = 0;
	// Load shader method virtual to be used in shader classes
	virtual bool LoadShader(ID3D11Device *device) = 0;
	// Load pixel shader file, not virtual as this shouldn't need overriding
	bool LoadPixelShader(
		ID3D11Device *device,
		WCHAR        *fileName,
		LPCSTR       shaderName
	);
	// Load vertex shader file, not virtual as this shouldn't need overriding 
	bool LoadVertexShader(
		ID3D11Device *device,
		WCHAR        *fileName,
		LPCSTR        shanderName
	);
	// Update matrix buffer parameters
	virtual void UpdateMatrixBuffer(
		ID3D11DeviceContext *pImmediateContext,
		CBMatrices          *cbMatrices
	);
	// Set the current texture to render
	virtual void SetTexture(
		ID3D11DeviceContext       *pImmediateContex,
		ID3D11ShaderResourceView  *pTexture,
		UINT textureNumber
	);
	// Return the vertex shader, not virtual as base level accessors
	ID3D11VertexShader *GetVertexShader();
	// Return the pixel shader, not virtual as base level accessors
	ID3D11PixelShader *GetPixelShader();

	// --- Public Variables
	// Matrix buffer
	ID3D11Buffer               *m_MatrixBuffer;
	// Pixel shader
	ID3D11PixelShader          *m_pixelShader;
	// Buffer for pixel shader
	ID3DBlob                   *m_pixelShaderBuffer;
	// Vertex shader
	ID3D11VertexShader         *m_vertexShader;
	// Buffer for vertex shader
	ID3DBlob                   *m_vertexShaderBuffer;
	// Vertex layout for shader
	ID3D11InputLayout          *m_vertexLayout;
	// Unique name for resource manager
	LPCTSTR						m_name;
	// Texture holder
	ID3D11ShaderResourceView   *m_textureResourceView;
	// Texture number
	UINT					   m_textureNo;
	

private:
	// --- Private methods
	// Load shader file and compile it, not virtual as base level method only
	HRESULT CompileShaderFromFile(
		WCHAR* szFileName,
		LPCSTR szEntryPoint,
		LPCSTR szShaderModel,
		ID3DBlob** ppBlobOut);
	// convert string to wide char, not virtual as base level method only
	void CharStrToWideChar(WCHAR *dest, char *source);
};


#endif

