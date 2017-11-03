#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"
#include"../Source/Laser.h"
#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double _dt)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}
	CheckForCollision();
	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Render();
	}
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity)
{
	entityList.push_back(_newEntity);
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		delete *findIter;
		findIter = entityList.erase(findIter);
		return true;
	}
	// Return false if not found
	return false;
}

// Constructor
EntityManager::EntityManager()
{
}

// Destructor
EntityManager::~EntityManager()
{
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{
	//check if this object is overlapping that object
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		||
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
	{
		return true;
	}
	//check if that object is overlapping this object
	if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
		||
		((thisMaxAABB >= thatMinAABB) && (thatMaxAABB <= thatMaxAABB)))
	{
		return true;
	}
	//check if this object is within that object
	if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
		&&
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
		return true;
	//check if that object is within this object
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		&&
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
		return true;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	//Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	//get the minaabb and maxaabb for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	//if radius of bounding sphere of TisEntity plus Radius of bounding sphere of ThatEntity is
	//greater than thew distance sqyared between the 2 reference points of the 2 entities,
	//then in  could mean tha they are colliding with each other
	if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) > DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()))
	{
		return true;
	}
	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	//Get the colliders for the 2 enttities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	//Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thisCollider->GetMaxAABB();

	//check for overlap
	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	//if AABB collision check fails,then we need to check the other corners of the bounding boxes to
	//do more collison checks with other points on each bounding box
	Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);

	//check for overlap
	if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	return false;
}

bool EntityManager::GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 & Hit)
{
	if ((fDst1*fDst2) >= 0.0f)
		return false;
	if (fDst1 == fDst2)
		return false;
	Hit = P1 + (P2 - P1)*(-fDst1 / (fDst2 - fDst1));
	return true;
}

bool EntityManager::CheckLineSegmentPlane(Vector3 line_start, Vector3 line_end, Vector3 minAABB, Vector3 maxAABB, Vector3 & hit)
{
	if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x, line_start, line_end, hit) && InBox(hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - minAABB.y, line_end.y - minAABB.y, line_start, line_end, hit) && InBox(hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - minAABB.z, line_end.z - minAABB.z, line_start, line_end, hit) && InBox(hit, minAABB, maxAABB, 3))
		|| (GetIntersection(line_start.x - maxAABB.x, line_end.x - maxAABB.x, line_start, line_end, hit) && InBox(hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - maxAABB.y, line_end.y - maxAABB.y, line_start, line_end, hit) && InBox(hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - maxAABB.z, line_end.z - maxAABB.z, line_start, line_end, hit) && InBox(hit, minAABB, maxAABB, 3)))
		return true;

	return false;
}

bool EntityManager::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
	if (Axis == 1 && Hit.z > B1.z&&Hit.z<B2.z&&Hit.y>B1.y&&Hit.y < B2.y)return true;
	if (Axis == 2 && Hit.z > B1.z&&Hit.z<B2.z&&Hit.x>B1.x&&Hit.x < B2.x)return true;
	if (Axis == 3 && Hit.x > B1.x&&Hit.x<B2.x&&Hit.y>B1.y&&Hit.y < B2.y)return true;
	return false;
}

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(void)
{
	//check for collision
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	colliderThisEnd = entityList.end();
	for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		//check if this entity is a claser type
		if ((*colliderThis)->GetIsLaser())
		{
			//dynamic cast it to a claser class
			CLaser* thisEntity = dynamic_cast<CLaser*>(*colliderThis);

			//check for collision with another collider class
			colliderThatEnd = entityList.end();
			int counter = 0;
			for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					Vector3 hitposition = Vector3(0, 0, 0);

					//get the minAABB and maxAABB for(*colliderthat)
					CCollider *thatCollider = dynamic_cast<CCollider*>(*colliderThat);
					Vector3 thatMinAABB = (*colliderThat)->GetPosition() + thatCollider->GetMinAABB();
					Vector3 thatMaxAABB = (*colliderThat)->GetPosition() + thatCollider->GetMaxAABB();

					if (CheckLineSegmentPlane(thisEntity->GetPosition(),
						thisEntity->GetPosition() - thisEntity->GetDirection()*thisEntity->GetLength(),
						thatMinAABB, thatMaxAABB,
						hitposition) == true)
					{
						(*colliderThis)->SetIsDone(true);
						(*colliderThat)->SetIsDone(true);
					}
				}
			}
		}
		else if ((*colliderThis)->HasCollider())
		{
			//this object was derived from a ccolider class,then it will have collision detection methods
			//ccolider *thiscollider = dynamic_cast<CCollider*>(*colliderThis);
			EntityBase*thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

			//check for collision with another collider class
			colliderThatEnd = entityList.end();
			int counter = 0;
			for (colliderThat = colliderThis; colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					//this obejct was derived from a ccollider class,then it will have collision detection methods
					EntityBase*thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
					if (CheckSphereCollision(thisEntity, thatEntity) == true)
					{
						if (CheckAABBCollision(thisEntity, thatEntity) == true)
						{
							thisEntity->SetIsDone(true);
							thatEntity->SetIsDone(true);
						}
					}
				}

			}
		}
	}
	return false;
}
