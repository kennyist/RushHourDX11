#ifndef SOFT351WORLDROW_H
#define SOFT351WORLDROW_H

#include <vector>
#include "Soft351WorldObject.h"
#include "Soft351ResourceManager.h"

class Soft351WorldRow
{
public:
	// --- Public Structs
	// column
	struct Column {
		Soft351WorldObject* object;
	};

	// --- Constructors and deconstruction
	Soft351WorldRow();
	~Soft351WorldRow();

	// --- Public Methods:
	// release objects before deletion
	void Release();
	// create a new column
	void CreateColumns(
		ID3D11Device* renderingDevice,
		ID3D11DeviceContext *renderingDeviceContext,
		float posZ,
		int lanes
	);
	// Set position of all elements in row columns
	void SetPosition(float positionZ);
	// Render all the objects in row
	void Render(
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		XMMATRIX& viewMatrix,
		XMMATRIX& projectionMatrix
	);
	// Update the behavior of a row
	void Update(float frameTime, float roadSpeed);


	// --- Public variables
	// asset manager
	Soft351ResourceManager*  m_resources;
	// row column list
	std::vector<Column> row;
	// is the row active (pool management)
	bool active;

private:
	// --- Private methods:
	// Create a new lane
	Soft351WorldObject* CreateLane(
		ID3D11Device* renderingDevice,
		ID3D11DeviceContext *renderingDeviceContext,
		int type
	);
	// Create a new side object
	Soft351WorldObject* CreateSide(
		ID3D11Device* renderingDevice,
		ID3D11DeviceContext *renderingDeviceContext,
		int index
	);
};

#endif // !SOFT351WORLD_H