// LoadedObject.h:  A class to represent objects loaded from blender or other .obj source

#ifndef __LOADEDOBJECT__
#define __LOADEDOBJECT__
#include <tiny_obj_loader.h>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string>
class LoadedObject {
	public:
		unsigned dl;
		/*
		 * Loads object in the file "filename" into video memory
		 * Allocates a display list ID for the object
		 */
		LoadedObject();//this inits glDisplayList for you dont forget to glEndList after entering texture
		LoadedObject(unsigned num);
		LoadedObject(const char* filename);
		LoadedObject(const std::string filename);
		LoadedObject(const LoadedObject &copy);
		~LoadedObject(){
			glDeleteLists(dl, 1);
		}

		void display(){
			//glutWireSphere(3, 10, 10);
			glCallList(dl);
		}
};

#endif
