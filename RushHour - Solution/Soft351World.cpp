#include "DXUT.h"
#include "Soft351World.h"

// Usefull links:
// random:
// http://www.cplusplus.com/reference/cstdlib/rand/
// Time: 
// http://www.cplusplus.com/reference/ctime/time/

// Constructor and setup of internal variables
Soft351World::Soft351World(
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	Soft351ResourceManager& resourceManager // Asset manager
)
{
	// Setup pool manager
	objectPool.Setup(device, deviceContext, resourceManager);
	// Internal value setup
	m_renderDevice = device;
	m_renderContext = deviceContext;
	m_resources = &resourceManager;
	// Set default values
	m_collisionDistance = 3.8f;
	m_worldObjectDistanceZ = 7.0f;
	m_worldObjectDistanceX = 5.0f;
	m_distanceBetweenCars = 10.0f;
	m_buildingDistance = 30.0f;
	m_buidlingDistanceCount = 0.0f;
	m_lanes = 5;
	m_carSpeed = 6.0f;
	m_distanceTraveled = 0.0f;
	m_speedIncrease = 0.1f;
	m_spawnDistanace = 0.0f;
	m_carSpawnStartPos = 0.0f;
	m_roadSpeed = 10.0f;
	m_spawnChance = 50;
	m_pickupChance = 10;
	m_score = 0;
	// Generate new rand seed
	srand(time(NULL));
}

// Deconstruction
Soft351World::~Soft351World(
)
{
	
}

// Reset world
void Soft351World::ResetWorld() {
	// Set all game objects deactivated in pool manager
	objectPool.WorldReset();
}

// Set the game difficulty settings
void Soft351World::SetDifficulty(
	int difficulty
) 
{
	switch (difficulty)
	{
	default:
	case 0: // Easy
		m_distanceBetweenCars = 14.0f;
		m_carSpeed = 6.0f;
		m_roadSpeed = 10.0f;
		m_speedIncrease = 0.1f;
		m_spawnChance = 50;
		m_pickupChance = 10;
		break;

	case 1: // Medium
		m_distanceBetweenCars = 12.0f;
		m_carSpeed = 7.0f;
		m_roadSpeed = 11.0f;
		m_speedIncrease = 0.12f;
		m_spawnChance = 60;
		m_pickupChance = 8;
		break;

	case 2: // Hard
		m_distanceBetweenCars = 10.0f;
		m_carSpeed = 9.0f;
		m_roadSpeed = 13.0f;
		m_speedIncrease = 0.15f;
		m_spawnChance = 70;
		m_pickupChance = 5;
		break;
	}
}

// Setup the world 
void Soft351World::SetupWorld(
	int numberOfLanes
)
{
	// Set the number of lanes
	m_lanes = numberOfLanes;

	// Create new lanes
	for (int i = 0; i < 40; i++) {
		CreateRow(i * m_worldObjectDistanceZ);

		// --- Car spawning
		// Update spawn distance for pres spawning cars
		m_spawnDistanace += m_worldObjectDistanceZ;
		// Pre spawn cars after set distance
		if (i > 5) {
			// Only if spawn distance is greater that car distance
			if (m_spawnDistanace > m_distanceBetweenCars) {
				m_carSpawnStartPos = m_worldObjectDistanceZ * i;
				// set distance to 0
				m_spawnDistanace = 0;
				// Spawn cars here
				SpawnCars();
			}
		}
	}

	for (int i = 0; i < 10; i++) {
		// --- Building spawning
		CreateBuilding();
	}

	// Set car spawn distance to last spawned lane distance
	m_carSpawnStartPos = m_worldObjectDistanceZ * 40;
	// Spawn cars here
	// SpawnCars();
	// reset player score
	m_score = 0;
}

// Render all world objects
void Soft351World::Render(
	XMMATRIX& viewMatrix,
	XMMATRIX& projectionMatrix
)
{
	// Send to object pool to render every active object
	objectPool.Render(viewMatrix, projectionMatrix);
}

// Debug: Get dead object pool count
int Soft351World::poolobjects() {
	return objectPool.DeadCount();
}

