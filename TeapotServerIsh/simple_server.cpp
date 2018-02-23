#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/ip.h>
#include<stdio.h>
#include<unistd.h>
#include<iostream>
#include<vector>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#define FLOAT_REZ 0.0000001

//simple trusting server, just track each player for later echo and projectile response, but does no verification
using namespace std;
std::mutex IDLock;
std::mutex sendAllLock;
__int32_t qPack(float in){
	return (__int32_t) (in/(float)FLOAT_REZ);
	
}
class Player;
inline ssize_t swrite(Player *player,const void * data,size_t count);
inline ssize_t sread(Player *player,void * place,size_t count);
float fUnpack(__int32_t in){
	return (float)in*(float)FLOAT_REZ;

}
class Player;
__uint32_t maxID=1;
__uint32_t newID(){
	IDLock.lock();
	int ret=maxID;
	cout<<ret<<endl;
	maxID++;
	IDLock.unlock();
	return ret;
}
struct pkg{
	pkg(){type=0;}
	__uint8_t type;
};
struct playerPkg: public pkg{
	__uint32_t ID;//ID of the player
	__uint16_t texture;//just in case
	__uint8_t state;
	__uint32_t x;
	__uint32_t y;
	__uint32_t z;
	__uint32_t vx;
	__uint32_t vy;
	__uint32_t vz;
	__uint32_t pvx;
	__uint32_t pvy;
	__uint32_t pvz;
	__uint32_t theta;
	__uint32_t phi;
	__uint32_t speed;//just in case
	bool inclusive=true;//not part of the network, this tells send all if it should ever send this as a me package, false for do not send
	
};
struct projPkg: public pkg{
	__uint32_t ID;	//ID of the projectile
	__uint32_t owner;	//ID of the projectile owner
	__uint16_t texture;//just in case
	__uint8_t state;
	__uint32_t x;
	__uint32_t y;
	__uint32_t z;
	__uint32_t vx;
	__uint32_t vy;
	__uint32_t vz;
	__uint32_t speed;//just in case

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


void spinUp(Player * self);
void sendAll(pkg *sendPkg);
vector<Player*> players;
class Player{
	public:
	int client_fd;
	bool lockState;
	bool connected=true;
	playerPkg playerData;
	std::thread reader;
	Player(int fd){
		client_fd=fd;
		playerData.ID=newID();

		//reader=std::thread(spinUp,this);//the maybe not working way
		reader=std::thread(&Player::read,this);
	}
	Player& shoot(){
			projPkg *newProj=new projPkg();
			newProj->type=2;
			newProj->x=playerData.x;
			newProj->y=playerData.y;
			newProj->z=playerData.z;
			newProj->speed=qPack(20);
				
			float r=cos(fUnpack(playerData.phi));
			cout<<"theta "<<fUnpack(playerData.theta)<<endl;
			cout<<"phi "<<fUnpack(playerData.phi)<<endl;
			cout<<"r "<<r<<endl;
			cout<<"X vel "<<qPack(fUnpack(newProj->speed)*r*sin(fUnpack(playerData.theta)));
			cout<<"y vel "<<qPack(fUnpack(newProj->speed)*sin(fUnpack(playerData.phi)));
			cout<<"z vel "<<qPack(fUnpack(newProj->speed)*r*cos(fUnpack(playerData.theta)));
			newProj->vx=qPack(fUnpack(newProj->speed)*r*sin(fUnpack(playerData.theta)));
			newProj->vy=qPack(fUnpack(newProj->speed)*sin(fUnpack(playerData.phi)));
			newProj->vz=qPack(fUnpack(newProj->speed)*r*cos(fUnpack(playerData.theta)));
			newProj->ID=newID();
			newProj->owner=playerData.ID;
			newProj->texture=2;//ignored right now, but we should probiably standardize textures some time
			newProj->state=0;
			respProj(newProj->ID);
			sendAll(newProj);
		return *this;
	}
	Player& lock(){
		while(lockState){
			for(int i=rand()%100;i>0;i--){
				sin(i);
			}		
		}
		lockState=true;
	}
	Player& unlock(){
		lockState=false;
	}
	Player& respProj(__uint32_t id){
		lock();
		char i8=5;
		__uint16_t ui16=htons(2);	
		__uint32_t ui32=htonl(id);
		swrite(this,&i8,1);
		swrite(this,&ui16,2);
		swrite(this,&ui32,4);
		unlock();		
		return *this;	
	}
	Player& sendMe(){
		lock();
		char i8=6;
		__uint32_t ui32;
		__uint16_t ui16;		
		swrite(this,&i8,1);
		ui32 =htonl(playerData.ID);
		swrite(this,&ui32,4);
		ui16=htons(playerData.texture);
		swrite(this,&ui16 ,2);
		swrite(this,&(playerData.state),1);
		ui32 =htonl(playerData.x);
		swrite(this,&ui32 ,4);
		ui32 =htonl(playerData.y);
		swrite(this,&ui32 ,4);
		ui32 =htonl(playerData.z);
		swrite(this,&ui32 ,4);	
		ui32 =htonl(playerData.vx);
		swrite(this,&ui32 ,4);
		ui32 =htonl(playerData.vy);
		swrite(this,&ui32 ,4);
		ui32 =htonl(playerData.vz);
		swrite(this,&ui32 ,4);
		ui32 =htonl(playerData.pvx);
		swrite(this,&ui32 ,4);
		ui32 =htonl(playerData.pvy);
		swrite(this,&ui32 ,4);
		ui32 =htonl(playerData.pvz);
		swrite(this,&ui32 ,4);
		ui32 =htonl(playerData.theta);
		swrite(this,&ui32 ,4);
		ui32 =htonl(playerData.phi);
		swrite(this,&ui32 ,4);
		ui32 =htonl(playerData.speed);
		swrite(this,&ui32 ,4);

		unlock();		
		return *this;
	}
	
