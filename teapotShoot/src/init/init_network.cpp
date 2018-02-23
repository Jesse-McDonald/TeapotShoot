#include <connection.h>

#include <iostream>
Connection* connection=new Connection();
void init_network(std::string ip, std::string port){
	std::cout<<connection->connect(ip,port)<<std::endl;
	connection->startReader();	
	connection->join();
	
}
