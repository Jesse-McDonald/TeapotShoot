#include<math.h>
#include<GL/glut.h>
#include<viewmanager.h>
#include<physics.h>
#include<var.h>
#include<limits>
#include<world.h>
//#include<iostream>
void fire(){
	//world.projectiles.push_back(world.players[0].shoot());//we no longer do this because we have to wait for the projectile from the server
	world.players[0].shoot();
	return;
}
void mousemove(int x, int y){
	mStatus.x=x;
	mStatus.y=y;
	if(mStatus.state==1){
		int dx = height/2 - x;
		int dy = width/2 - y;
		view.theta += dx/360.0;
		view.phi += dy/360.0;
		if(view.phi > PI/2) view.phi = 1.56802;//.49911626773 ish * pi ie less than pi/2 but not very, but enough that in calcLooking sin(pi/2) is never calculated, if it ever is the view will flip 180 and is quite dissorenting, this value is the last value to not flip like this in experimentation
		if(view.phi < -PI/2) view.phi = -1.56802;

		if(dx || dy ){
			glutWarpPointer(height/2, width/2);
		}
		world.players[0].theta=view.theta;
		world.players[0].phi=view.phi;
	}
}

void mousefire(int x, int y, int a, int b){
	if (mStatus.state==1){
		static bool flip=1;
		if (flip)
			fire();
		flip=flip^1;
	}
}

