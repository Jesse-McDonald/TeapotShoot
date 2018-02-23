#include<connection.h>
#include <world.h>
#include <var.h>
#include <player.h>
#include <projectile.h>
#include <iostream>
inline ssize_t dwrite(int fd,const void * data,size_t count){
	using namespace std;
	ssize_t ret=write(fd,data,count);
	//cout<<"Sent "<<count<<" bytes to server bytes sent where ";
	//for (int i=0;i<count;i++){
	//cout<<hex<<(int)((char*)data)[i]<<" ";
	//}		
	//cout<<endl;
	return ret;
}
inline ssize_t dread(int fd,void * place,size_t count){
	using namespace std;	
	size_t ret=read(fd,place,count);
	//cout<<"Read "<<count<<" bytes from server bytes read where ";
	//for (int i=0;i<count;i++){
	//cout<<hex<<(int)((char*)place)[i]<<" ";
	//}		
	//cout<<endl;
	return ret;
} 
std::mutex netLock;
std::mutex datLock;
Connection::Connection(){
	pks=std::queue<std::shared_ptr<pkg>>();
}

int Connection::connect(std::string ip, std::string port){
    sad.sin_port=htons(atoi(port.c_str()));
    sad.sin_family=AF_INET;

    this->skt=socket(AF_INET,SOCK_STREAM,0);
    entry = gethostbyname(ip.c_str());
    if(!entry) {
        throw std::runtime_error("Get host by name -> null");
    }
    addr_list = (struct in_addr**)entry->h_addr_list;
    c_addr = addr_list[0];
    sad.sin_addr=*c_addr;
	int ret=::connect(this->skt, (struct sockaddr*)&sad, sizeof(struct sockaddr_in));    
	connected=(ret==0);
    return ret;
}

Connection& Connection::leave(){
	closeThread =true;
	connected=false;
   	close(this->skt);
	listener.join();
	return *this;
}

Connection& Connection::lock(){
	datLock.lock();
	return *this;
}

Connection& Connection::unlock(){
	datLock.unlock();
	return *this;
}

