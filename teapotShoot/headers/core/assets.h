#ifndef ASSETS_H
#define ASSETS_H

#include <LoadedObject.h>
#include<hitbox.h>
#include <map>
#include <string>
//TODO rework this with templates
class Assets{
	public:
		std::map<std::string,LoadedObject> assets;
		Assets();
		Assets& add(std::string name, LoadedObject obj);
		Assets& add(std::string name, LoadedObject *obj);
		Assets& reload();
		LoadedObject* load(std::string filename);
		LoadedObject* get(std::string name);
		LoadedObject* operator[](std::string name);
};
class BoxAssets{
	public:
		std::map<std::string,HitBox*> assets;
		BoxAssets();
		BoxAssets& add(std::string name, HitBox *obj);
		HitBox* get(std::string name);
		HitBox* operator[](std::string name);
};

#endif
