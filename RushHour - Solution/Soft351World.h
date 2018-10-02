#ifndef SOFT351WORLD_H
#define SOFT352WORLD_H

#include "Soft351WorldRow.h"
#include "Soft351ResourceManager.h"
#include "Soft351PickupObject.h"
#include "Soft351PoolManager.h"
#include "Soft351VMathHelper.h"
#include <time.h>

// World manager controlling spawning and updating of objects 
class Soft351World
{
public:
	// --- Constructor and deconstruction
	Soft351World(
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		Soft351ResourceManager& resourceManager	// Asset manager
	);
	~Soft351World();

	// --- Public methods
	// Setup and initialize world to lane count
	void SetupWorld(int numberOfLanes);
	// Reset the world (Clear all objects)
	void ResetWorld();
	// Render all world objects
	void Render(
		XMMATRIX& viewMatrix,
		XMMATRIX& projectionMatrix
	);
	// Update all object behavior
	void Update(float frameTime);
	// Set the game difficulty
	void SetDifficulty(int difficulty);
	// Return the players current score
	int GetScore();
	// Get the nearest car from target position
	Soft351Transform* GetNearestCar(XMFLOAT3 position);
	// Get collided pickup (returns -1 if none found)
	int CollidedPickup(XMFLOAT3 position, bool remove = false);
	// Remove the collided car from game
	void DeleteCollidedCar(XMFLOAT3 position);

	// delete test
	int poolobjects();

private:
	// --- Private Methods
	// Create new pickup
	void CreatePickup(float positionX);
	// Create new lane row
	void CreateRow(float posZ);
	// Create new row of cars
	void SpawnCars();
	// Remove cards from map
	void DeleteCars();
	// Remove pickups from map
	void DeletePickups();
	// Create new building in scene
	void CreateBuilding();
	// Remove buildings from scene
	void DeleteBuilding();
	
	// --- Private Variables
	// Pool manager
	Soft351PoolManager objectPool;
	// Distance between lanes length
	float m_worldObjectDistanceZ;
	// Distance between lanes width
	float m_worldObjectDistanceX;
	// Car start position
	float m_carSpawnStartPos;
	// Distance between cars spawning
	float m_distanceBetweenCars;
	// spawn distance counter
	float m_spawnDistanace;
	// Number of lanes
	int m_lanes;
	// Current car speed
	float m_carSpeed;
	// Current road speed
	float m_roadSpeed;
	// speed increase over time
	float m_speedIncrease;
	// Distance traveled by the player
	float m_distanceTraveled;
	// Car spawning chance
	int m_spawnChance;
	// Pickup spawn chance
	int m_pickupChance;
	// Player score
	float m_score;
	// Buildings size (Always cube)
	float m_buildingDistance;
	// Building distance counter
	float m_buidlingDistanceCount;
	// Collision distance
	float m_collisionDistance;

	//
	ID3D11Device* m_renderDevice;
	ID3D11DeviceContext* m_renderContext;
	Soft351ResourceManager*  m_resources;
};

#endif // !SOFT351WORLD_H