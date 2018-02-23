#include<hitbox.h>

bool HitBox::checkCollision(float x, float y, float z, float px,float py, float pz,float r){

	return ((x-px)*(x-px)+(y-py)*(y-py)+(z-pz)*(z-pz))<=(r+radius)*(r+radius);
}
/*
bool HitBox::checkCollision(HitBox& other){
	return checkCollision(x-other.x,y-other.y,z-other.z,other.geometry->radius);
}
bool HitBox::checkCollision(float px,float py, float pz,float r){
	bool ret=(_geometry->checkCollision(x-px,y-py,z-pz,r));//something is wrong with this line
	return ret;
}
HitBox& HitBox::changeGeometry(Geometry* geo){
	delete _geometry;
	_geometry=geo;
	return *this;
}

HitBox::~HitBox(){
	delete _geometry;
}





bool SphereBox::checkCollision(float x,float y,float z, float r){//only check like two spheres	
	return (x*x+y*y+z*z)<=r*r+radius*radius;
}*/
