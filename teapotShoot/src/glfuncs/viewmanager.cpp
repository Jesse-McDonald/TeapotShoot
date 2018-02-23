#include<viewmanager.h>
#include<math.h>
#include <physics.h>

View& View::calcLooking(float distance){
	float r=cos(phi);
	lookingX=distance*r*sin(theta);
	lookingY=distance*sin(phi);
	lookingZ=distance*r*cos(theta);
	
	return *this;
}
View::View(){
	theta=0;
	phi=0;
	calcLooking(1);
}
