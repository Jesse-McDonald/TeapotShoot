#include <LoadedObject.h>
#include <var.h>
#include<GL/glut.h>
#include<world.h>
#include<hitbox.h>
void init_world(){
		world=World();

		world.assets.add("nul",LoadedObject());//these are dangerous and make it so we can never do assets.reload()
		glEndList();
		{
			world.assets.add("player",LoadedObject());

			float material_color[] = {1,1,1, 1.0};
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color);
			float material_spec_color[] = {1,1,1, 1};
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_spec_color);

			glutSolidTeapot(1);
			glEndList();
		}
		{
			world.assets.add("projectile",LoadedObject());
		
			float material_color[] = {1,0,0, 1.0};
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color);
			float material_spec_color[] = {1,0,0, 1};
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_spec_color);
		
			glutSolidSphere(.1,20,20);
			glEndList();
		}		
		Player tp=Player((float)0,(float)0,(float)0,(float)0,(float)0,world.assets["nul"]);
		tp.ID=1;
		world.players.push_back(tp);

		//tp=Player((float)0,(float)0,(float)0,(float)0,(float)0,world.assets["player"]);
		//tp.ID=2;
		//world.players.push_back(tp);

}
