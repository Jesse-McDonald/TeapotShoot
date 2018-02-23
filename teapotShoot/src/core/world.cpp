#include<world.h>

World::World(){
	assets=Assets();
	players=std::vector<Player>();	
	players.reserve(10);
}
