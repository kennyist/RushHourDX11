#ifndef _SOFT351_H
#define _SOFT351_H

#include "DXUT.h"
#include "SDKmesh.h"
#include "Soft351ShaderAmbient.h"

class Soft351Mesh
{
public:
	// --- Constructors and deconstruction
	Soft351Mesh();
	~Soft351Mesh();

	// --- Public methods
	// Setup and initialize mesh
	bool Setup(
		ID3D11Device* pd3Device,		// Rendering device
		LPCTSTR meshFile,				// Mesh location
		bool createAdjacentIndicies,	
		LPCTSTR meshName,				// Unique name to store mesh under
		Soft351ShaderAmbient* shader			// Shader the mesh will use
	);
	// Rease all objects before deletion
	void Release();
	// Render the mesh to screen
	void Render(
		ID3D11Device        *pRenderingDevice,
		ID3D11DeviceContext *pImmediateContext,
		XMMATRIX &worldMatrix,
		XMMATRIX &worldViewProjectionMatrix
	);
	// Get this meshes unique name
	LPCTSTR GetName();
	// Set ambient light
	void SetAmbientLight(XMFLOAT4 ambLight);
	// Set diffuse light
	void SetDiffuseLight(XMFLOAT4 diffuse);
	// Set sun position
	void SetSunPosition(XMVECTOR position);
	
	// --- Public Vars
	// The shader the mesh uses
	Soft351ShaderAmbient* m_shader;
	// The mesh file
	CDXUTSDKMesh* m_mesh;
	// Use ambient lighting
	bool m_useAmbientLight;
	// Has alpha
	bool alphaTexture;

	
private:
	// --- Private Vars:
	// mesh's unique name
	LPCTSTR m_name;
	// Light direction
	XMVECTOR m_sunPosition;
	// Light diffuse
	XMFLOAT4 m_lightDiffuse;
	// light ambient colour
	XMFLOAT4 m_lightAmbient;
	// Material diffuse
	XMFLOAT4 m_materialDiffuse;
};

#endif // !_SOFT351_H