Connection& Connection::startReader(){
	listener= std::thread(reader,this);
	return *this;
}
#include <iostream>
Connection& Connection::handle(){
	using namespace std;
	this->lock();
	while(!this->pks.empty()){
		std::shared_ptr<pkg> temp=this->pks.front();
		pkg* cPkg=temp.get();
		if(cPkg->type==1){
			playerPkg p=*((playerPkg*)cPkg);
			Player newPlayer=Player(p.x,p.y,p.z,p.theta,p.phi,world.assets.get("player"));
			newPlayer.vx=p.vx;
			newPlayer.vy=p.vy;
			newPlayer.vz=p.vz;
			newPlayer.pvx=p.pvx;
			newPlayer.pvy=p.pvy;
			newPlayer.pvz=p.pvz;
			newPlayer.ID=p.ID;
			newPlayer.state=p.state;
			newPlayer.speed=p.speed;
			bool found=false;
			cleanLock.lock();
			for(int i=0;i<world.players.size();i++){

				if(world.players[i].ID==p.ID){//the times you wish you also used a hashmap for players too
					world.players[i]=newPlayer;
					found=true;		
				}
			}
			if(!found){
				world.players.push_back(newPlayer);
			}
			cleanLock.unlock();
		}else if(cPkg->type==2){
	
			projPkg p=*((projPkg*)cPkg);
			cout<<"Type "<<p.type<<endl;
			cout<<"x "<<p.x<<endl;
			cout<<"y "<<p.y<<endl;
			cout<<"z "<<p.z<<endl;
			cout<<"vx "<<p.vx<<endl;
			cout<<"vy "<<p.vy<<endl;
			cout<<"vz "<<p.vz<<endl;
			cout<<"ID "<<p.ID<<endl;
			cout<<"owner "<<p.owner<<endl;
			cout<<"state "<<p.state<<endl;
			cout<<"speed "<<p.speed<<endl;
			Projectile newProj=Projectile(p.x,p.y,p.z,0,0,world.assets.get("projectile"));
			newProj.vx=p.vx;
			newProj.vy=p.vy;
			newProj.vz=p.vz;
			newProj.ID=p.ID;
			newProj.attatched_id=p.owner;
			newProj.state=p.state;
			newProj.speed=p.speed;
			bool found=false;
			cleanLock.lock();
			for(int i=0;i<world.projectiles.size();i++){

				if(world.projectiles[i].ID==p.ID){//the times you wish you also used a hashmap for projectiless too
					world.projectiles[i]=newProj;
					found=true;		
				}
			}
			if(!found){
				world.projectiles.push_back(newProj);
			}
			cleanLock.unlock();
		}else if(cPkg->type==3){
			reqPkg p=*((reqPkg*)cPkg);
			if(p.spec==3){
				this->sendMe();
			}else if(p.spec==4){
				//yah, not sure what to do with notif death since the server is sending a dead player packet any way...
			}
			
		}else if(cPkg->type==4){
			//yah... not sure what to actualy do with the ack package... but its here if I need it
		}else if(cPkg->type==5){
			//yah... network protocal is built to support a better constructed game than I have.... oh well, turns our this implimentation never uses data from response packets so why bother processing them
		}else if(cPkg->type==6){
		
			playerPkg p=*((playerPkg*)cPkg);
			Player newPlayer=Player(p.x,p.y,p.z,p.theta,p.phi,world.assets.get("nul"));
			//newPlayer.vx=p.vx;
			//newPlayer.vy=p.vy;
			//newPlayer.vz=p.vz;//inexplicable slidewalk if these are on
			//newPlayer.vx=p.pvx;
			//newPlayer.vy=p.pvy;
			//newPlayer.vz=p.pvz;
			newPlayer.ID=p.ID;
			newPlayer.state=p.state;
			newPlayer.speed=p.speed;
			cleanLock.lock();
			world.players[0]=newPlayer;
			
			bool found=false;
			cleanLock.unlock();
		}
		this->pks.pop();
	}
	this->unlock();
	return *this;
}
Connection& Connection::reader(Connection* self){
	while(self->connected){
    	unsigned char type;
    	pkg* ret;
	__uint32_t ui32;
	__uint16_t ui16;
	char i8;
	//std::temp->startning dread loop=std::endl;
	dread(self->skt,&type,1);
	//std::temp->type recieved: =(int)type<<std::endl;
	if(type==1||type==6){//done

	    playerPkg *temp=new playerPkg();
	    temp->type=type;


			dread(self->skt,&ui32,4);
			temp->ID=ntohl(ui32);

			dread(self->skt,&ui16,2);
			temp->texture=ntohs(ui16);

			dread(self->skt,&i8,1);
			temp->state= i8;	

			dread(self->skt,&ui32,4);
			temp->x=self->fUnpack(ntohl(ui32)) ;
			dread(self->skt,&ui32,4);
			temp->y=self->fUnpack(ntohl(ui32) );		
			dread(self->skt,&ui32,4);
			temp->z=self->fUnpack(ntohl(ui32) );
			dread(self->skt,&ui32,4);
			temp->vx=self->fUnpack(ntohl(ui32) );	
			dread(self->skt,&ui32,4);
			temp->vy=self->fUnpack(ntohl(ui32) );	
			dread(self->skt,&ui32,4);
			temp->vz=self->fUnpack(ntohl(ui32) );
			dread(self->skt,&ui32,4);
			temp->pvx =self->fUnpack(ntohl(ui32) );	
			dread(self->skt,&ui32,4);
			temp->pvy =self->fUnpack(ntohl(ui32) );	
			dread(self->skt,&ui32,4);
			temp->pvz =self->fUnpack(ntohl(ui32) );
			dread(self->skt,&ui32,4);
			temp->theta =self->fUnpack(ntohl(ui32) );	
			dread(self->skt,&ui32,4);
			temp->phi =self->fUnpack(ntohl(ui32) );	
			dread(self->skt,&ui32,4);
			temp->speed =self->fUnpack(ntohl(ui32)) ;

	    ret=temp;
	}else if(type==2){//done

	    projPkg *temp=new projPkg();
	    temp->type=type;

		
			dread(self->skt,&ui32,4);
			temp->ID=ntohl(ui32);
			dread(self->skt,&ui32,4);
			temp->owner=ntohl(ui32);

			dread(self->skt,&ui16,2);
			temp->texture=ntohs(ui16);

			dread(self->skt,&i8,1);
			temp->state= i8;	

			dread(self->skt,&ui32,4);
			temp->x=self->fUnpack(ntohl(ui32)) ;
			dread(self->skt,&ui32,4);
			temp->y=self->fUnpack(ntohl(ui32) );		
			dread(self->skt,&ui32,4);
			temp->z=self->fUnpack(ntohl(ui32) );
			dread(self->skt,&ui32,4);
			temp->vx=self->fUnpack(ntohl(ui32) );	
			dread(self->skt,&ui32,4);
			temp->vy=self->fUnpack(ntohl(ui32) );	
			dread(self->skt,&ui32,4);
			temp->vz=self->fUnpack(ntohl(ui32) );
			
			temp->speed =self->fUnpack(ntohl(ui32)) ;

	    ret=temp;
	}else if(type==3){//done

	    reqPkg *temp=new reqPkg();
	    temp->type=type;

			dread(self->skt,&ui16,2);
			temp->spec=ntohs(ui16);

	    ret=temp;
	}else if(type==4){//done

	    ackPkg *temp=new ackPkg();
	    temp->type=type;

			dread(self->skt,&ui16,2);
			temp->spec=ntohs(ui16);

	    ret=temp;
	}else if(type==5){//done

	    respPkg *temp=new respPkg();
	    temp->type=type;

			dread(self->skt,&ui16,2);
			temp->spec=ntohs(ui16);
			
			dread(self->skt,&ui32,4);
			temp->data=ntohl(ui32);
	    ret=temp;
	}
	else{//done
	    ret=new pkg();
	    ret->type=type;
	}
	
	    
	self->lock();
	self->cPak.reset(ret);
	self->pks.push(self->cPak);
	self->unlock();
    }
    return *self;
}


