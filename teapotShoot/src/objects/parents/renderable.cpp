#include<renderable.h>
#include<physics.h>
Renderable::Renderable(LoadedObject *obj,float ix, float iy, float iz, float iTheta, float iPhi):model(obj){
	//model=obj;
	dX=ix;
	dY=iy;
	dZ=iz;
	dTheta=iTheta;
	dPhi=iPhi;
}
Renderable& Renderable::draw(){
	this->draw(0,0,0,0,0);
	return *this;
}
Renderable& Renderable::draw(float x, float y, float z, float theta, float phi){
	glPushMatrix();
	glTranslatef(x+dX, y+dY, z+dZ);
	glRotatef((theta+dTheta-90)*180./PI, 0.0f, 1.0f, 0.0f);
	glRotatef((phi+dPhi)*180./PI, 0.0f, 0.0f, 1.0f);
	model->display();
	glPopMatrix();
	return *this;
}
