#include "Soft351ShaderAbstract.h"


Soft351ShaderAbstract::Soft351ShaderAbstract()
{
	m_MatrixBuffer = 0;
	m_pixelShader = 0;
	m_pixelShaderBuffer = 0;
	m_vertexLayout = 0;
	m_vertexShader = 0;
	m_vertexShaderBuffer = 0;
	m_textureResourceView = 0;
}


Soft351ShaderAbstract::~Soft351ShaderAbstract()
{
}

void Soft351ShaderAbstract::Release() {
	try {
		this->ReleaseImplementation();
	}
	catch (std::exception e)
	{}

	if (m_textureResourceView) {
		m_textureResourceView->Release();
		m_textureResourceView = NULL;
	}

	if (m_vertexLayout) {
		m_vertexLayout->Release();
		m_vertexLayout = NULL;
	}

	if (m_MatrixBuffer) {
		m_MatrixBuffer->Release();
		m_MatrixBuffer = NULL;
	}

	if (m_pixelShaderBuffer) {
		m_pixelShaderBuffer->Release();
		m_pixelShaderBuffer = NULL;
	}

	if (m_vertexShaderBuffer) {
		m_vertexShaderBuffer->Release();
		m_vertexShaderBuffer = NULL;
	}

	if (m_pixelShader) {
		m_pixelShader->Release();
		m_pixelShader = NULL;
	}

	if (m_vertexShader) {
		m_vertexShader->Release();
		m_vertexShader = NULL;
	}
}


// Compile pixel shader from file into class, returning false if errors occurred
bool Soft351ShaderAbstract::LoadPixelShader(
	ID3D11Device *device,
	WCHAR        *fileName,
	LPCSTR       shaderName
)
{
	HRESULT result = S_OK;

	// --- Pixel shader
	result = CompileShaderFromFile(
		fileName,
		shaderName,
		"ps_4_0",
		&m_pixelShaderBuffer
	);
	if (FAILED(result)) return false;

	result = device->CreatePixelShader(
		m_pixelShaderBuffer->GetBufferPointer(),
		m_pixelShaderBuffer->GetBufferSize(),
		NULL,
		&m_pixelShader
	);
	if (FAILED(result)) return false;

	// Cleanup
	m_pixelShaderBuffer->Release();
	m_pixelShaderBuffer = 0;

	return true;
}

// Compile vertex shader into class, retuning false if errors ocurred
bool Soft351ShaderAbstract::LoadVertexShader(
	ID3D11Device *device,
	WCHAR        *fileName,
	LPCSTR       shaderName
)
{
	HRESULT result = S_OK;

	result = CompileShaderFromFile(
		fileName,
		shaderName,
		"vs_4_0",
		&m_vertexShaderBuffer
	);
	if (FAILED(result)) return false;

	result = device->CreateVertexShader(
		m_vertexShaderBuffer->GetBufferPointer(),
		m_vertexShaderBuffer->GetBufferSize(),
		NULL,
		&m_vertexShader
	);
	if (FAILED(result)) return false;

	result = device->CreateInputLayout(
		VertexLayoutDesc,
		ARRAYSIZE(VertexLayoutDesc),
		m_vertexShaderBuffer->GetBufferPointer(),
		m_vertexShaderBuffer->GetBufferSize(),
		&m_vertexLayout
	);
	if (FAILED(result)) return false;

	return true;
}

// Update matrix buffer parameters
void Soft351ShaderAbstract::UpdateMatrixBuffer(
	ID3D11DeviceContext *pImmediateContext,
	CBMatrices          *cbMatrices)
{

	pImmediateContext->UpdateSubresource(m_MatrixBuffer, 0, NULL, cbMatrices, 0, 0);
	pImmediateContext->VSSetConstantBuffers(0, 1, &m_MatrixBuffer);
}

// Set the current texture to render
void Soft351ShaderAbstract::SetTexture(
	ID3D11DeviceContext       *pImmediateContex,
	ID3D11ShaderResourceView  *pTexture,
	UINT                      textureNumber)
{
	m_textureResourceView = pTexture;
	m_textureNo = textureNumber;
	pImmediateContex->PSSetShaderResources(1, 1, &m_textureResourceView);
}

// Return the vertex shader
ID3D11VertexShader *Soft351ShaderAbstract::GetVertexShader()
{
	return m_vertexShader;
}

// Return the pixel shader
ID3D11PixelShader *Soft351ShaderAbstract::GetPixelShader()
{
	return m_pixelShader;
}

// Compile shader file from local file
HRESULT Soft351ShaderAbstract::CompileShaderFromFile(
	WCHAR* szFileName,
	LPCSTR szEntryPoint,
	LPCSTR szShaderModel,
	ID3DBlob** ppBlobOut) 
{
	HRESULT hr = S_OK;

	// find the file
	WCHAR str[MAX_PATH];
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, szFileName));

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(str, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		WCHAR errorCharsW[1000];
		if (pErrorBlob != NULL)
		{
			CharStrToWideChar(errorCharsW, (char *)pErrorBlob->GetBufferPointer());
			MessageBox(0, errorCharsW, L"Shader file compile error", 0);
			return E_FAIL;
		}
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	SAFE_RELEASE(pErrorBlob);

	return S_OK;
}

// Convert string to wide char
void Soft351ShaderAbstract::CharStrToWideChar(WCHAR *dest, char *source)
{
	int length = strlen(source);
	for (int i = 0; i <= length; i++)
		dest[i] = (WCHAR)source[i];
}


