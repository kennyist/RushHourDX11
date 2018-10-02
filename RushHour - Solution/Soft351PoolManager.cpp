#include "DXUT.h"
#include "Soft351PoolManager.h"

// -- Usefull links
// Vector:
// http://www.cplusplus.com/reference/vector/vector/

// Constructor and set initial values
Soft351PoolManager::Soft351PoolManager(
)
{
	m_lastNumLane = 0;
	m_pd3device = 0;
	m_pd3deviceContext = 0;
	m_resourceManager = 0;
	// Generate new rand seed
	srand(time(NULL));
}

// Deconstruction
Soft351PoolManager::~Soft351PoolManager()
{
}

// Setup and set external references
void Soft351PoolManager::Setup(
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	Soft351ResourceManager& resourceManager
)
{
	m_pd3device = device;
	m_pd3deviceContext = deviceContext;
	m_resourceManager = &resourceManager;
}

// Reset the world by deactivating all pool objects
void Soft351PoolManager::WorldReset() {
	// pickups
	for (int i = 0; i < pickupPool.size(); i++) {
		pickupPool[i].active = false;
	}

	// cars
	for (int i = 0; i < carPool.size(); i++) {
		carPool[i].active = false;
	}

	// world
	for (int i = 0; i < lanePool.size(); i++) {
		lanePool[i].active = false;
	}

	// Buildings
	for (int i = 0; i < buildingPool.size(); i++) {
		buildingPool[i].active = false;
	}
}

// Render all pool objects
void Soft351PoolManager::Render(
	XMMATRIX& viewMatrix,
	XMMATRIX& projectionMatrix
) {
	// Pickups
	for (int i = 0; i < pickupPool.size(); i++) {
		if (pickupPool[i].active) {
			pickupPool[i].SetProjectionMatrix(projectionMatrix);
			pickupPool[i].SetViewMatrix(viewMatrix);
			pickupPool[i].Render(
				m_pd3device,
				m_pd3deviceContext
			);
		}
	}

	// Cars
	for (int i = 0; i < carPool.size(); i++) {
		if (carPool[i].active) {
			carPool[i].SetProjectionMatrix(projectionMatrix);
			carPool[i].SetViewMatrix(viewMatrix);
			carPool[i].Render(
				m_pd3device,
				m_pd3deviceContext
			);
		}
	}

	// world lanes
	for (int i = 0; i < lanePool.size(); i++) {
		if (lanePool[i].active) {
			lanePool[i].Render(
				m_pd3device,
				m_pd3deviceContext,
				viewMatrix,
				projectionMatrix
			);
		}
	}

	// world buildings
	for (int i = 0; i < buildingPool.size(); i++) {
		if (buildingPool[i].active) {
			buildingPool[i].SetProjectionMatrix(projectionMatrix);
			buildingPool[i].SetViewMatrix(viewMatrix);
			buildingPool[i].Render(
				m_pd3device,
				m_pd3deviceContext
			);
		}
	}
}

// --- pickups
// Create new pickup
void Soft351PoolManager::CreatePickup(
	Soft351PickupObject::PickupType type,
	XMFLOAT3 position
)
{
	// Loop through all pickups
	for (int i = 0; i < pickupPool.size(); i++) {
		// if inactive object found, reset position and activate
		if (pickupPool[i].GetType() == type && !pickupPool[i].active) {
			pickupPool[i].transform.SetPosition(
				position.x,
				position.y,
				position.z
			);
			pickupPool[i].active = true;
			return; // exit method
		}
	}

	// if no deactivated object found create new pickup
	Soft351PickupObject obj;
	obj.SetType(type);
	obj.transform.SetPosition(
		position.x,
		position.y,
		position.z
	);
	obj.active = true;

	// give the pickup a mesh based on type
	switch (type) {
	default:
	case Soft351PickupObject::PickupType::Jump:
		obj.mesh = m_resourceManager->GetMesh(L"jumpPickup");
		break;
	case Soft351PickupObject::PickupType::Sheild:
		obj.mesh = m_resourceManager->GetMesh(L"shieldPickup");
		break;
	case Soft351PickupObject::PickupType::Life:
		obj.mesh = m_resourceManager->GetMesh(L"lifePickup");
		break;
	}

	// add pickup to pool
	pickupPool.push_back(obj);
}

// Remove pickup by index
void Soft351PoolManager::RemovePickup(
	int index
)
{
	// Get pickup by index and deactivate it
	pickupPool[index].active = false;
}

