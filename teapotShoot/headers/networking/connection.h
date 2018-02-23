#ifndef CONNECTION_H
#define CONNECTION_H
#include <sys/types.h>

#include <unistd.h>
#include <netdb.h>

#include <sys/socket.h>
#include <queue>
#include <mutex>
#include <thread>
#include <netinet/ip.h>
#include <arpa/inet.h>
#define FLOAT_REZ 0.0000001

struct pkg{
	pkg(){type=0;}
	__uint8_t type;
};
struct playerPkg: public pkg{
	__uint32_t ID;//ID of the player
	__uint16_t texture;//just in case
	__uint8_t state;
	float x;
	float y;
	float z;
	float vx;
	float vy;
	float vz;
	float pvx;
	float pvy;
	float pvz;
	float theta;
	float phi;
	float speed;//just in case
	
};
struct projPkg: public pkg{
	__uint32_t ID;	//ID of the projectile
	__uint32_t owner;	//ID of the projectile owner
	__uint16_t texture;//just in case
	__uint8_t state;
	float x;
	float y;
	float z;
	float vx;
	float vy;
	float vz;
	float speed;//just in case

};

struct reqPkg: public pkg{
	__uint16_t spec;//specify the type of request
};
struct ackPkg: public pkg{
	__uint16_t spec;//specify the type of acknowledged package
};
struct respPkg: public pkg{
	__uint16_t spec;//specify the type of response
	__uint32_t data;//responded data
};
class Connection{
	private:
		std::thread listener;
		struct sockaddr_in sad;
		struct hostent* entry;
		struct in_addr **addr_list;
		struct in_addr* c_addr;
		int skt;
		bool closeThread=false;
	public:

		bool connected=false;
		

  		std::shared_ptr<pkg> cPak;
   		std::queue<std::shared_ptr<pkg>> pks;

		Connection();		
		
		int connect(std::string ip, std::string port);
		Connection& leave();
		
		Connection& lock();
    		Connection& unlock();

		static Connection& reader(Connection*);
		Connection& startReader();
		Connection& readerL();

		Connection& sendMe();
		Connection& newProjectile();
		Connection& sync();
		Connection& join();
		Connection& handle();

		Connection& offline(int);
		//it does not matter how these work, as long as they are perfict inverses and maintain 
		//FLOAT_REZ resolution durring conversion
		__int32_t qPack(float);
		float fUnpack(__int32_t);
};
#endif
