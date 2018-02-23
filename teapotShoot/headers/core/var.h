#ifndef VAR_H
#define VAR_H
#ifdef MAIN
#define EXTERN
#define INIT = 0
#else
#define EXTERN extern
#define INIT
#endif
#include <world.h>
#include <mutex>
#include <connection.h>
struct Keys {
	char forward;
	char backward;
	char stepright;
	char turnright;
	char stepleft;
	char turnleft;
	char fire;

};
struct Mouse{
	int x;
	int y;
	char state;
};
EXTERN Mouse mStatus;
EXTERN Keys kStatus;
EXTERN int width INIT,height INIT;
EXTERN float fps;
EXTERN World world;
EXTERN std::mutex cleanLock;
extern Connection* connection;
#endif

