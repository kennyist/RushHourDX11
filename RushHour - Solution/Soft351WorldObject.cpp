#include "DXUT.h"
#include "Soft351WorldObject.h"

// Constructor
Soft351WorldObject::Soft351WorldObject()
	: Soft351GameObject()
{
	mesh = 0;
}

// Deconstructor
Soft351WorldObject::~Soft351WorldObject()
{
}

// Update the objects behavior
void Soft351WorldObject::Update(float frameTime) {

}

// Render the object
void Soft351WorldObject::Render(ID3D11Device        *pRenderingDevice,
	ID3D11DeviceContext *pImmediateContext)
{
	XMMATRIX matWorldlocal = transform.GetWorldMatrix();
	XMMATRIX matViewLocal = GetViewMatrix();
	XMMATRIX matProjectionLocal = GetProjectionMatrx();
	XMMATRIX matWorldViewProjectionLocal = matWorldlocal * matViewLocal * matProjectionLocal;


	mesh->Render(
		pRenderingDevice,
		pImmediateContext,
		matWorldlocal,
		matWorldViewProjectionLocal
	);
}