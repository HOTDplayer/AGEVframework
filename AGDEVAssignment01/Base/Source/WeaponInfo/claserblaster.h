#pragma once

#include"./WeaponInfo.h"

class CLaserBlaster :
	public CWeaponInfo
{
public:
	CLaserBlaster() {};
	virtual ~CLaserBlaster() {};

	//Initialise this instance to default values 
	void Init();
	//Discharge this weapon
	void Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);

};