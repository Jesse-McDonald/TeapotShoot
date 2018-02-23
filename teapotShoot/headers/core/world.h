#ifndef WORLD_H
#define WORLD_H

#include <player.h>
#include <projectile.h>
#include <vector>
#include <assets.h>
#include <connection.h>
class World{
	public:
		World();

		std::vector<Player> players;
		std::vector<Projectile> projectiles;
		Assets assets;
};
#endif

