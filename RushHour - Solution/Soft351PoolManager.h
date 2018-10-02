#ifndef _SOFT351POOLMANAGER_H
#define _SOFT351POOLMANAGER_H

#include <vector>
#include "Soft351ResourceManager.h"
#include "Soft351WorldObject.h"
#include "Soft351PickupObject.h"
#include "Soft351WorldRow.h"
#include <time.h>

// Object pooling manager
class Soft351PoolManager
{
public:
	// --- Constructor and deconstruction 
	Soft351PoolManager();
	~Soft351PoolManager();

	// --- Public methods
	// Setup and initialize the pool manager
	void Setup(
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		Soft351ResourceManager& resourceManager
	);
	// Reset the world
	void WorldReset();
	// Render all the active pool objects
	void Render(
		XMMATRIX& viewMatrix,
		XMMATRIX& projectionMatrix
	);
	// DEBUG get number of dead objects
	int DeadCount();
	// Create a new pickup
	void CreatePickup(
		Soft351PickupObject::PickupType type,	// Pickup type
		XMFLOAT3 position						// start position
	);
	// Remove pickup by index
	void RemovePickup(int index);
	// Create new car
	void CreateCar(
		XMFLOAT3 position	// Start position
	);
	// Remove car by index
	void RemoveCar(
		int index
	);
	// Create a new world lane row
	void CreateLaneRow(
		float positionZ,	// Start position
		int numLanes		// Number of lanes
	);
	// Remove lane by index
	void RemoveLane(int index);
	// Get the spawn position for new lanes
	float GetLaneSpawnPos();

	// --- Buildings
	// Create new building object
	void CreateBuilding(
		XMFLOAT3 position
	);
	// Remove building from scene
	void RemoveBuilding(
		int index
	);
	// Get spawn position based on furthest object
	float GetBuildingSpawnPoint();

	// --- Public Variables
	// Lane object pool
	std::vector<Soft351WorldRow> lanePool;
	// Car object pool
	std::vector<Soft351WorldObject> carPool;
	// Pickup object pool
	std::vector<Soft351PickupObject> pickupPool;
	// Building object pool
	std::vector<Soft351WorldObject> buildingPool;

private:
	// --- Private vars
	// Last number of lanes on start
	int m_lastNumLane;
	ID3D11Device* m_pd3device;
	ID3D11DeviceContext* m_pd3deviceContext;
	// Asset resource manager
	Soft351ResourceManager* m_resourceManager;
};

#endif // !_SOFT351POOLMANAGER_H

