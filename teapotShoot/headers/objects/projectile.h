#ifndef PROJECTILE_H
#define PROJECTILE_H
#include<entity.h>
class Projectile: public Entity{
	public:
	unsigned int ID;
	unsigned int attatched_id;
	float theta,phi; //heading directions
	float speed;
	float timeToLive;
	Projectile(float ix, float iy, float iz, float itheta ,float iphi , LoadedObject *obj, float iDX=0, float iDY=0, float iDZ=0, float iDTheta=0, float iDPhi=0);
	Projectile& draw() override;
	Projectile& move(float time) override;
};
#endif