// Update the behavior of each world object
void Soft351World::Update(float fTime) {
	// --- Method Vars
	XMFLOAT3 position;

	// For each lane in the object pool
	for (int i = 0; i < objectPool.lanePool.size(); i++) {
		// if lane is active
		if (objectPool.lanePool[i].active) {
			// Update lane position
			objectPool.lanePool[i].Update(
				fTime,
				m_roadSpeed
			);

			// if lane is behind the player out of view
			if (objectPool.lanePool[i].row[0].object->transform.GetPosition().z
				< -m_worldObjectDistanceZ)
			{
				// remove the lane
				objectPool.RemoveLane(i);

				// create new lane at end of the road
				objectPool.CreateLaneRow(objectPool.GetLaneSpawnPos() + m_worldObjectDistanceZ, m_lanes);
			}
		}
	}

	// for each car in object pool
	for (int j = 0; j < objectPool.carPool.size(); j++) {
		// if car is active
		if (objectPool.carPool[j].active) {
			// get the car position
			position = objectPool.carPool[j].transform.GetPosition();
			// update car position using car speed
			position.z -= m_carSpeed * fTime;
			// set the new position
			objectPool.carPool[j].transform.SetPosition(
				position.x,
				position.y,
				position.z
			);
		}
	}

	// For each pickup in the object pool
	for (int t = 0; t < objectPool.pickupPool.size(); t++) {
		// if pickup is active
		if (objectPool.pickupPool[t].active) {
			// Get the pickup position
			position = objectPool.pickupPool[t].transform.GetPosition();
			// update the pickup position using car speed 
			position.z -= m_carSpeed * fTime;
			// set the new position
			objectPool.pickupPool[t].transform.SetPosition(
				position.x,
				position.y,
				position.z
			);
			// Do the rotation behavior
			objectPool.pickupPool[t].Update(fTime);
		}
	}

	// For each building in the object pool
	for (int t = 0; t < objectPool.buildingPool.size(); t++) {
		// if building is active
		if (objectPool.buildingPool[t].active) {
			// Get the building position
			position = objectPool.buildingPool[t].transform.GetPosition();
			// update the pickup position using road speed 
			position.z -= m_roadSpeed * fTime;
			// set the new position
			objectPool.buildingPool[t].transform.SetPosition(
				position.x,
				position.y,
				position.z
			);
		}
	}

	// Update distance since last car spawn
	m_spawnDistanace += m_carSpeed * fTime;
	// if distance is greater than spawn distance
	if (m_spawnDistanace > m_distanceBetweenCars) {
		// Spawn new car
		SpawnCars();
		// set spawn distance to 0
		m_spawnDistanace = 0;
	}

	// Delete any cars that have gone off screen
	DeleteCars();
	// Delete any pickups that have gone off screen
	DeletePickups();
	// Delete any buildings that have gone off screen
	DeleteBuilding();

	// update speed and distance
	m_distanceTraveled = m_carSpeed * fTime;
	m_carSpeed += m_speedIncrease * fTime;
	m_roadSpeed += m_speedIncrease * fTime;
	// score
	m_score += m_carSpeed * fTime;
}

// Return the players score
int Soft351World::GetScore() {
	return (int)m_score;
}

// Create new lane row
void Soft351World::CreateRow(float posZ) {
	// create new lane in object pool
	objectPool.CreateLaneRow(
		posZ,
		m_lanes
	);
}

// spawn new cars
void Soft351World::SpawnCars
(
)
{
	// spawned count
	int spawned = 0;
	// for lane count
	for (int i = 0; i < m_lanes; i++) {
		// new random number
		int r = rand() % 100;

		// if spawn chance is grater than random 
		// and spawned less that lanes -1
		if (m_spawnChance > r && spawned < m_lanes - 1) {
			// set new spawn position
			XMFLOAT3 position = XMFLOAT3(
				m_worldObjectDistanceX + (i * m_worldObjectDistanceX),
				0,
				m_carSpawnStartPos
			);
			// create new car in pool
			objectPool.CreateCar(position);
			// increase spawned count
			spawned++;
		}
		// If car did not spawn try and spawn a a pickup instead
		else
		{
			// Generate new random number
			int pickupRand = rand() % 100;
			// if pickup chance less that random number
			if (pickupRand < m_pickupChance) {
				// generate new spawn position
				float posX = m_worldObjectDistanceX + (i * m_worldObjectDistanceX);
				// Spawn pickup in object pool
				CreatePickup(posX);
			}
		}
	}
}

// Spawn new pickup
void Soft351World::CreatePickup(float positionX) {
	// Generate new random number
	int pickupRand = rand() % 100;
	// Generate new spawn position
	XMFLOAT3 position = XMFLOAT3(
		positionX,
		0,
		m_carSpawnStartPos
	);	

	// if rand number under 10
	if (pickupRand < 10) 
	{
		// create new life pickup in pool
		objectPool.CreatePickup(
			Soft351PickupObject::PickupType::Life,
			position
		);
	}
	// if rand number below 40
	else if (pickupRand < 40) 
	{
		// spawn shield pickup
		objectPool.CreatePickup(
			Soft351PickupObject::PickupType::Sheild,
			position
		);
	}
	// if rand number below 80
	else if (pickupRand < 80) 
	{
		// spawn jump pickup
		objectPool.CreatePickup(
			Soft351PickupObject::PickupType::Jump,
			position
		);
	}
	// else don't spawn new pickup
}

