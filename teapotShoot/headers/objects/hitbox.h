#ifndef HITBOX_H
#define HITBOX_H
class HitBox{
	public:

		float ox; //offset if nessesarry
		float oy;
		float oz;
		float radius;

		HitBox(float r, float iox=0, float ioy=0, float ioz=0):radius(r), ox(iox), oy(ioy), oz(ioz){}
		bool checkCollision(float x, float y, float z, float px, float py, float pz, float radius=0);
};
/*I wish I could get his working but it is too anoying, I will come back to it later if I have time, for now just use the sphirical Hitbox
class Geometry{
	public:
		float radius;//you can approximate collisions using fake spheres before doing real check
		Geometry(float r):radius(r){}
		virtual bool checkCollision(float x, float y, float z, float radius)=0;//pure virtual, also assume object is centered arround (0,0,0)
};

class NulBox : public Geometry{
	public:

		NulBox(float r): Geometry(r){};
		bool checkCollision(float x,float y,float z, float radius=0){return false;}
};

class SphereBox : public Geometry{
	public:

		SphereBox(float r): Geometry(r){};
		bool checkCollision(float x,float y,float z, float radius=0);
};

class HitBox{
	private:
		Geometry* _geometry;
	public:
		const float& x; //attatch to parrent xyz
		const float& y;
		const float& z;
		float ox; //offset if nessesarry
		float oy;
		float oz;
		const Geometry*& geometry=(const Geometry*&)_geometry;//refference to _geometry, this may have strange problems
		HitBox(Geometry *geo, float &ix, float &iy,  float &iz, float iox=0, float ioy=0, float ioz=0):_geometry(geo), x(ix), y(iy), z(iz), ox(iox), oy(ioy), oz(ioz){}
		bool checkCollision(float px, float py, float pz, float radius=0);//use x,y and z to shift these to (0,0,0) before passing call on
		bool checkCollision( HitBox& other);
		HitBox& changeGeometry(Geometry* geo);
		~HitBox();

};
*/
#endif
