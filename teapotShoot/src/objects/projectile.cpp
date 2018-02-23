#include <projectile.h>
#include <math.h>
Projectile::Projectile(float ix, float iy, float iz, float itheta ,float iphi , LoadedObject *obj, float iDX, float iDY, float iDZ, float iDTheta, float iDPhi):Entity(ix,iy,iz,itheta,iphi,obj,iDX,iDY,iDZ,iDTheta,iDPhi){
	hitbox=HitBox(.1);	
	speed=20;//you can custom set this if you want, but you have to figure out when and how
	timeToLive=1;//same with this
	gravity=false;
	float r=cos(iphi);
	vx=speed*r*sin(itheta);
	vy=speed*sin(iphi);
	vz=speed*r*cos(itheta);
	state=0;
}
Projectile& Projectile::draw(){
	if(state==0){
		Entity::draw();
	}
	return *this;
}
#include <iostream>
Projectile& Projectile::move(float time){

	if (timeToLive>0){	
		timeToLive-=time;
		Entity::move(time);
	}else{
		state=1;
	}
	if (y<=-1){//temporarily hack in floor handling
		vy=-vy;
		y=-1;
	}
	return *this;
}
