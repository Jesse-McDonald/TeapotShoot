#include <LoadedObject.h>


#include<iostream>
using namespace tinyobj;
using namespace std;
/*
 * Loads object in the file "filename" into video memory
 * Allocates a display list ID for the object
 */
LoadedObject::LoadedObject(){
	dl = glGenLists(1);
	glNewList(dl, GL_COMPILE);

}//dont forget to glEndList

LoadedObject::LoadedObject(unsigned list){
	dl=list;
}
LoadedObject::LoadedObject(const LoadedObject &copy){
	dl=copy.dl;
}
LoadedObject::LoadedObject(const std::string filename){
	LoadedObject(filename.c_str());
}
LoadedObject::LoadedObject(const char* filename) {
	string err;
	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
		
	if(!LoadObj(&attributes, &shapes, &materials, &err, filename)){
		printf("Oh no!  We got error %s\n", err.c_str());
		return;
	}
	dl = glGenLists(1);
	glNewList(dl, GL_COMPILE);

	for(auto &s : shapes){
		// Add the shape to the display list
		cout << "Loading shape:  " << s.name << endl;
		// In case we do something different for each shape
		glBegin(GL_TRIANGLES);
		for(auto &i : s.mesh.indices){
			// TODO:  Deal with materials
			// Add the indicated vertex, normal, and texture coordinate
			printf("Loading vertex:  %f %f %f \n", attributes.vertices[i.vertex_index*3],
							attributes.vertices[i.vertex_index*3+1],
							attributes.vertices[i.vertex_index*3+2]);
			glVertex3f(attributes.vertices[i.vertex_index*3],
							attributes.vertices[i.vertex_index*3+1],
							attributes.vertices[i.vertex_index*3+2]);
			
			if(i.normal_index != -1) // Then actually load it
				glNormal3f(attributes.normals[i.normal_index*3],
								attributes.normals[i.normal_index*3+1],
								attributes.normals[i.normal_index*3+2]);
			if(i.texcoord_index != -1)
				glTexCoord2f(attributes.texcoords[i.texcoord_index*2], 
						attributes.texcoords[i.texcoord_index*2+1]);

		}
		glEnd();
	}
	glEndList();
}

