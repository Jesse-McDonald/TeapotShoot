
#include <keyboard.h>
#include <display.h>
#include <mouse.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include<viewmanager.h>
#include <var.h>

void glinit(int argc, char ** argv)
{    
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width,height);
	glutCreateWindow("Simple OpenGL Demo");
	glEnable(GL_DEPTH_TEST);
	view=View();
	glutIdleFunc(drawScene);
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(keypress);
	glutKeyboardUpFunc(keyup);
	glutDisplayFunc(drawScene);
	glutPassiveMotionFunc(mousemove);
	glutMotionFunc(mousemove);
	glutMouseFunc(mousefire);
	glutSetCursor(GLUT_CURSOR_NONE);

	//glutCloseFunc(windowClose);

   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_DEPTH_TEST);

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                   GL_NEAREST);

	float material_color[] = {0.5, 0.5, 1.0, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color);
	float material_spec_color[] = {.8, .8, 1, 1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_spec_color);
	
	float light0position[] = {5, 3, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, light0position);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float light1color[] = {0.5, 0.5, 0.5, 1.0};
	float light1position[] = {0, 500, 0};
	float down[] = {0, -1, 0};
	glLightfv(GL_LIGHT1, GL_AMBIENT_AND_DIFFUSE, light1color);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1color);
	glLightfv(GL_LIGHT1, GL_POSITION, light1position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, down);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 5);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 1);
	glEnable(GL_LIGHT1);

}
