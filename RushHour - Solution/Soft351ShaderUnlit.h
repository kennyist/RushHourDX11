#ifndef _SOFT351SHADERUNLIT_H
#define _SOFT351SHADERUNLIT_H

#include "Soft351ShaderAbstract.h"

class Soft351ShaderUnlit : public Soft351ShaderAbstract
{
public:
	// --- Constructors and deconstruction
	Soft351ShaderUnlit();
	~Soft351ShaderUnlit();

	// --- Public methods
	// Release all pointers before deletion
	virtual void ReleaseImplementation();
	// Load shader files and compile to class/application
	bool LoadShader(
		ID3D11Device *device
	);
};


#endif // !_SOFT351SHADERUNLIT_H

