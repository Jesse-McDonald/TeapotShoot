#include<entity.h>
Entity::Entity(float ix, float iy, float iz, float itheta ,float iphi , LoadedObject *obj, float iDX, float iDY, float iDZ, float iDTheta, float iDPhi):Moveable(ix,iy,iz,0,0,0),Renderable(obj,iDX,iDZ,iDTheta,iDPhi),hitbox(0){
	x=ix;
	y=iy;
	z=iz;
	model=obj;
	vx=0;
	vz=0;
	vy=0;
	theta=itheta;
	phi=iphi;
	dX=iDX;
	dY=iDY;
	dZ=iDZ;
	dTheta=iDTheta;
	dPhi=iDPhi;
	state=0;
}
Entity& Entity::draw(){
	if (state==0){
		Renderable::draw(x,y,z,theta,phi);
	}
	return *this;
}
