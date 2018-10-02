#include "Soft351PlayerObject.h"


// Construct player object and set initial values
Soft351PlayerObject::Soft351PlayerObject()
	: Soft351WorldObject()
{
	meshShield = new Soft351Mesh();
	m_targetPosition = transform.GetPosition().x;
	m_lanes = 3;
	m_sideSpeed = 100.0f;
	m_controlType = 0;
	m_collisionDistance = 3.8f;
	ResetPlayer();
}

// Deconstruction
Soft351PlayerObject::~Soft351PlayerObject()
{
}

// Reset player to default values
void Soft351PlayerObject::ResetPlayer() {
	m_lives = 1;
	m_jumps = 1;
	m_sheildActive = false;
	m_jumping = false;
	m_jumpingUp = false;
	m_jumpHeight = 12.0f;
	m_sheildActiveTime = 15.0f;
	m_sheildTimer = 0.0f;
}

// Render the players shield
void Soft351PlayerObject::RenderSheild(
	ID3D11Device  *pRenderingDevice,
	ID3D11DeviceContext *pImmediateContext)
{
	XMMATRIX matWorldlocal = transform.GetWorldMatrix();
	XMMATRIX matViewLocal = GetViewMatrix();
	XMMATRIX matProjectionLocal = GetProjectionMatrx();

	XMMATRIX matWorldViewProjectionLocal = matWorldlocal * matViewLocal * matProjectionLocal;

	// Make sure shield IS active
	if (m_sheildActive) {
		meshShield->Render(
			pRenderingDevice,
			pImmediateContext,
			matWorldlocal,
			matWorldViewProjectionLocal
		);
	}
}

// Setup the players control method
void Soft351PlayerObject::SetupControl(int lanes, int controlType) {
	// Set internal values
	m_lanes = lanes - 1;
	m_controlType = controlType;

	// Set player start position
	float startpos = 5.0f + (5.0f * (m_lanes / 2));
	m_targetPosition = startpos;
	transform.SetPosition(startpos, 0, 0);
}

// Update the players behavior per frame
void Soft351PlayerObject::Update(Soft351Input& input, float frameTime)
{
	// Read the players input 
	switch (m_controlType)
	{
	default:
	case 0: // If control type quick or default
		// if left array or A key set target position to lane left
		if (input.GetKeyDown(DIK_LEFTARROW) || input.GetKeyDown(DIK_A)) {
			m_targetPosition -= 5.0f;
		}
		// if right array or D key set target position to lane right
		if (input.GetKeyDown(DIK_RIGHTARROW) || input.GetKeyDown(DIK_D)) {
			m_targetPosition += 5.0f;
		}
		break;

	case 1: // if control type smooth
		// if A or left arrow key, move player left smoothly
		if (input.GetKey(DIK_LEFTARROW) || input.GetKey(DIK_A)) {
			m_targetPosition -= 25.0f * frameTime;
		}
		// if D or right array, move player right smoothly
		if (input.GetKey(DIK_RIGHTARROW) || input.GetKey(DIK_D)) {
			m_targetPosition += 25.0f * frameTime;
		}
		break;
	case 2: // if control type mouse, move player by mouse movement
		m_targetPosition += input.MouseAccelerationXDamp();
		break;
	}
	
	// if player has jumps and is not jumping
	// and
	// player pressed W or Up arrow or clicks left mouse
	if ((m_jumps > 0 && !m_jumping) 
		&& 
			(
			input.GetKeyDown(DIK_W) || 
			input.GetKeyDown(DIK_UPARROW) || 
			input.MouseClick(0)
			)
		) 
	{
		// set jump pressed to true
		jumpPressed = true;
		// set player jumping
		m_jumping = true;
		// Set player jumping up to true
		m_jumpingUp = true;
		// set target jump heignt
		m_jumpHeightTemp = m_jumpHeight;
		// remove jump from player
		m_jumps--;
	}

	// If player is jumping run jump behavior
	if (m_jumping) {
		Jump(frameTime);
	}

	// if player has shield active
	if (m_sheildActive) {
		// add time to timer
		m_sheildTimer += 1.0f * frameTime;
		// if timer greater than max time, remove shield
		if (m_sheildTimer > m_sheildActiveTime) {
			m_sheildActive = false;
		}
	}

	// do movement behavior
	Move(frameTime);
}

// Check target position is within bounds
void Soft351PlayerObject::MoveBounds(float &position)
{
	// Get max value position can be
	float max = 5.0f + (5.0f * m_lanes);
	// if position is greater than max, set to max
	if (position > max) position = max; 
	// if position less that minimum, set to minimum
	if (position < 5.0f) position = 5.0f;
}