// Remove cars from scene
void Soft351World::DeleteCars() {
	// if pool is empty return
	if (objectPool.carPool.size() <= 0) return;

	// for each car in pool
	for (int i = 0; i < objectPool.carPool.size(); i++) {
		//if car behind the player 
		if (
			objectPool.carPool[i].transform.GetPosition().z <
			-m_distanceBetweenCars
			)
		{
			// disable the car
			objectPool.RemoveCar(i);
		}
	}
}

// delete pickup
void Soft351World::DeletePickups() {
	// if pool is empty return
	if (objectPool.pickupPool.size() <= 0) return;

	// for each pickup in pool
	for (int i = 0; i < objectPool.pickupPool.size(); i++) {
		// if pickup is active and behind the player
		if (
			objectPool.pickupPool[i].active &&
			objectPool.pickupPool[i].transform.GetPosition().z < -m_distanceBetweenCars
			)
		{
			// remove pickup from scene
			objectPool.RemovePickup(i);
		}
	}
}

// Find the nearest car to target position
Soft351Transform* Soft351World::GetNearestCar(XMFLOAT3 position)
{
	Soft351Transform* transform = new Soft351Transform;

	// for each car in object pool
	for (int i = 0; i < objectPool.carPool.size(); i++) {
		// if car is active
		if (objectPool.carPool[i].active) {
			// if transform null set to this transform
			if (!transform) {
				transform = &objectPool.carPool[i].transform;
			}

			// Get distance from target and this cars position
			float distA = DistanceFloat3(
				position, 
				objectPool.carPool[i].transform.GetPosition()
			);
			// get distance from target and saved transform
			float distB = DistanceFloat3(
				position, 
				transform->GetPosition()
			);

			// if distanceA less that DistanceB
			if (distA < distB) {
				// update transform
				transform = &objectPool.carPool[i].transform;
			}
		}
	}

	// return transform
	return transform;
}

// Delete collided car
void Soft351World::DeleteCollidedCar(XMFLOAT3 position) {
	// for each car in the pool
	for (int i = 0; i < objectPool.carPool.size(); i++) {
		// if car is active
		if (objectPool.carPool[i].active) {
			// if car is on same lane as target
			if (OnSameLane(
				position,
				objectPool.carPool[i].transform.GetPosition()
			))
			{
				// get the distance
				float dist = DistanceFloat3(
					position, 
					objectPool.carPool[i].transform.GetPosition()
				);
				// if distance less than collision distance
				if (dist <= m_collisionDistance) {
					// remove car from scene
					objectPool.RemoveCar(i);
				}
			}
		}
	}
}

// Get collided pickup
int Soft351World::CollidedPickup(XMFLOAT3 position, bool remove) {
	// set return value and set to null
	int pickup = -1;

	// for each pickup in object pool
	for (int i = 0; i < objectPool.pickupPool.size(); i++) {
		// if pickup is active
		if (objectPool.pickupPool[i].active) {
			// if pickup is on the same lane
			if (OnSameLane(
				position,
				objectPool.pickupPool[i].transform.GetPosition()
			))
			{
				// get the distance from pickup and target
				float dist = DistanceFloat3(
					position, 
					objectPool.pickupPool[i].transform.GetPosition()
				);

				// if distance less than collision distance
				if (dist <= m_collisionDistance) {
					// return value = pickup type
					pickup = objectPool.pickupPool[i].GetType();

					// if remove is true
					if (remove) {
						// remove pickup from scene
						objectPool.RemovePickup(i);
					}
				}
			}		
		}
	}

	// return pickup index
	return pickup;
}

// --- Buildings

void Soft351World::CreateBuilding() {
	// Setup spawn position for left building
	XMFLOAT3 pos = XMFLOAT3(
		-(m_buildingDistance / 2) - (m_worldObjectDistanceX / 2) ,
		0.0f,
		objectPool.GetBuildingSpawnPoint() + m_buildingDistance
	);

	// Spawn left building
	objectPool.CreateBuilding(pos);

	// get right spawn building distance
	float toAdd = (m_lanes * m_worldObjectDistanceX) +
		(m_worldObjectDistanceX / 2) +
		(m_buildingDistance / 2) +
		m_worldObjectDistanceX;
	// set X
	pos.x = toAdd;
	// spawn right building
	objectPool.CreateBuilding(pos);

	// reset building distance count
	m_buidlingDistanceCount = 0;
}

// delete building
void Soft351World::DeleteBuilding() {
	// if pool is empty return
	if (objectPool.buildingPool.size() <= 0) return;
	bool deleted = false;

	// for each pickup in pool
	for (int i = 0; i < objectPool.buildingPool.size(); i++) {
		// if pickup is active and behind the player
		if (
			objectPool.buildingPool[i].active &&
			objectPool.buildingPool[i].transform.GetPosition().z < -(m_buidlingDistanceCount + 30.0f)
			)
		{
			deleted = true;
			// remove pickup from scene
			objectPool.RemoveBuilding(i);
		}
	}

	if (deleted) {
		CreateBuilding();
	}
}