#include "Soft351GameObject.h"

// Constructor & Set default values
Soft351GameObject::Soft351GameObject()
{
	active = true;
	m_projectionMatrix = m_viewMatrix = XMMatrixIdentity();
	// Setup transform to default values
	transform.Setup();
}

Soft351GameObject::~Soft351GameObject()
{
	
}

// Set the current view matrix
void Soft351GameObject::SetViewMatrix(XMMATRIX &viewMatrix) {
	this->m_viewMatrix = viewMatrix;
}

// Get the stored view matrix
XMMATRIX Soft351GameObject::GetViewMatrix() {
	return this->m_viewMatrix;
}

// Set the current projection matrix
void Soft351GameObject::SetProjectionMatrix(XMMATRIX &projectionMatrix) {
	this->m_projectionMatrix = projectionMatrix;
}

// Get the stored projection matrix
XMMATRIX Soft351GameObject::GetProjectionMatrx() {
	return this->m_projectionMatrix;
}



