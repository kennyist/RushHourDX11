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
//	3:	You should correct at least 10% of the typig abd spekking errirs.   //
//**************************************************************************//

#include "Soft351ShaderAbstract.h"

#ifndef SOFT351SHADERAMBIENT_H
#define SOFT351SHADERAMBIENT_H

class Soft351ShaderAmbient : public Soft351ShaderAbstract
{
public:
	// --- Public Structs

	// lighting buffer
	struct CBLighting
	{
		XMFLOAT4 materialDiffuseColor;      // Material's  color
		XMVECTOR vecLightDir;               // Light's direction in world space
		XMFLOAT4 lightDiffuseColour;        // Light's diffuse color
		XMFLOAT4 LightAmbientColour;		// Light's's diffuse color
		float blendAmount;					// Alpha blend value
	};


	// Vertex layout
	struct VertexStruct
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 texUV;
	};

public:
	// --- Constructor
	Soft351ShaderAmbient();

	// --- Public methods
	// Release and delete pointers
	virtual void ReleaseImplementation();
	// Load and compile shader to class
	bool LoadShader(
		ID3D11Device *device
	);
	// Create constant buffers and texture sampler.	
	void CreateConstandBuffersAndSampler(ID3D11Device *pRenderingDevice);
	// Set up the matrices and constant buffers.
	void SetupShadersAndConstantbuffers(
		ID3D11DeviceContext *pImmediateContext
	);
	// Update the lighting constant buffer.	
	void UpdateCBLighting(
		ID3D11DeviceContext *pImmediateContext,
		CBLighting           *cbLighting
	);

	// --- Public variables
	// Lighting buffer
	ID3D11Buffer               *pCBLighting;
	// Sample state
	ID3D11SamplerState         *pSamLinear;
};


#endif