	Player& sendPlayer(playerPkg sendPkg){
		lock();
		__uint32_t ui32;
		__uint16_t ui16;
		swrite(this,&(sendPkg.type),1);
		ui32 =htonl(sendPkg.ID);
		swrite(this,&ui32 ,4);
		ui16= htons(sendPkg.texture);
		swrite(this,&ui16,2);
		swrite(this,&(sendPkg.state),1);
		ui32 =htonl(sendPkg.x);
		swrite(this,&ui32 ,4);
		ui32 =htonl(sendPkg.y);
		swrite(this,&ui32 ,4);
		ui32 =htonl(sendPkg.z);
		swrite(this,&ui32 ,4);
		ui32 =htonl(sendPkg.vx);
		swrite(this,&ui32 ,4);
		ui32 =htonl(sendPkg.vy);
		swrite(this,&ui32 ,4);
		ui32 =htonl(sendPkg.vz);
		swrite(this,&ui32 ,4);
		ui32 =htonl(sendPkg.pvx);
		swrite(this,&ui32 ,4);
		ui32 =htonl(sendPkg.pvy);
		swrite(this,&ui32 ,4);
		ui32 =htonl(sendPkg.pvz);
		swrite(this,&ui32 ,4);
		ui32 =htonl(sendPkg.theta);
		swrite(this,&ui32 ,4);
		ui32 =htonl(sendPkg.phi);
		swrite(this,&ui32 ,4);
		ui32 =htonl(sendPkg.speed);
		swrite(this,&ui32 ,4);
		unlock();
		return *this;		
	}
	Player& sendProj(projPkg sendPkg){
		lock();
__uint32_t ui32;
		__uint16_t ui16;
		swrite(this,&(sendPkg.type),1);

		ui32 =htonl(sendPkg.ID);
		swrite(this,&ui32 ,4);

		ui32 =htonl(sendPkg.owner);
		swrite(this,&ui32 ,4);

		ui16= htons(sendPkg.texture);
		swrite(this,&ui16,2);

		swrite(this,&(sendPkg.state),1);

		ui32 =htonl(sendPkg.x);
		swrite(this,&ui32 ,4);

		ui32 =htonl(sendPkg.y);
		swrite(this,&ui32 ,4);

		ui32 =htonl(sendPkg.z);
		swrite(this,&ui32 ,4);
		
		ui32 =htonl(sendPkg.vx);
		swrite(this,&ui32 ,4);

		ui32 =htonl(sendPkg.vy);
		swrite(this,&ui32 ,4);

		ui32 =htonl(sendPkg.vz);
		swrite(this,&ui32 ,4);

		ui32 =htonl(sendPkg.speed);
		swrite(this,&ui32 ,4);

		unlock();
		return *this;		
	}
	Player& sendRequest(reqPkg sendPkg){
		lock();
		swrite(this,&(sendPkg.type),1);
		__uint16_t ui16 =htons(sendPkg.spec);
		swrite(this,&ui16 ,2);

		unlock();
		return *this;				
	}	
	Player& sendAck(ackPkg sendPkg){
		lock();
		swrite(this,&(sendPkg.type),1);
		__uint16_t ui16 =htons(sendPkg.spec);
		swrite(this,&ui16 ,2);

		unlock();
		return *this;				
	}
	Player& sendResp(respPkg sendPkg){
		lock();
		swrite(this,&(sendPkg.type),1);
		__uint16_t ui16 =htons(sendPkg.spec);
		swrite(this,&ui16 ,2);
		__uint32_t ui32 =htonl(sendPkg.data);
		swrite(this,&ui32 ,4);
		unlock();		
		return *this;		
	}
#include<iostream>
	Player& read(){
		while(this->connected){
		    	unsigned char type;
		    	pkg* ret;
			__uint32_t ui32;
			__uint16_t ui16;
			char i8;
			//std::temp->startning sread loop=std::endl;
			::sread(this,&type,1);
			//std::temp->type recieved: =(int)type<<std::endl;
			//cout<<"Recieved package type "<<(int)type<<" from client "<<this->playerData.ID<<endl;
			if(type==1||type==6){//done
			    	playerPkg *temp=new playerPkg();
			    	temp->type=type;


				::sread(this,&ui32,4);
				temp->ID=ntohl(ui32);

				::sread(this,&ui16,2);
				temp->texture=ntohs(ui16);

				::sread(this,&i8,1);
				temp->state= i8;	

				::sread(this,&ui32,4);
				temp->x=(ntohl(ui32)) ;
				::sread(this,&ui32,4);
				temp->y=(ntohl(ui32) );		
				::sread(this,&ui32,4);
				temp->z=(ntohl(ui32) );
				::sread(this,&ui32,4);
				temp->vx=(ntohl(ui32) );	
				::sread(this,&ui32,4);
				temp->vy=(ntohl(ui32) );	
				::sread(this,&ui32,4);
				temp->vz=(ntohl(ui32) );
				::sread(this,&ui32,4);
				temp->pvx =(ntohl(ui32) );	
				::sread(this,&ui32,4);
				temp->pvy =(ntohl(ui32) );	
				::sread(this,&ui32,4);
				temp->pvz =(ntohl(ui32) );
				::sread(this,&ui32,4);
				temp->theta =(ntohl(ui32) );	
				::sread(this,&ui32,4);
				temp->phi =(ntohl(ui32) );	
				::sread(this,&ui32,4);
				temp->speed =(ntohl(ui32)) ;
				temp->inclusive=false;
			    	sendAll(temp);
			}else if(type==2){//done

			    projPkg *temp= new projPkg();
			    temp->type=type;

		
					::sread(this,&ui32,4);
					temp->ID=ntohl(ui32);

					::sread(this,&ui16,2);
					temp->texture=ntohs(ui16);

					::sread(this,&i8,1);
					temp->state= i8;	

					::sread(this,&ui32,4);
					temp->x=(ntohl(ui32)) ;
					::sread(this,&ui32,4);
					temp->y=(ntohl(ui32) );		
					::sread(this,&ui32,4);
					temp->z=(ntohl(ui32) );
					::sread(this,&ui32,4);
					temp->vx=(ntohl(ui32) );	
					::sread(this,&ui32,4);
					temp->vy=(ntohl(ui32) );	
					::sread(this,&ui32,4);
					temp->vz=(ntohl(ui32) );
			
					temp->speed =(ntohl(ui32)) ;
		
			   	sendAll(temp);
			}else if(type==3){//done
			
			    	reqPkg temp=reqPkg();
				temp.type=type;

				::sread(this,&ui16,2);
				temp.spec=ntohs(ui16);

				if(temp.spec==1){//join
					respPkg resp;
					resp.type=5;
					resp.spec=1;
					resp.data=playerData.ID;
					playerPkg *data=new playerPkg();
					data->ID=resp.data;
					this->sendResp(resp);
					data->type=1;
					data->texture=1;
					data->state=0;
					data->x=0;
					data->y=qPack(500);
					data->z=0;
					data->vx=0;
					data->vy=0;
					data->vz=0;
					data->pvx=0;
					data->pvy=0;
					data->pvz=0;
					data->theta=0;
					data->phi=0;
					data->speed=qPack(20);
					playerData.ID=data->ID;
					sendAll(data);
					for(auto &player :players){
						if (this!=player) {
							player->playerData.type=1;
							sendPlayer(player->playerData);
							
							}

					}
				}else if(temp.spec==2){
					this->shoot();
				}else if(temp.spec==3){
					//I am just going to hope no one asks to sync
				}
			}else if(type==4){//done
			/*we dont care about ack much...
			    ackPkg *temp=new ackPkg();
			    temp->type=type;

					::sread(this,&ui16,2);
					temp->spec=ntohs(ui16);

			    ret=temp;
			*/
			}else if(type==5){//done
			/*we dont care about resp data much... sorry
			    respPkg *temp=new respPkg();
			    temp->type=type;

					::sread(this,&ui16,2);
					temp->spec=ntohs(ui16);
			
					::sread(this,&ui32,4);
					temp->data=ntohl(ui32);
			    ret=temp;*/
			}	
		    }
	    return *this;
	}
};
void spinUp(Player * self){
	self->read();
}
void sendAll(pkg *sendPkg){
		sendAllLock.lock();
		if(sendPkg->type==1||sendPkg->type==6){
			playerPkg p=*((playerPkg*)sendPkg);
			for(auto &player :players){
				if(p.ID!=player ->playerData.ID){
					p.type=1;
					player ->sendPlayer(p);
				}else{
					player ->playerData.type=p.type;
					player ->playerData.x=p.x;
				
					player ->playerData.y=p.y;
					player ->playerData.z=p.z;
					player ->playerData.ID=p.ID;
					player ->playerData.vx=p.vx;
					player ->playerData.vz=p.vz;
					player ->playerData.vy=p.vy;
					player ->playerData.pvx=p.pvx;
					player ->playerData.pvy=p.pvy;
					player ->playerData.pvz=p.pvz;
					player ->playerData.state=p.state;
					player ->playerData.theta=p.theta;
	
					player ->playerData.phi=p.phi;
					player ->playerData.speed=p.speed;
					if(p.inclusive) player ->sendMe();

				}
			}
			delete (playerPkg*)sendPkg;
		}else if(sendPkg->type==2){
			projPkg p=*((projPkg*)sendPkg);
			for(auto &player :players){
				player ->sendProj(p);
			}
			delete (projPkg*)sendPkg;
		}else if(sendPkg->type==3){
			reqPkg p=*((reqPkg*)sendPkg);
			for(auto &player :players){
				player ->sendRequest(p);
			}
			delete (reqPkg*)sendPkg;
		}else if(sendPkg->type==4){
			ackPkg p=*((ackPkg*)sendPkg);
			for(auto &player :players){
				player ->sendAck(p);
			}
			delete (ackPkg*)sendPkg;
		}else if(sendPkg->type==5){
			respPkg p=*((respPkg*)sendPkg);			
			for(auto &player :players){
				player ->sendResp(p);
			
			}
			delete (respPkg*)sendPkg;
		}
		sendAllLock.unlock();
}
void

