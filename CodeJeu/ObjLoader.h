/*
 *
 * Demonstrates how to load and display an Wavefront OBJ file.
 * Using triangles and normals as static object. No texture mapping.
 *
 * OBJ files must be triangulated!!!
 * Non triangulated objects wont work!
 * You can use Blender to triangulate
 *
 */

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#include "Mesh.h"

#define KEY_ESCAPE 27

using namespace std;

/***************************************************************************
Méthode pour charger un objet .obj
 ***************************************************************************/

class ObjLoader
{

  public:
    
  Vec3f calculateNormal(Vec3f coord1,Vec3f coord2,Vec3f coord3 );
  int load(string filename);	// Loads the model
	void draw();					// Draws the model on the screen
	void release();				// Release the model

};
