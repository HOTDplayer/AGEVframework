#include"CLaserBlaster.h"

void CLaserBlaster::Init()
{
	//Call the parent's Init method
	CWeaponInfo::Init();

	//The number of ammunition in a magazine for this weapon
	magRounds = 5;
	//the maximum number of ammunition for this magazine for this weapon 
	maxMagRounds = 5;
	//the current total number of rounds currently cxarried by this player
	totalRounds = 10;
	//the max total number of rounds currently carried by this player
	maxTotalRounds = 10;

	//The time between shots
	timeBetweenShots = 0.1667;
	//the elapsed time (between shots)
	elapsedTime = 0.0;
	//Boolean flag to indicate if weapon can fire now
	bFire = true;
}

void CLaserBlaster::Discharge(Vector3 position, Vector3 target, CPlayerInfo * _source)
{
	if (bFire)
	{
		//if there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			Vector3 _direction = (target - position).Normalized();
			//create a laser with a lser mesh.The length is 10.0f,mesh is also set at 10.0f
			CLaser* aLaser = Create::Laser("laser",
				position,
				_direction,
				10.0f,
				2.0f,
				100.0f,
				_source);

			aLaser->SetCollider(true);
			aLaser->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			magRounds--;
		}
	}
}
