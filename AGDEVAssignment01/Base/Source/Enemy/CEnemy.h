#pragma once
#include "../GenericEntity.h"
#include "../GroundEntity.h"
#include "../Waypoint/Waypoint.h"

#include <vector>
using namespace std;

class Mesh;

class CEnemy :public GenericEntity
{
public:
	CEnemy(void);
	virtual ~CEnemy();

	enum STATE
	{
		STATE_NONE = 0,
		STATE_TOOFULL,
		STATE_FULL,
		STATE_HUNGRY,
		STATE_DEAD,
	};

	void Init(void);
	//Reset this player instance to default
	void Reset(void);
	//Set Position
	void SetPos(const Vector3& pos);
	//Set target
	void SetTarget(const Vector3& target);
	//Set Up
	void SetUp(const Vector3& up);
	//Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	//Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);

	//Get position
	Vector3 GetPos(void) const;
	//Get target
	Vector3 GetTarget(void) const;
	//Get Up
	Vector3 GetUp(void) const;
	//Get the terrain for the player info
	GroundEntity* GetTerrain(void);
	//Get next WayPoint for this CEnemy
	CWaypoint* GetNextWayPoint(void);
	STATE currstate;
	//Update
	void Update(double dt = 0.0333f);

	//Constrain the position within the borders
	void Constrain(void);
	//Render
	void Render(void);

protected:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

	//Vector containing IDs of Waypoints
	vector<int> listOfWaypoints;
	//Current ID of Waypoint
	int m_iWayPointIndex;

	double m_dSpeed;
	double m_dAcceleration;
};