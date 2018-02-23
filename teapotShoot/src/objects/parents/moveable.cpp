#include<moveable.h>
#include<physics.h>
Moveable::Moveable(float ix,float iy,float iz,float ivx,float ivy,float ivz){
	x=ix;
	y=iy;
	z=iz;
	vx=ivx;
	vy=ivy;
	vz=ivz;
};
Moveable& Moveable::move(float dt){
	x+=vx*dt;
	y+=vy*dt+gravity*0.5*G*dt*dt; 
	z+=vz*dt;
	vy-=gravity*G*dt;
	

	return *this;
};