Connection& Connection::sendMe(){
	netLock.lock();
	if(!connected) return this->offline(0);
	char i8=6;
	dwrite(this->skt,&i8,1);
	__uint32_t ui32=htonl(world.players[0].ID); 
	dwrite(this->skt,&ui32,4);
	__uint16_t ui16=htons(1); 
	dwrite(this->skt,&ui16,2);
	    
	i8=world.players[0].state;
	dwrite(this->skt,&i8,1);
	
	ui32=htonl(qPack(world.players[0].x)); 
	dwrite(this->skt,&ui32,4);
	ui32=htonl(qPack(world.players[0].y)); 
	dwrite(this->skt,&ui32,4);
	ui32=htonl(qPack(world.players[0].z)); 
	dwrite(this->skt,&ui32,4);
	ui32=htonl(qPack(world.players[0].vx)); 
	dwrite(this->skt,&ui32,4);
	ui32=htonl(qPack(world.players[0].vy)); 
	dwrite(this->skt,&ui32,4);
	ui32=htonl(qPack(world.players[0].vz)); 
	dwrite(this->skt,&ui32,4);
	ui32=htonl(qPack(world.players[0].pvx)); 
	dwrite(this->skt,&ui32,4);
	ui32=htonl(qPack(world.players[0].pvy)); 
	dwrite(this->skt,&ui32,4);
	ui32=htonl(qPack(world.players[0].pvz)); 
	dwrite(this->skt,&ui32,4);
	ui32=htonl(qPack(world.players[0].theta)); 
	dwrite(this->skt,&ui32,4);
	ui32=htonl(qPack(world.players[0].phi)); 
	dwrite(this->skt,&ui32,4);
	ui32=htonl(qPack(world.players[0].speed)); 
	
	dwrite(this->skt,&ui32,4);
	netLock.unlock();
	return *this;
}

Connection& Connection::newProjectile(){
	
	if(!connected) return this->offline(2);
	char i8=3;
	sendMe();
	netLock.lock();
	dwrite(this->skt,&i8,1);
	__uint16_t ui16=htons(2);
	dwrite(this->skt,&ui16,2);
netLock.unlock();
	return *this;
}

Connection& Connection::sync(){
netLock.lock();
	if(!connected) return this->offline(3);

	char i8=3;
	dwrite(this->skt,&i8,1);
	__uint16_t ui16=htons(3);
	dwrite(this->skt,&ui16,1);
netLock.unlock();
	return *this;
}
Connection& Connection::join(){
netLock.lock();
	if(!connected) return this->offline(1);
	char i8=3;
	dwrite(this->skt,&i8,1);
	__uint16_t ui16=htons(1);
	dwrite(this->skt,&ui16,2);
netLock.unlock();
	return *this;
}
Connection& Connection::offline(int arg){
	if(arg==0){

	}else if(arg==1){
  
	}else if(arg==2){

	}else if(arg==3){
		
	}
	return *this;
}

__int32_t Connection::qPack(float in){
	return (__int32_t) (in/(float)FLOAT_REZ);
	
}

float Connection::fUnpack(__int32_t in){
	return (float)in*(float)FLOAT_REZ;

}
