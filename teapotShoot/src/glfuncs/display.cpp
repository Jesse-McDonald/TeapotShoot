#include<GL/glu.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<viewmanager.h>
//#include<player.h>
#include <var.h>
#include <world.h>
void crosshair(){
	glBegin(GL_LINES);
	glVertex3f(0, -0.1, 0);
	glVertex3f(0, 0.1, 0);
	glVertex3f(.1, 0, 0);
	glVertex3f(-.1, 0, 0);
	glEnd();
}
void drawScene(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	crosshair();

	gluPerspective(90, 1, .1, 1000);

	float yheight = 0;
	view.calcLooking();
	gluLookAt(world.players[0].x, world.players[0].y, world.players[0].z,   view.lookingX+world.players[0].x, view.lookingY+world.players[0].y, view.lookingZ+world.players[0].z,     0, 1, 0);
	/*
	glBegin(GL_LINES);
	glVertex3f(xpos + lap[0] - 0.1, yheight, zpos + lap[2]);
	glVertex3f(xpos + lap[0] + 0.1, yheight, zpos + lap[2]);
	glVertex3f(xpos + lap[0], yheight - 0.1, zpos + lap[2]);
	glVertex3f(xpos + lap[0], yheight + 0.1, zpos + lap[2]);
	glEnd();
*/
	
	if(cleanLock.try_lock()){
		for(int i=0;i<world.players.size();i++){
			world.players[i].draw();
		}
		for(auto &projectile :world.projectiles){
			projectile.draw();
	
		}
		cleanLock.unlock();
	}


//this is the floor
	glBegin(GL_QUADS);
	for(int x = -10; x < 10; x++){
		for(int z = -10; z < 10; z++){
			float material_color[] = {0.2+z/50.0, 0.9, 0.2+z/50.0, 1.0};
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color);
			float material_spec_color[] = {.2+z/50.0, .9, .2+z/50.0, 1};
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_spec_color);
			glNormal3f(0, 1, 0);
			glVertex3f(x, -1, z);			
			glVertex3f(x+1, -1, z);			
			glVertex3f(x+1, -1, z+1);			
			glVertex3f(x, -1, z+1);			
		}
	}
	glEnd();

	
	glutSwapBuffers();
}


void windowClose(){
	connection->leave();
}
