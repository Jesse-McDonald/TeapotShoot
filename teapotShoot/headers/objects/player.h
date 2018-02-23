#ifndef PLAYER_H
#define PLAYER_H
#include<entity.h>
#include <projectile.h>

class Player: public Entity{
	public:
	unsigned int ID;
	float pvx=0;//player controled velocity, this is set by buttons
	float pvy=0;//think of these as velocity offsets
	float pvz=0;
	float speed;
	char state; //0 normal, 1 dead, 
	float theta,phi; //heading directions
	float oTheta,oPhi;//old heading
	Player& move(float dt);
	Player(float ix, float iy, float iz, float itheta ,float iphi , LoadedObject *obj, float iDX=0, float iDY=0, float iDZ=0, float iDTheta=0.5, float iDPhi=0);//apparently the handle on the tea pot I am using is 30 degrees off axis... who knew
	Player& buttonCalc();
	//Projectile shoot();
	//Projectile& shoot(Projectile&);
	Player& shoot();
	Player& draw();
};
#endif
