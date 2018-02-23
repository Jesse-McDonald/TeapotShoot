#include <physics.h>
#include <var.h>
#include <world.h>
#include <sys/time.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>
unsigned long long getMS(){
	timeval tv;
	gettimeofday(&tv,NULL);
	return  (unsigned long long)(tv.tv_sec) * 1000 +
    (unsigned long long)(tv.tv_usec) / 1000;
}

void movement(){
	unsigned long long start=getMS();
	unsigned long long sleepStart=getMS();
	unsigned long long last_run=0;
	unsigned long long longest=0;
	while(true){//forever run the movment loop, probiably want to change this to something in var	
		start=getMS();

		float secondsSince=(start-sleepStart+last_run)/1000.;//convert the total time the last loop took to run to seconds for physics calculations, theoreticaly same as 1/fps	
		{//creating block for organizational purposes
		connection->handle();

			world.players[0].buttonCalc();
			int deadProjectiles=0;
			int deadPlayers=0;
			for(auto &player :world.players){
				player.move(secondsSince);
				deadPlayers+=player.state;
			}
			for(auto &projectile :world.projectiles){
				if(projectile.state==0){				
					projectile.move(secondsSince);
				
					for(auto &player :world.players){
						if (player.hitbox.checkCollision(player.x, player.y, player.z, projectile.x,projectile.y,projectile.z,projectile.hitbox.radius)&&projectile.attatched_id!=player.ID){
							projectile.state=1;
							player.state=1;
							player.y=100;
							connection->sendMe();
						}
					}
				}else{
					deadProjectiles++;
				}	
			}
			if(deadPlayers*2>world.players.size()){
				cleanLock.lock();
				for(int i=world.players.size()-1;i>=0;i--){
					if(world.players[i].state){
						world.players[i]=world.players[world.players.size()-1];
						world.players.pop_back();
					}
				}
				cleanLock.unlock();
			}
			if(deadProjectiles*2>world.projectiles.size()){
				cleanLock.lock();
				for(int i=world.projectiles.size()-1;i>=0;i--){
					if(world.projectiles[i].state){
						world.projectiles[i]=world.projectiles[world.projectiles.size()-1];
						world.projectiles.pop_back();
					}
				}
				cleanLock.unlock();

			}
			
			
		}
		sleepStart=getMS();
		last_run=sleepStart-start;
		longest=std::max(longest,last_run);
		usleep((int)std::max((float)0,((1/fps)-last_run)*1000000));
	}
	return;//yah.... we never do this ;)	
}