	signal_callback_handler(int signum)

	{
//handle broken pipe, not really an issue, we handle it other places

	}

inline ssize_t swrite(Player *player,const void * data,size_t count){
	//return 0;	
	
	ssize_t ret=write(player->client_fd,data,count*player->connected);//if connected, write
	 player->connected*=ret>=0;//if you cant write, mark dissconnected
	//cout<<"Sent "<<count<<" bytes to client "<<fd<<" bytes sent where ";
	//for (int i=0;i<count;i++){
	//cout<<hex<<(int)((char*)data)[i]<<" ";
	//}		
	//cout<<endl;
	return ret;
}
inline ssize_t sread(Player *player,void * place,size_t count){
	size_t ret=read(player->client_fd,place,count*player->connected);
	 player->connected*=ret>=0;
	//cout<<"Read "<<count<<" bytes from client "<<fd<<" bytes read where ";
	//for (int i=0;i<count;i++){
	//cout<<hex<<(int)((char*)place)[i]<<" ";
	//}		
	//cout<<endl;
	return ret;
}  
int main( int args,char ** argv){
	signal(SIGPIPE, signal_callback_handler);
	using namespace std;
	players=vector<Player*>();

	struct sockaddr_in sad;
	sad.sin_port = htons(5143);
	sad.sin_addr.s_addr = INADDR_ANY;
	sad.sin_family = AF_INET;

	int skt = socket(AF_INET, SOCK_STREAM, 0);
	int success=bind(skt, (struct sockaddr *)(&sad), sizeof(struct sockaddr_in));
	if(success){
		perror("Could not aquire network port");
		return -1;
	}
	
	listen(skt, 5);
	int client_fd;
	
	for(;;){
		cout<<"waiting for connection"<<endl;
		client_fd = accept(skt, 0, 0);
		cout<<"Connected"<<endl;
		players.push_back(new Player(client_fd));

	}
	return 0;
}
