#ifndef viewmanager_h
#define viewmanager_h
#ifdef MAIN
#define EXTERN
#define INIT = 0
#else
#define EXTERN extern
#define INIT
#endif
class View{
	public:
	View();
	float theta;
	float phi;
	float lookingX;
	float lookingY;
	float lookingZ;
	View& calcLooking(float distance=1);
};
EXTERN View view;
#endif
