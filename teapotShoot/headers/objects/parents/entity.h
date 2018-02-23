#ifndef ENTITY_H
#define ENTITY_H
#include<renderable.h>
#include<moveable.h>
#include <hitbox.h>

class Entity: public Renderable, public Moveable{
	public:
	HitBox hitbox;//this is set to nul, set to something else if you actually need a bounding box
	unsigned int ID;

	float speed;
	char state; //0 normal, 1 dead, 
	float theta,phi; //heading directions
	Entity(float ix, float iy, float iz, float itheta ,float iphi , LoadedObject *obj, float iDX=0, float iDY=0, float iDZ=0, float iDTheta=0, float iDPhi=0);
	virtual Entity& draw() override;
};
#endif