// --- Cars
// Create car
void Soft351PoolManager::CreateCar(
	XMFLOAT3 position
)
{
	// loop through pool
	for (int i = 0; i < carPool.size(); i++) {
		// if inactive object found, reset position and activate
		if (!carPool[i].active) {
			carPool[i].transform.SetPosition(
				position.x,
				position.y,
				position.z
			);

			carPool[i].active = true;
			return; // exit method
		}
	}

	int r = rand() % 6;

	// if no deactivated object found, create new car
	Soft351WorldObject car;
	car.transform.SetPosition(
		position.x,
		position.y,
		position.z
	);
	// Give it a mesh
	switch (r)
	{
	default:
	case 0:
		car.mesh = m_resourceManager->GetMesh(L"car01");
		break;
	case 1:
		car.mesh = m_resourceManager->GetMesh(L"car02");
		break;
	case 2:
		car.mesh = m_resourceManager->GetMesh(L"car03");
		break;
	case 3:
		car.mesh = m_resourceManager->GetMesh(L"car04");
		break;
	case 4:
		car.mesh = m_resourceManager->GetMesh(L"car05");
		break;
	case 5:
		car.mesh = m_resourceManager->GetMesh(L"car06");
		break;
	}
	
	car.active = true;

	// add car to pool
	carPool.push_back(car);
}

// Remove car by index
void Soft351PoolManager::RemoveCar(
	int index
)
{
	// Find car in pool and deactivate it
	carPool[index].active = false;
}

// world lanes
// Create new lane
void Soft351PoolManager::CreateLaneRow(
	float positionZ,
	int numLanes
)
{
	// Check the lane size matches last stored value
	if (numLanes != m_lastNumLane) {
		// if it doesnt set new value and clear the pool
		m_lastNumLane = numLanes;
		lanePool.clear();
	}

	// loop through the pool
	for (int i = 0; i < lanePool.size(); i++) {
		// if inactive object found, reset position and activate
		if (!lanePool[i].active) {
			lanePool[i].SetPosition(positionZ);
			lanePool[i].active = true;

			return;
		}
	}

	// If no deactivated object found, create new row
	Soft351WorldRow row;
	row.m_resources = m_resourceManager;
	row.CreateColumns(
		m_pd3device,
		m_pd3deviceContext,
		positionZ,
		numLanes
	);
	row.active = true;

	// Add row to pool
	lanePool.push_back(row);
}

// Remove lane by index
void Soft351PoolManager::RemoveLane(
	int index
)
{
	// find lane by index and deactivate it
	lanePool[index].active = false;
}

// Get spawn position for new lane
float Soft351PoolManager::GetLaneSpawnPos() {
	float pos = 0.0f;

	// loop through pool
	for (int i = 0; i < lanePool.size(); i++) {
		// if pool object is active
		if (lanePool[i].active) {
			// Set new pos if greater that stored
			if (lanePool[i].row[0].object->transform.GetPosition().z > pos) {
				pos = lanePool[i].row[0].object->transform.GetPosition().z;
			}
		}
	}

	// return position
	return pos;
}

// --- DEBUG:
// Get dead count
int Soft351PoolManager::DeadCount() {
	int i = 0;

	for (int j = 0; j < pickupPool.size(); j++) {
		if (!pickupPool[j].active) i++;
	}

	return i;
}

// --- Buildings
// Create new building
void Soft351PoolManager::CreateBuilding(
	XMFLOAT3 position
)
{
	// loop through pool
	for (int i = 0; i < buildingPool.size(); i++) {
		// if inactive object found, reset position and activate
		if (!buildingPool[i].active) {
			buildingPool[i].transform.SetPosition(
				position.x,
				position.y,
				position.z
			);

			buildingPool[i].active = true;
			return; // exit method
		}
	}

	// Generate new random number for random mesh select
	int r = rand() % 4;

	// if no deactivated object found, create new car
	Soft351WorldObject building;
	building.transform.SetPosition(
		position.x,
		position.y,
		position.z
	);

	switch (r)
	{
	default:
	case 0:
		building.mesh = m_resourceManager->GetMesh(L"building01");
		break;
	case 1:
		if (rand() % 2 == 0) {
			building.mesh = m_resourceManager->GetMesh(L"building02");
		}
		else {
			building.mesh = m_resourceManager->GetMesh(L"building01");
		}
		break;
	case 2:
		building.mesh = m_resourceManager->GetMesh(L"building03");
		break;
	case 3:
		building.mesh = m_resourceManager->GetMesh(L"building04");
		break;
	}
	
	buildingPool.push_back(building);
}

// Remove building by index
void Soft351PoolManager::RemoveBuilding(
	int index
)
{
	buildingPool[index].active = false;
}

// Get spawn position based on furthest building
float Soft351PoolManager::GetBuildingSpawnPoint() {
	float pos = -30.0f;

	// loop through pool
	for (int i = 0; i < buildingPool.size(); i++) {
		// if pool object is active
		if (buildingPool[i].active) {
			// Set new pos if greater than stored
			if (buildingPool[i].transform.GetPosition().z > pos) {
				pos = buildingPool[i].transform.GetPosition().z;
			}
		}
	}

	// return position
	return pos;
}