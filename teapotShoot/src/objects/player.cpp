#include <player.h>
#include <var.h>
#include <math.h>
#include <projectile.h>
#include <world.h>
#include <connection.h>
Player::Player(float ix, float iy, float iz, float itheta ,float iphi , LoadedObject *obj, float iDX, float iDY, float iDZ, float iDTheta, float iDPhi):Entity(ix,iy,iz,itheta,iphi,obj,iDX,iDY,iDZ,iDTheta,iDPhi){
	hitbox=HitBox(1);	
	oTheta=theta;
	oPhi=phi;
	speed=20;//you can custom set this if you want, but you have to figure out when and how
	gravity=true;
	state=0;
}
Player& Player::move(float dt){
	Moveable::move(dt);
	if (y<=0){//temporarily hack in floor handling
		vy=0;
		y=0;
	}
	x+=pvx*dt;
	y+=pvy*dt;
	z+=pvz*dt;
	return *this;
};

Player& Player::buttonCalc(){

	
	int tpvz=0;
	int tpvx=0;	
	if (kStatus.forward){
		tpvx+=speed*sin(theta);
		tpvz+=speed*cos(theta);
	}
	if (kStatus.backward){
		tpvx-=speed*sin(theta);
		tpvz-=speed*cos(theta);
	}
	if (kStatus.stepright){
		tpvz+=speed*sin(theta);
		tpvx-=speed*cos(theta);

	}
	if (kStatus.stepleft){
		tpvz-=speed*sin(theta);
		tpvx+=speed*cos(theta);

	}
	if(tpvz!=pvz||tpvx!=pvx||oTheta!=theta||oPhi!=phi){//only send player to server if something has changed that the server cant calculate for its self
		oTheta=theta;
		oPhi=phi;
		pvz=tpvz;
		pvx=tpvx;//we also commit changes here because they must be commited before sending and conveniently wont need to be updated if they didnt change		
		connection->sendMe();
	}
	
	
	return *this;
}
/*//old shoot implimentation, kept for refference
Projectile Player::shoot(){
	Projectile temp(x,y,z,theta,phi,world.assets.get("projectile"));
	temp.attatched_id=ID;
	return temp;
}

Projectile& Player::shoot(Projectile& temp){
	temp=Projectile(x,y,z,theta,phi,world.assets.get("projectile"));
	temp.attatched_id=ID;
	return temp;
}*/
Player& Player::shoot(){
	if(state==0){
		connection->newProjectile();
	}
	return *this;
}
Player& Player::draw(){
	if (state==0){
		Entity::draw();
	}
	return *this;
}
