#include "Soft351WorldRow.h"

// -- Usefull links
// Vector:
// http://www.cplusplus.com/reference/vector/vector/

// Constructor and initial value setup
Soft351WorldRow::Soft351WorldRow()
{
	active = true;
	m_resources = 0;
}

// Deconstructor
Soft351WorldRow::~Soft351WorldRow()
{
}

// Create a new row
void Soft351WorldRow::CreateColumns(
	ID3D11Device* renderingDevice,
	ID3D11DeviceContext *renderingDeviceContext,
	float posZ, 
	int lanes
)
{
	// for lane count, plus 2 for sides of the road
	for (int i = 0; i < (lanes + 2); i++) {
		Column col;

		// if first or last 
		if (i == 0 || i == (lanes + 1)) 
		{
			// create side object
			col.object = CreateSide(
				renderingDevice,
				renderingDeviceContext,
				i
			);
		}
		// else create lane
		else
		{
			// What type of lane?
			int type = 1; // Middle
			if (i == 1) type = 0; // left
			if (i == lanes) type = 2; // right

			col.object = CreateLane(
				renderingDevice,
				renderingDeviceContext,
				type
			);
		}

		// set the position of the current object to lane position offset
		col.object->transform.SetPosition(
			i * 5.0f,
			0,
			posZ
		);

		// add column to row
		row.push_back(col);
	}
}

// Create new lane
Soft351WorldObject* Soft351WorldRow::CreateLane(
	ID3D11Device* renderingDevice,
	ID3D11DeviceContext *renderingDeviceContext,
	int type
) 
{
	// create new world object
	Soft351WorldObject* worldObject = new Soft351WorldObject();
	// give it a lane mesh
	switch (type)
	{
	default:
	case 1: // middle
		worldObject->mesh = m_resources->GetMesh(L"lane_m");
		break;
	case 0: // left
		worldObject->mesh = m_resources->GetMesh(L"lane_l");
		break;
	case 2: // right
		worldObject->mesh = m_resources->GetMesh(L"lane_r");
		break;
	}
	// return object
	return worldObject;
}

// create side object
Soft351WorldObject* Soft351WorldRow::CreateSide(
	ID3D11Device* renderingDevice,
	ID3D11DeviceContext *renderingDeviceContext,
	int index
)
{
	// Create new world object
	Soft351WorldObject* worldObject = new Soft351WorldObject();
	// Give the object a mesh
	worldObject->mesh = m_resources->GetMesh(L"pavement");
	// is it the left?
	if (index == 0) {
		worldObject->transform.SetRotation(0, 180, 0);
	}
	// return object
	return worldObject;
}

// render a row
void Soft351WorldRow::Render(
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	XMMATRIX& viewMatrix,
	XMMATRIX& projectionMatrix
)
{
	// for each element in a row
	for (int i = 0; i < row.size(); i++) {
		// update its view and projection matrix
		row[i].object->SetProjectionMatrix(projectionMatrix);
		row[i].object->SetViewMatrix(viewMatrix);
		// render the mesh
		row[i].object->Render(
			device,
			deviceContext
		);
	}
}

// Update the row behavior
void Soft351WorldRow::Update(float fTime, float roadspeed) {
	// for each row element
	for (int i = 0; i < row.size(); i++) {
		// get its position
		XMFLOAT3 pos = row[i].object->transform.GetPosition();
		// update its position by road speed
		pos.z -= roadspeed * fTime;
		// set its new position
		row[i].object->transform.SetPosition(
			pos.x,
			pos.y,
			pos.z
		);
	}
}

// Release all objects before deletion
void Soft351WorldRow::Release() {
}

// Set the position for all elements in row
void Soft351WorldRow::SetPosition(
	float positionZ
)
{
	// for each element in row
	for (int i = 0; i < row.size(); i++) {
		// Get its position
		XMFLOAT3 position = row[i].object->transform.GetPosition();
		// udpate its Z position
		position.z = positionZ;
		// set its new position
		row[i].object->transform.SetPosition(
			position.x,
			position.y,
			position.z
		);
	}
}