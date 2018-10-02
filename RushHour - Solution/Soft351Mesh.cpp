#include "Soft351Mesh.h"


// Constructor
Soft351Mesh::Soft351Mesh()
{
	// Set shader default values
	m_materialDiffuse = XMFLOAT4(1, 1, 1, 1);
	m_lightDiffuse = XMFLOAT4();
	m_sunPosition = XMVECTOR();
	m_lightAmbient = XMFLOAT4();
	m_useAmbientLight = true;
	alphaTexture = false;

	m_shader = 0;
	m_mesh = 0;
}
// Deconstructor
Soft351Mesh::~Soft351Mesh()
{
}

// Setup and initalize the mesh 
bool Soft351Mesh::Setup(
	ID3D11Device* pd3dDevice,
	LPCTSTR meshFile,
	bool createAdjacentIndicies,
	LPCTSTR meshName,
	Soft351ShaderAmbient* shader
) 
{
	HRESULT result = S_OK;

	// Create mesh object
	m_mesh = new CDXUTSDKMesh();
	if (!m_mesh) {
		// if failed to create object return false and message
		MessageBox(0, L"Mesh class failed", L"Soft351Mesh class", MB_OK);
		return false;
	}

	// Attempt to load the mesh from file
	result = m_mesh->Create(
		pd3dDevice,
		meshFile,	// mesh location
		createAdjacentIndicies
	);
	if (FAILED(result)) {
		// if failed return false and message
		MessageBox(0, L"Mesh failed to create", L"Soft351Mesh class", MB_OK);
		return false;
	}

	// Set the mesh's unique name
	m_name = meshName;
	// set the mesh's shader
	m_shader = shader;

	// everything is ok!
	return true;
}

// Release objects before deletion
void Soft351Mesh::Release()
{
	if (m_mesh) {
		m_mesh->Destroy();
		m_mesh = NULL;
	}
}

// Return the mesh's unique name
LPCTSTR Soft351Mesh::GetName() {
	return m_name;
}

// Render the mesh to screen
void Soft351Mesh::Render(
	ID3D11Device        *pRenderingDevice,
	ID3D11DeviceContext *pImmediateContext,
	XMMATRIX &worldMatrix,
	XMMATRIX &worldViewProjectionMatrix
) 
{
	// --- Method Variables
	Soft351ShaderAmbient::CBMatrices matrixBuffer;
	Soft351ShaderAmbient::CBLighting lightingBuffer;
	UINT Strides[1];
	UINT Offsets[1];
	ID3D11Buffer* pVB[1];
	D3D11_PRIMITIVE_TOPOLOGY  PrimType;
	ID3D11ShaderResourceView  *pDiffuseRV = NULL;
	SDKMESH_SUBSET			  *pSubset = NULL;

	/* ALPHA BLEND TEST, does not work well at all!
	if (alphaTexture) {
		ID3D11BlendState* blendState;
		D3D11_BLEND_DESC blendDescription;
		ZeroMemory(&blendDescription, sizeof(D3D11_BLEND_DESC));
		blendDescription.RenderTarget[0].BlendEnable = TRUE;
		blendDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		pRenderingDevice->CreateBlendState(&blendDescription, &blendState);
		pImmediateContext->OMSetBlendState(blendState, 0, 0xffffffff);
	}
	else {
		ID3D11BlendState* blendState;
		D3D11_BLEND_DESC blendDescription;
		ZeroMemory(&blendDescription, sizeof(D3D11_BLEND_DESC));
		blendDescription.RenderTarget[0].BlendEnable = false;
		blendDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		pRenderingDevice->CreateBlendState(&blendDescription, &blendState);
		pImmediateContext->OMSetBlendState(blendState, 0, 0xffffffff);
	}
	*/
	
	// Update Matrix buffer values
	matrixBuffer.matWorld = XMMatrixTranspose(worldMatrix);
	matrixBuffer.matWorldViewProjection = XMMatrixTranspose(worldViewProjectionMatrix);
	
	// Update Lighting buffer values
	lightingBuffer.blendAmount = 0.3f;			// Alpha blend test
	// If ambient light disable set to full colour (Unlit test)
	if (m_useAmbientLight) {
		lightingBuffer.LightAmbientColour = m_lightAmbient;
	}
	else {
		lightingBuffer.LightAmbientColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	lightingBuffer.lightDiffuseColour = m_lightDiffuse;
	lightingBuffer.materialDiffuseColor = m_materialDiffuse;
	lightingBuffer.vecLightDir = XMVector3Normalize(m_sunPosition);

	// Set Values
	m_shader->UpdateMatrixBuffer(pImmediateContext, &matrixBuffer);
	m_shader->UpdateCBLighting(pImmediateContext, &lightingBuffer);

	// Setup shader
	pImmediateContext->PSSetSamplers(0, 1, &m_shader->pSamLinear);
	m_shader->SetupShadersAndConstantbuffers(pImmediateContext);

	// Set vertex layout
	pImmediateContext->IASetInputLayout(m_shader->m_vertexLayout);
	pVB[0] = m_mesh->GetVB11(0, 0);
	Strides[0] = (UINT)m_mesh->GetVertexStride(0, 0);
	Offsets[0] = 0;
	// Set buffers
	pImmediateContext->IASetVertexBuffers(0, 1, pVB, Strides, Offsets);
	pImmediateContext->IASetIndexBuffer(m_mesh->GetIB11(0), m_mesh->GetIBFormat11(0), 0);

	UINT numSubsets = m_mesh->GetNumSubsets(0);

	for (UINT subset = 0; subset < numSubsets; ++subset)
	{
		pSubset = NULL;

		// Get the subset
		pSubset = m_mesh->GetSubset(0, subset);

		PrimType = CDXUTSDKMesh::GetPrimitiveType11((SDKMESH_PRIMITIVE_TYPE)pSubset->PrimitiveType);
		pImmediateContext->IASetPrimitiveTopology(PrimType);

		// Get the material
		pDiffuseRV = m_mesh->GetMaterial(pSubset->MaterialID)->pDiffuseRV11;
		pImmediateContext->PSSetShaderResources(0, 1, &pDiffuseRV);

		// Render the mesh
		pImmediateContext->DrawIndexed((UINT)pSubset->IndexCount, 0, (UINT)pSubset->VertexStart);
	}

	// Cleanup
	pDiffuseRV = NULL;
	pSubset = NULL;
}

// Set the ambient light colour
void Soft351Mesh::SetAmbientLight(XMFLOAT4 ambLight) {
	m_lightAmbient = ambLight;
}

// Set the light diffuse colour
void Soft351Mesh::SetDiffuseLight(XMFLOAT4 diffuse) {
	m_lightDiffuse = diffuse;
}

// Set the suns position for light direction
void Soft351Mesh::SetSunPosition(XMVECTOR position) {
	m_sunPosition = position;
}