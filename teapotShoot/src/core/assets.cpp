#include <assets.h>
Assets::Assets(){
	assets=std::map<std::string,LoadedObject>();
}
Assets& Assets::add(std::string name, LoadedObject obj){
	assets.insert(std::pair<std::string,LoadedObject>(name,obj));
	return *this;
}
Assets& Assets::add(std::string name, LoadedObject *obj){
	assets.insert(std::pair<std::string,LoadedObject>(name,*obj));
	return *this;
}
Assets& Assets::reload(){//this will mess with any internaly defined texturs, DO NOT CALL unless all assets are loaded from file
  for (auto const& asset : assets) {
	assets[(std::string)asset.first]=LoadedObject(((std::string)asset.first).c_str());
  }
	return *this;

}
LoadedObject* Assets::load(std::string filename){
	assets.insert(std::pair<std::string,LoadedObject>(filename,LoadedObject(filename)));
	return &(assets[filename]);
}
LoadedObject* Assets::get(std::string name){
	return &(assets[name]);
}
LoadedObject* Assets::operator[](std::string name){
	return &(assets[name]);
}


BoxAssets::BoxAssets(){
	assets=std::map<std::string,HitBox*>();
}
BoxAssets& BoxAssets::add(std::string name, HitBox *obj){
	assets.insert(std::pair<std::string,HitBox*>(name,obj));
	return *this;
}

HitBox* BoxAssets::get(std::string name){
	return (assets[name]);
}
HitBox* BoxAssets::operator[](std::string name){
	return (assets[name]);
}
