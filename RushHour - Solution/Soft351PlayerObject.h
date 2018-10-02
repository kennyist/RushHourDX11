#ifndef SOFT351PLAYEROBJECT_H
#define SOFT351PLAYEROBJECT_H

#include "Soft351WorldObject.h"
#include "Soft351Mesh.h"
#include "SDKmesh.h"
#include "Soft351Input.h"
#include "Soft351PickupObject.h"
#include "Soft351VMathHelper.h"

// player object with world object as parent
// inherits mesh abilities from worldobject
// inherits transform abilities from gameobject
class Soft351PlayerObject : public Soft351WorldObject
{
public:
	// --- Constructor and deconstruction
	Soft351PlayerObject();
	~Soft351PlayerObject();		

	// --- Public methods
	// Update the players behaviour by frame
	void Update(Soft351Input& input, float frameTime);
	// Reset the player to defaults
	void ResetPlayer();
	// Set the control method and number of lanes
	void SetupControl(int lanes, int controlType);
	// Check if player has collided with object
	bool Collision(Soft351Transform* inTransform);
	// Add a pickup to the player
	void AddPickup(int typeIndex);
	// Return the number of lives the player has
	int GetLives();
	// Return the number of jumps the player has
	int GetJumps();
	// Remove a life from the player
	void RemoveLife();
	// Is the players shield currently active?
	bool SheildActive();
	// Dissable the players shield
	void DissableSheild();
	// Render the players shield
	void RenderSheild(
		ID3D11Device *pRenderingDevice,
		ID3D11DeviceContext *pImmediateContext
	);
	// Get sheild active time
	float ShieldTimeRemaining();

	// --- Public Variables:
	// the players shield mesh
	Soft351Mesh* meshShield;
	// Has the player press jump
	bool jumpPressed = false;

private:
	// --- Private methods:
	// Move the player to target location
	void Move(float fTime);
	// Do jump behavior
	void Jump(float fTIme);
	// Is target position within bounds
	void MoveBounds(float &position);

	// --- Private vars
	// PLayer life count
	int m_lives;
	// Player jump count
	int m_jumps;
	// IS the shield active
	bool m_sheildActive;
	// is the player jumping
	bool m_jumping;
	// Is the jump going up (used in jump behavior)
	bool m_jumpingUp;
	// Target height of the jump ability
	float m_jumpHeight;
	// Temp height used in jump behavior
	float m_jumpHeightTemp;
	// How long should the shield stay active
	float m_sheildActiveTime;
	// Current timer of the active shield
	float m_sheildTimer;
	// How fast does the player move between lanes
	float m_sideSpeed;
	// Number of lanes on the level
	int m_lanes;
	// Current control type
	int m_controlType;
	// Target move position
	float m_targetPosition;
	// Collision distance
	float m_collisionDistance;
};

#endif // !SOFT351PLAYEROBJECT_H

