#ifndef MOVEABLE_H
#define MOVEABLE_H

class Moveable{
	public:
	Moveable(float ix=0,float iy=0,float iz=0,float ivx=0,float ivy=0,float ivz=0);
	float x,y,z;//position 
	float vx,vy,vz;//velocity
	bool gravity=true;
	virtual Moveable& move(float time);
};

#endif
