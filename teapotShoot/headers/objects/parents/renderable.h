#ifndef RENDERABLE_H
#define RENDERABLE_H
#include <GL/gl.h>
#include <LoadedObject.h>
#include <time.h>
class Renderable{
	public:
	float dX,dY,dZ;//Display position
	float dTheta, dPhi;
	LoadedObject* model;
	Renderable(LoadedObject* obj,float ix=0, float iy=0, float iz=0, float iTheta=0, float iPhi=0);
	virtual Renderable& draw();
	virtual Renderable& draw(float x, float y, float z, float theta=0, float phi=0);
};
#endif
