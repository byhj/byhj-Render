#ifndef COLLISION_H
#define COLLISION_H

#include <windows.h>
#include <DirectXMath.h> 
using namespace DirectX;
#include <vector>

/************************************New Stuff****************************************************/
// This is the scale of our scene, which we will use to find "a very small distance"
// If this is set too small, you will notice the camera "stick" to the geometry once in a while
// Just play with it and use whatever works best for your application
const float unitsPerMeter = 100.0f;

// The gravity's velocity vector
XMVECTOR gravity = XMVectorSet(0.0f, -0.2f, 0.0f, 0.0f);

// Polygon Soup
std::vector<XMFLOAT3> collidableGeometryPositions;
std::vector<DWORD> collidableGeometryIndices;

struct CollisionPacket{
	// Information about ellipsoid (in world space)
	XMVECTOR ellipsoidSpace;
	XMVECTOR w_Position;
	XMVECTOR w_Velocity;

	// Information about ellipsoid (in ellipsoid space)
	XMVECTOR e_Position;
	XMVECTOR e_Velocity;
	XMVECTOR e_normalizedVelocity;

	// Collision Information
	bool foundCollision;
	float nearestDistance;
	XMVECTOR intersectionPoint;
	int collisionRecursionDepth;
};

// Collision Detection and Response Function Prototypes
XMVECTOR CollisionSlide(CollisionPacket& cP,			// Pointer to a CollisionPacket object (expects ellipsoidSpace, w_Position and w_Velocity to be filled)
						std::vector<XMFLOAT3>& vertPos,						// An array holding the polygon soup vertex positions
						std::vector<DWORD>& indices);						// An array holding the polygon soup indices (triangles)

XMVECTOR CollideWithWorld(CollisionPacket& cP,			// Same arguments as the above function
						  std::vector<XMFLOAT3>& vertPos,
						  std::vector<DWORD>& indices);

bool SphereCollidingWithTriangle(CollisionPacket& cP,	// Pointer to a CollisionPacket object	
								 XMVECTOR &p0,										// First vertex position of triangle
								 XMVECTOR &p1,										// Second vertex position of triangle
								 XMVECTOR &p2,										// Third vertex position of triangle 
								 XMVECTOR &triNormal);								// Triangle's Normal

// Checks if a point (inside the triangle's plane) is inside the triangle
bool checkPointInTriangle(const XMVECTOR& point, const XMVECTOR& triV1,const XMVECTOR& triV2, const XMVECTOR& triV3);

// Solves the quadratic eqation, and returns the lowest root if equation is solvable, returns false if not solvable
bool getLowestRoot(float a, float b, float c, float maxR, float* root);
/*************************************************************************************************/



#endif
