#ifndef _SOFT351GAMEOBJECT_H
#define _SOFT351GAMEOBJECT_H

#include "Soft351ShaderAmbient.h"
#include "Soft351Transform.h"

class Soft351GameObject
{  
public:
	// --- Constructor and deconstruction
	Soft351GameObject();
	~Soft351GameObject(); 

	// --- Public methods
	// Update the stored view matrix values
	virtual void SetViewMatrix(XMMATRIX &viewMatrix);
	// Get the stored view matrix
	virtual XMMATRIX GetViewMatrix();
	// Update the stored projection matrix values
	virtual void SetProjectionMatrix(XMMATRIX &projectionMatrix);
	// Get the stored projection matrix
	virtual XMMATRIX GetProjectionMatrx();

	// --- Public variables
	// Transform for position and rotation values 
	Soft351Transform transform;
	// (for pool management) Is the game object active?
	bool active;

private:
	// --- Private variables
	// view matrix storage
	XMMATRIX  m_viewMatrix;
	// porjection matrix storage
	XMMATRIX  m_projectionMatrix;
};

#endif // !_SOFT351GAMEOBJECT_H

