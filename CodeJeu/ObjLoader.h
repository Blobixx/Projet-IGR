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

#include <windows.h>
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
	ObjLoader();
  float* ObjLoader::calculateNormal(float* coord1,float* coord2,float* coord3 );
  int ObjLoader::load(string filename);	// Loads the model
	void ObjLoader::draw();					// Draws the model on the screen
	void ObjLoader::release();				// Release the model

	float* normals;							// Stores the normals
  float* Faces_Triangles;					// Stores the triangles
	float* vertexBuffer;					// Stores the points which make the object
	long TotalConnectedPoints;				// Stores the total number of connected verteces
	long TotalConnectedTriangles;			// Stores the total number of connected triangles

};