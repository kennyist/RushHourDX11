//**************************************************************************//
// This code is copyright of Dr Nigel Barlow, lecturer in computing,		//
// University of Plymouth, UK.  email: nigel@soc.plymouth.ac.uk.			//
//																			//
// You may use, modify and distribute this (rather cack-handed in places)	//
// code subject to the following conditions:								//
//																			//
//	1:	You may not use it, or sell it, or use it in any adapted form for	//
//		financial gain, without my written permission.						//
//																			//
//	2:	You must not remove the copyright messages.							//
//																			//
//	3:	You should correct at least 10% of the typing and speaking errors.  //
//**************************************************************************//


#include "Soft351ShaderAmbient.h"


Soft351ShaderAmbient::Soft351ShaderAmbient()
	: Soft351ShaderAbstract()
{
	pCBLighting = 0;
	pSamLinear = 0;
}


// Release pointers before deletion
void Soft351ShaderAmbient::ReleaseImplementation() 
{
	if (pCBLighting) {
		pCBLighting->Release();
		pCBLighting = NULL;
	}

	if (pSamLinear) {
		pSamLinear->Release();
		pSamLinear = NULL;
	}
}

// Load shader files and compile them into the class/Application
bool Soft351ShaderAmbient::LoadShader(
	ID3D11Device *device
) {
	if (!LoadVertexShader(
		device,
		L".\\Thing3D_VS.hlsl",
		"Thing3D_VS"
	))
	{
		return false;
	}

	if (!LoadPixelShader(
		device,
		L".\\Thing3D_PS.hlsl",
		"Thing3D_PS"
	)) {
		return false;
	}
	
	CreateConstandBuffersAndSampler(device);

	return true;
}

// Set up the matrices and constant buffers.
void Soft351ShaderAmbient::SetupShadersAndConstantbuffers(ID3D11DeviceContext
	*pImmediateContext)
{
	pImmediateContext->VSSetShader(m_vertexShader, NULL, 0);
	pImmediateContext->PSSetShader(m_pixelShader, NULL, 0);

	pImmediateContext->VSSetConstantBuffers(0, 1, &m_MatrixBuffer);
	pImmediateContext->PSSetConstantBuffers(0, 1, &pCBLighting);
	pImmediateContext->PSSetSamplers(0, 1, &pSamLinear);


}

// Create constant buffers and texture sampler.	
void Soft351ShaderAmbient::CreateConstandBuffersAndSampler(ID3D11Device
	*pRenderingDevice)
{
	HRESULT hr = 0;	

	D3D11_SAMPLER_DESC SamDesc;
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.MipLODBias = 0.0f;									
	SamDesc.MaxAnisotropy = 1;									
	SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamDesc.BorderColor[0] = SamDesc.BorderColor[1] = SamDesc.BorderColor[2] = SamDesc.BorderColor[3] = 0;
	SamDesc.MinLOD = 0;
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
	V(pRenderingDevice->CreateSamplerState(&SamDesc, &pSamLinear));
	DXUT_SetDebugName(pSamLinear, "Thing3D Abstract");


	//**************************************************************************//
	// Create the 2 constant bufers, using the same buffer descriptor to create //
	// all three.																//
	//**************************************************************************//
	D3D11_BUFFER_DESC Desc;
	ZeroMemory(&Desc, sizeof(Desc));
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = 0;
	Desc.MiscFlags = 0;

	Desc.ByteWidth = sizeof(CBMatrices);
	V(pRenderingDevice->CreateBuffer(&Desc, NULL, &m_MatrixBuffer));
	DXUT_SetDebugName(m_MatrixBuffer, "CBMatrices");

	Desc.ByteWidth = sizeof(CBLighting);
	V(pRenderingDevice->CreateBuffer(&Desc, NULL, &pCBLighting));
	DXUT_SetDebugName(pCBLighting, "CBLighting");
}

// Update the lighting constant buffer.
void Soft351ShaderAmbient::UpdateCBLighting(ID3D11DeviceContext *pImmediateContext,
	CBLighting          *cbLighting)
{
	pImmediateContext->UpdateSubresource(pCBLighting, 0, NULL, cbLighting, 0, 0);
	pImmediateContext->PSSetConstantBuffers(1, 1, &pCBLighting);
}