// Move player locked to frame 
void Soft351PlayerObject::Move(float frameTime) {
	// check target position is within bounds
	MoveBounds(m_targetPosition);

	// if current position is not equal to target
	if (transform.GetPosition().x != m_targetPosition) {
		// get current position
		XMFLOAT3 pos = transform.GetPosition();

		// if current is less than target
		if (pos.x < m_targetPosition) {
			// increase position by speed and frame time
			pos.x += m_sideSpeed * frameTime;

			// if target position within 0.01f of target lock to target
			// added to stop movement flickering 
			if (pos.x >(m_targetPosition - 0.01f)) {
				pos.x = m_targetPosition;
			}
		}

		// if current is greater than target
		if (pos.x > m_targetPosition) {
			// decrease position by speed and frame time
			pos.x -= m_sideSpeed * frameTime;

			// if target is within 0.01f of target, lock to target
			// Added to stop movement flickering
			if (pos.x < (m_targetPosition + 0.01f)) {
				pos.x = m_targetPosition;
			}
		}

		// Finally set the players new position
		transform.SetPosition(pos.x, pos.y, pos.z);
	}
}

// Has the player collided with target
bool Soft351PlayerObject::Collision(Soft351Transform* inTransform)
{
	// Is the player on the same lane as target?
	if (!OnSameLane(transform.GetPosition(), inTransform->GetPosition())) {
		// did not collide
		return false;
	}

	// is the player within collision distance of target?
	if (DistanceFloat3(transform.GetPosition(), inTransform->GetPosition()) < m_collisionDistance) {
		// did collide
		return true;
	}

	// Did not collide
	return false;
}





// Add a pickup to the player
void Soft351PlayerObject::AddPickup(
	int typeIndex
)
{
	// convert int to pickup enum type
	Soft351PickupObject::PickupType  type = static_cast<Soft351PickupObject::PickupType>(typeIndex);
	
	// Switch based on pickup enum type
	switch (type)
	{
		// if jump, add a jump to the player with limiting
	case Soft351PickupObject::PickupType::Jump:
		if (m_jumps < 2) m_jumps++;
		break;
		// if life, add a life to the player with limiting
	case Soft351PickupObject::PickupType::Life:
		if (m_lives < 1) m_lives++;
		break;
		// if shield, enable shield and reset timer
	case Soft351PickupObject::PickupType::Sheild:
		m_sheildActive = true;
		m_sheildTimer = 0.0f;
		break;
		// if anything else, do nothing 
	default:
		break;
	}
}

// Return the number of lives the player has
int Soft351PlayerObject::GetLives() {
	return m_lives;
}

// Return the number of Jumps the player has
int Soft351PlayerObject::GetJumps() {
	return m_jumps;
}

// Run the jump behavior
void Soft351PlayerObject::Jump(float fTime) {
	// Get the players current position
	XMFLOAT3 pos = transform.GetPosition();
	// Reduce the temp jump height holder each frame
	m_jumpHeightTemp -= 6 *fTime;

	// For down movement 
	float jumpDownFloat;
	// reverse the value
	jumpDownFloat = m_jumpHeight - m_jumpHeightTemp;

	// if jumping up
	if (m_jumpingUp) {
		// While jump height is greater than value
		if (m_jumpHeightTemp > 4.0f) {
			// add height to the player
			pos.y += m_jumpHeightTemp * fTime;
		}
		else {
			// once height reached start jump down
			m_jumpingUp = false;
			// reset jump height temp
			m_jumpHeightTemp = m_jumpHeight;
		}
	}
	// if falling
	else 
	{
		// while player not grounded
		if (transform.GetPosition().y > 0.0f) {
			// remove height from player
			pos.y -= jumpDownFloat * fTime;
		}
		// if grounded
		else {
			// player not jumping anymore
			m_jumping = false;
			// reset player to ground level
			pos.y = 0.0f;
		}
	}

	// Set the new position
	transform.SetPosition(
		pos.x,
		pos.y,
		pos.z
	);
}

// Remove a life from the player
void Soft351PlayerObject::RemoveLife() {
	if (m_lives > 0) {
		m_lives--; // only remove if count is greater than 0
	}
}

// Disable the shield on the player
void Soft351PlayerObject::DissableSheild() {
	m_sheildActive = false;
}

// Is the shield currently active?
bool Soft351PlayerObject::SheildActive() {
	return m_sheildActive;
}

// Return how long the shield has left
float Soft351PlayerObject::ShieldTimeRemaining() {
	return m_sheildActiveTime - m_sheildTimer;
}