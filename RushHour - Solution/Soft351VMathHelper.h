#ifndef _SOFT351VMATHHELPER_H
#define _SOFT351VMATHHELPER_H

#include <DirectXMath.h>
using namespace DirectX;

// --- Usefull links:
// vector distances
// https://www.engineeringtoolbox.com/distance-relationship-between-two-points-d_1854.html


// Is the player on the same lane as target
inline bool OnSameLane(
	XMFLOAT3& player,
	XMFLOAT3& target
)
{
	// Get the current lane of target
	int lane = (target.x - 5.0f) / 5.0f;
	// Get the distance to the edge of lane
	float vergeDist = 5.0f / 2;

	// if the player is within target and verge distance
	if (player.x > (target.x - vergeDist) &&
		player.x < (target.x + vergeDist))
	{
		// collided!
		return true;
	}

	// did not collide
	return false;
}

// Get the distance between two points
inline float DistanceFloat3(
	XMFLOAT3& pointA,
	XMFLOAT3& pointB
)
{
	float distance =
		sqrt(
		(pointA.x - pointB.x) * (pointA.x - pointB.x) +
			(pointA.y - pointB.y) * (pointA.y - pointB.y) +
			(pointA.z - pointB.z) * (pointA.z - pointB.z)
		);

	return distance;
}

#endif // !_SOFT351VMATHHELPER_H
