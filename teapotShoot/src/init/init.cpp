#include <glinit.h>
#include<thread>
#include<movement.h>
#include<iostream>
#include <var.h>
#include<GL/glut.h>
#include <init_world.h>
#include<init_network.h>
int init(int argc, char ** argv){
	
	fps=60;
	width=1028;
	height=768;
	mStatus.state=1;
	glinit(argc,argv);
	init_network("isoptera.lcsc.edu","5143");
	init_world();

	std::thread move(movement);
	glutMainLoop();
	connection->leave();
	std::cout<<"ending"<<std::endl;
	return 0;
}
