// ----------------------------------------------
// Informatique Graphique 3D & R�alit� Virtuelle.
// Projet
// Lancer de Rayon de Monte Carlo
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <GL/glut.h>
#include "Vec3.h"
#include "tiny_obj_loader.h"
#include "Ray.h"
#include "KdNode.h"
#include "BoundingBox.h"

using namespace std;

// App parameters
static const unsigned int DEFAULT_SCREENWIDTH = 1024;
static const unsigned int DEFAULT_SCREENHEIGHT = 768;
static const char * DEFAULT_SCENE_FILENAME = "scenes/cornell_box/cornell_box.obj";
static string appTitle ("MCRT - Monte Carlo Ray Tracer");
static GLint window;
static unsigned int screenWidth = 1024;
static unsigned int screenHeight = 768;
static bool rayDisplayMode = false;

// Camera parameters
static float fovAngle = 45.f ;
static float aspectRatio = 1024/768;
static float nearPlane;
static float farPlane;
static Vec3f camTarget;
static Vec3f camUp = Vec3f(0,0,1);
 // Expressing the camera position in polar coordinate, in the frame of the target

// Scene elements
static Vec3f lightPos = Vec3f (1.f, 1.f, 1.f);
static Vec3f lightColor = Vec3f (1.f, 1.f, 1.f);
static Vec3f sceneCenter = Vec3f (0.f, 0.f, 0.f);
static float sceneRadius = 1.f;
static vector<tinyobj::shape_t> shapes;
static vector<tinyobj::material_t> materials;

// Mouse parameters
static bool mouseLeftButtonClicked = false;
static int clickedX, clickedY;
static float baseCamPhi;
static float baseCamTheta;

// Raytraced image
static unsigned char * rayImage = NULL;


void printUsage () {
  std::cerr << std::endl // send a line break to the standard error output
			<< appTitle << std::endl
			<< "Author : Tamy Boubekeur" << std::endl << std::endl
			<< "Usage : ./myRayTracer [<file.obj>]" << std::endl
			<< "Commandes clavier :" << std::endl
			<< "------------------" << std::endl
			<< " ?: Print help" << std::endl
			<< " <space>: Toggle raytracing/rasterization (GL)  display mode" << std::endl
			<< " r: Ray trace an image from the current point of view" << std::endl
			<< " s: Save the current ray traced image under raytraced_image.ppm" << std::endl
			<< " <drag>+<left button>: rotate model" << std::endl
			<< " <drag>+<right button>: move model" << std::endl
			<< " <drag>+<middle button>: zoom" << std::endl
			<< " q, <esc>: Quit" << std::endl << std::endl;
}

void initOpenGL () {
  glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
  glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
  glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
  glEnable (GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Background color
  glEnable (GL_COLOR_MATERIAL);
}

int calculTailleTotalDePoints(){
  unsigned int taille=0;
  for (unsigned int s = 0; s < shapes.size (); s++){
     for (unsigned int p = 0; p < shapes[s].mesh.positions.size () / 3; p++) {
       taille++;
     }
   }

   return taille;
}

Vec3f evaluateResponse(Intersection intersection) {

  Vec3f wi = intersection.ptIntersection - lightPos ;
  Vec3f w0 = camEyeCartesian-intersection.ptIntersection;
  Vec3f wh = Vec3f(w0 + wi);

    float alphaP = 0.01f;
    // GGX
    float GGGX1 = (dot(intersection.normal,wi)*2)/(dot(intersection.normal,wi)+sqrt(pow(alphaP,2)+(1.f-pow(alphaP,2))*(pow(dot(intersection.normal,wi),2))))  ;
    float GGGX2 = (dot(intersection.normal,w0)*2)/(dot(intersection.normal,w0)+sqrt(pow(alphaP,2)+(1.f-pow(alphaP,2))*(pow(dot(intersection.normal,w0),2))))  ;
    float GGGX = GGGX1*GGGX2 ;

    float DGGX = pow(alphaP,2)/(3.14*pow(1.0f+(pow(alphaP,2)-1.0f)*pow(dot(intersection.normal,wh),2),2)) ;

    float F0 = 0.04f ;
	  float zero = 0.0f ;
	  float Fresnel = F0 + (1.f-F0)*pow(1-max(zero,dot(wi,wh)),5) ;

    Vec3f fs_GGX = (DGGX*Fresnel*GGGX)/(dot(intersection.normal,wi)*dot(intersection.normal,w0)*4)*(Vec3f(1.0f,0.0f,0.0f)+Vec3f(0.0f,1.0f,0.0f)+Vec3f(0.0f,0.0f,1.0f)) ;
  //  Vec3f fd = Vec/ 3.14;
    Vec3f f = fs_GGX;

  Vec3f color = lightColor*dot(intersection.normal,wi)*f ; //on multipliera par la réflectance dans rayTrace()
  return color ;
}

vector<float> createListPointOrigine(){

vector<float> pointList;
unsigned int taille=calculTailleTotalDePoints();
 pointList.resize(taille);
 unsigned int i=0;
  for (unsigned int s = 0; s < shapes.size (); s++){
     for (unsigned int p = 0; p < shapes[s].mesh.positions.size () / 3; p++) {

       pointList[i]=shapes[s].mesh.positions[3*p] ;
       pointList[i+1]=shapes[s].mesh.positions[3*p+1] ;
       pointList[i+2]=shapes[s].mesh.positions[3*p+2] ;
       i=i+3;
     }
  }
  return pointList ;
}

BoundingBox computeBoundingBox(vector<float> pointList) {
  float xMin = 10000.f;
  float xMax = -10000.f;
  float yMin = 10000.f;
  float yMax = -10000.f;
  float zMin = 10000.f;
  float zMax = -10000.f;

  for(unsigned i =0; i<pointList.size();i+=3 ){
    if(pointList[i]<xMin){ xMin=pointList[i];}
    if(pointList[i]>xMax){ xMax=pointList[i];}

  }
  for(unsigned i =1; i<pointList.size();i+=3 ){
    if(pointList[i]<yMin){ yMin=pointList[i];}
    if(pointList[i]>yMax){ yMax=pointList[i];}

  }
  for(unsigned i =2; i<pointList.size();i+=3 ){
    if(pointList[i]<zMin){ zMin=pointList[i];}
    if(pointList[i]>zMax){ zMax=pointList[i];}

  }
  BoundingBox boundingBox = BoundingBox(xMin, xMax,yMin,yMax,zMin,zMax);
  return boundingBox;
}

vector<float> TriSelection(vector<float> liste,unsigned int n) {
	for(unsigned int i=0 ; i < n ; i++) {				//on parcourt le tableau
		unsigned int p = i ;
		for( unsigned int j = i ; j < n ; j++) {			//on cherche la position p du plus petit élément
			if(liste[j] < liste[p] ) {
        p=j;
      }
			//resultat.nbComparaisons++;	//comptage des opérations
		}
	float c = liste[i] ;						//on échange
	liste[i] = liste[p] ;					//les deux
	liste[p] = c ;						//nombres
	//resultat.nbAffectations+=2;			//comptage des opérations
  }
  vector<float> listeRetour ;
  listeRetour.resize(n) ;
  for(unsigned int k = 0 ; k < n ; k++) {
    liste[k] = listeRetour[k] ;
  }
  return listeRetour ;
}

float findMedianSample(BoundingBox boundingBox, vector<float> pointList) {
  char axeMax = boundingBox.maxAxis() ;
  int sizeList = pointList.size()/3 ;
  vector<float> listeATrier ;
  listeATrier.resize(pointList.size()/3) ;
  unsigned int i = 0 ;

  if (axeMax == 'x') {
    for(unsigned j = 0 ; j<pointList.size(); j+=3) {
      listeATrier[i] = pointList[j] ;
      i++ ;
    }
  }
  if (axeMax == 'y') {
    for(unsigned j = 1 ; j<pointList.size(); j+=3) {
      listeATrier[i] = pointList[j] ;
      i++ ;
    }
  }
  if (axeMax == 'z') {
    for(unsigned j = 2 ; j<pointList.size(); j+=3) {
      listeATrier[i] = pointList[j] ;
      i++ ;
    }
  }

  vector<float> listeTrier ;
  listeTrier = TriSelection(listeATrier, sizeList) ;

  int indiceMedian = floor(listeTrier.size()/2) +1 ;

  return listeTrier[indiceMedian] ;

}

vector<float> upperPartition(vector<float> listPoint, char axeMax, float val) {

    vector<float> upperPartition ;
    upperPartition.resize(listPoint.size()) ;
    unsigned int indice =0;
    if (axeMax == 'x') {
      for(unsigned j = 0 ; j<listPoint.size(); j+=3) {
        if(listPoint[j] > val ) {
          upperPartition[indice]=listPoint[j];
          upperPartition[indice+1]=listPoint[j+1];
          upperPartition[indice+2]=listPoint[j+2];
          indice+=3;

        }
      }
    }
    if (axeMax == 'y') {
      for(unsigned j = 1 ; j<listPoint.size(); j+=3) {
        if(listPoint[j] > val ) {
          upperPartition[indice]=listPoint[j-1];
          upperPartition[indice+1]=listPoint[j];
          upperPartition[indice+2]=listPoint[j+1];
          indice+=3;

        }
      }
    }
    if (axeMax == 'z') {
      for(unsigned j = 2 ; j<listPoint.size(); j+=3) {
        if(listPoint[j] > val ) {
          upperPartition[indice]=listPoint[j-2];
          upperPartition[indice+1]=listPoint[j-1];
          upperPartition[indice+2]=listPoint[j];
          indice+=3;

        }
      }
    }

    return upperPartition ;
}

vector<float> lowerPartition(vector<float> listPoint, char axeMax, float val) {

    vector<float> lowerPartition ;
    lowerPartition.resize(listPoint.size()) ;
    unsigned int indice =0;

    if (axeMax == 'x') {
      for(unsigned j = 0 ; j<listPoint.size(); j+=3) {
        if(listPoint[j] < val ) {
          lowerPartition[indice]=listPoint[j];
          lowerPartition[indice+1]=listPoint[j+1];
          lowerPartition[indice+2]=listPoint[j+2];
          indice+=3;

        }
      }
    }
    if (axeMax == 'y') {
      for(unsigned j = 1 ; j<listPoint.size(); j+=3) {
        if(listPoint[j] < val ) {
          lowerPartition[indice]=listPoint[j-1];
          lowerPartition[indice+1]=listPoint[j];
          lowerPartition[indice+2]=listPoint[j+1];
          indice+=3;

        }
      }
    }
    if (axeMax == 'z') {
      for(unsigned j = 2 ; j<listPoint.size(); j+=3) {
        if(listPoint[j] < val ) {
          lowerPartition[indice]=listPoint[j-2];
          lowerPartition[indice+1]=listPoint[j-1];
          lowerPartition[indice+2]=listPoint[j];
          indice+=3;

        }
      }
    }

    return lowerPartition ;
}

void computeSceneNormals () {
  for (unsigned int s = 0; s < shapes.size (); s++)
	if (shapes[s].mesh.normals.empty ()) {
	  shapes[s].mesh.normals.resize (shapes[s].mesh.positions.size (), 0.f);
	  for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
		Vec3f q[3];
		for (size_t v = 0; v < 3; v++) {
		  unsigned int index = 3*shapes[s].mesh.indices[3*f+v];
		  for (unsigned int i = 0; i < 3; i++)
			q[v][i] = shapes[s].mesh.positions[index+i];
		}
		Vec3f e01 = q[1] - q[0];
		Vec3f e02 = q[2] - q[0];
		Vec3f nf = normalize (cross (e01, e02));
		for (size_t v = 0; v < 3; v++) {
		  unsigned int index = 3*shapes[s].mesh.indices[3*f+v];
		  for (unsigned int i = 0; i < 3; i++)
			shapes[s].mesh.normals[index+i] += nf[i];
		}
	  }
	  for (unsigned int i = 0; i < shapes[s].mesh.normals.size () / 3; i++) {
		Vec3f n;
		for (unsigned int j = 0; j < 3; j++)
		  n[j] = shapes[s].mesh.normals[3*i+j];
		n.normalize ();
		for (unsigned int j = 0; j < 3; j++)
		  shapes[s].mesh.normals[3*i+j] = n[j];
	  }
	}
}

void computeSceneBoundingSphere () {
  sceneCenter = Vec3f (0.f, 0.f, 0.f);
  unsigned int count = 0;
  for (unsigned int s = 0; s < shapes.size (); s++)
	for (unsigned int p = 0; p < shapes[s].mesh.positions.size () / 3; p++) {
	  sceneCenter += Vec3f (shapes[s].mesh.positions[3*p],
							shapes[s].mesh.positions[3*p+1],
							shapes[s].mesh.positions[3*p+2]);
	  count++;
	}
  sceneCenter /= count;
  sceneRadius = 0.f;
  for (unsigned int s = 0; s < shapes.size (); s++)
    for (unsigned int p = 0; p < shapes[s].mesh.positions.size () / 3; p++) {
      float d = dist (sceneCenter, Vec3f (shapes[s].mesh.positions[3*p],
										  shapes[s].mesh.positions[3*p+1],
										  shapes[s].mesh.positions[3*p+2]));
      if (d > sceneRadius)
		sceneRadius = d;
    }
}

// Loads an OBJ file using tinyOBJ (http://syoyo.github.io/tinyobjloader/)
bool loadScene(const string & filename, const string & basepath = "") {
  shapes.clear ();
  materials.clear ();
  std::cout << "Loading " << filename << std::endl;
  std::string err = tinyobj::LoadObj(shapes, materials, filename.c_str (), basepath.c_str ());
  if (!err.empty()) {
    std::cerr << err << std::endl;
    return false;
  }
  computeSceneNormals ();
  computeSceneBoundingSphere ();
  return true;
}

void initCamera () {
  nearPlane = sceneRadius/10000.0f;
  farPlane = 10*sceneRadius;
  camTarget = sceneCenter;
}

void initLighting () {
  lightPos = 2.f * Vec3f (sceneRadius, sceneRadius, sceneRadius);
  glEnable (GL_LIGHTING);
  GLfloat position[4] = {lightPos[0], lightPos[1], lightPos[2], 1.0f};
  GLfloat color[4] = {lightColor[0], lightColor[1], lightColor[2], 1.0f};
  glLightfv (GL_LIGHT0, GL_POSITION, position);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, color);
  glLightfv (GL_LIGHT0, GL_SPECULAR, color);
  glEnable (GL_LIGHT0);
}

void init (const string & filename) {
  initOpenGL ();
  unsigned int i = filename.find_last_of ("/");
  loadScene (filename, filename.substr (0, i+1));
  initCamera ();
  initLighting ();
}

void setupCamera () {
  glMatrixMode (GL_PROJECTION); // Set the projection matrix as current. All upcoming matrix manipulations will affect it.
  glLoadIdentity ();
  gluPerspective (fovAngle, aspectRatio, nearPlane, farPlane); // Set the current projection matrix with the camera intrinsics
  glMatrixMode (GL_MODELVIEW); // Set the modelview matrix as current. All upcoming matrix manipulations will affect it.
  glLoadIdentity ();
  Vec3f eye = polarToCartesian (camEyePolar);
  swap (eye[1], eye[2]); // swap Y and Z to keep the Y vertical
  eye += camTarget;
  gluLookAt (eye[0], eye[1], eye[2],
			 camTarget[0], camTarget[1], camTarget[2],
			 0.0, 1.0, 0.0); // Set up the current modelview matrix with camera transform
}

void reshape (int w, int h) {
  screenWidth = w;
  screenHeight = h;
  aspectRatio = static_cast<float>(w)/static_cast<float>(h);
  glViewport (0, 0, (GLint)w, (GLint)h); // Dimension of the drawing region in the window
  setupCamera ();
  if (rayImage != NULL)
	delete [] rayImage;
  unsigned int l = 3*screenWidth*screenHeight;
  rayImage = new unsigned char [l];
  memset (rayImage, 0, l);
}

void rasterize () {
  setupCamera ();
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Erase the color and z buffers.
  glBegin (GL_TRIANGLES);
  glColor3f (1.f, 1.f, 1.f);
  for (size_t s = 0; s < shapes.size (); s++)
	for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
	  if (!materials.empty ()) {
		unsigned int i = shapes[s].mesh.material_ids[f];
		glColor3f (materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
  }
	  for (size_t v = 0; v  < 3; v++) {
		unsigned int index = 3*shapes[s].mesh.indices[3*f+v];
		glNormal3f (shapes[s].mesh.normals[index],
					shapes[s].mesh.normals[index+1],
					shapes[s].mesh.normals[index+2]);
		glVertex3f (shapes[s].mesh.positions[index],
					shapes[s].mesh.positions[index+1],
					shapes[s].mesh.positions[index+2]);
	  }
	}
	glEnd ();
  glFlush (); // Ensures any previous OpenGL call has been executed
  glutSwapBuffers ();  // swap the render buffer and the displayed (screen) one
}

void displayRayImage () {
  glDisable (GL_DEPTH_TEST);
  glDrawPixels (screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, static_cast<void*>(rayImage));
  glutSwapBuffers ();
  glEnable (GL_DEPTH_TEST);
}

Intersection raySceneIntersection(Ray ray) {
  //chaque intersection est suivie de sa normale dans la liste;
  Intersection retour = Intersection(camEyeCartesian, Vec3f(0.f,0.f,0.f));
  float distanceMin = 100000.f;

	for (unsigned int s = 0; s < shapes.size (); s++) {
		//on tourne sur les triangles
		for (unsigned int t = 0; t < shapes[s].mesh.indices.size() / 3; t++) {
			//pour chaque triangle
				//on obtient les float x,y,z des vertices du triangle via index, index+1, index+2
				unsigned int indexV1 = 3*shapes[s].mesh.indices[3*t];
				unsigned int indexV2 = 3*shapes[s].mesh.indices[3*t+1];
				unsigned int indexV3 = 3*shapes[s].mesh.indices[3*t+2];
				//j'ai les 3 vertices de mon triangle
				Vec3f vertex1 = Vec3f(shapes[s].mesh.positions[indexV1],shapes[s].mesh.positions[indexV1+1],shapes[s].mesh.positions[indexV1+2]);
				Vec3f vertex2 = Vec3f(shapes[s].mesh.positions[indexV2],shapes[s].mesh.positions[indexV2+1],shapes[s].mesh.positions[indexV2+2]);
				Vec3f vertex3 = Vec3f(shapes[s].mesh.positions[indexV3],shapes[s].mesh.positions[indexV3+1],shapes[s].mesh.positions[indexV3+2]);
				//intersection est composée de la reelle intersection et de sa normale associée au triangle
				Intersection intersection = ray.RayTriangleIntersection(vertex1, vertex2, vertex3);

				//on retournera celle dont la distance est minimale
				if(intersection.ptIntersection != camEyeCartesian) {
          float d = dist(intersection.ptIntersection,camEyeCartesian);
          if( d < distanceMin){
            retour = intersection;
          }
				}
			}
		}

	return retour;
}

int calculOmbre(Intersection intersection){

    Ray rayon = Ray(intersection.ptIntersection+0.0001f*intersection.normal,lightPos);
  if( raySceneIntersection(rayon).ptIntersection !=  camEyeCartesian ) {
    return 0;
  }
    else return 1;

}

void rayTrace () {
  //Direction dans la quelle regarde la camera
  Vec3f d = camTarget - camEyeCartesian;
  // Calcul de la largeur et de la longueur de l'image centree en camTarget
  float h = 2*(d.length())*tan(fovAngle/2) ;
  float w = aspectRatio*h ;
  // Calcul des pas de parcours de l'image
  Vec3f Dv =  (h/screenHeight)*Vec3f(0.0f,1.0f,0.0f) ;
  Vec3f Du = (w/screenWidth)*Vec3f(1.0f,0.0f,0.0f) ;
  // Calcul de la position du pixel P(0,0)
  Vec3f positionDepart = camTarget - (w/2)*Vec3f(1.0f,0.0f,0.0f) + (h/2)*Vec3f(0.0f,1.0f,0.0f);

  // Debut de la boucle de remplissage de l'image
  for (unsigned int i = 0; i < screenWidth; i++) {
	  for (unsigned int  j = 0; j < screenHeight; j++) {

      // Calcul de la couleur du pixel
	    unsigned int index = 3*(i+j*screenWidth);
      Vec3f positionPixel = positionDepart + Du*i+Dv*j;
      Ray rayon = Ray(camEyeCartesian, positionPixel) ;
      Intersection intersection = raySceneIntersection(rayon) ;

      int val = calculOmbre(intersection);

      Vec3f pixelColor = evaluateResponse(intersection) * val;
    //  else {Vec3f pixelColor = Vec3f(0.0f,0.0f,0.0f);}


      rayImage[index] = pixelColor[0] ;
      rayImage[index+1] = pixelColor[1] ;
      rayImage[index+2] = pixelColor[2];
	    }
    }
}

void display () {
  if (rayDisplayMode)
	displayRayImage ();
  else
	rasterize ();
}

void saveRayImage (const string & filename) {
  if (rayImage != NULL) {
	std::ofstream out (filename.c_str ());
	out << "P3" << endl
		<< screenWidth << " " << screenHeight << endl
		<< "255" << endl;
	for (unsigned int i = 0; i < 3*screenWidth*screenHeight; i++)
	  out << static_cast<int>(rayImage[i]) << " ";
	out.close ();
  }
}

void keyboard (unsigned char keyPressed, int x, int y) {
  switch (keyPressed) {
  case ' ':
	rayDisplayMode = !rayDisplayMode;
	glutPostRedisplay ();
	break;
  case 'r':
	rayTrace ();
	glutPostRedisplay ();
	break;
  case 's':
	saveRayImage ("raytraced_image.ppm");
	break;
  case 'q':
  case 27:
	exit (0);
	break;
  default:
	printUsage ();
	break;
  }
}

void mouse (int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
	if (state == GLUT_DOWN) {
	  mouseLeftButtonClicked = true;
	  clickedX = x;
	  clickedY = y;
	  baseCamPhi = camEyePolar[1];
	  baseCamTheta = camEyePolar[2];
	} else {
	  mouseLeftButtonClicked = false;
	}
  }
}

void motion (int x, int y) {
  if (mouseLeftButtonClicked == true) {
	camEyePolar[1] =  baseCamPhi  + (float (clickedY-y)/screenHeight) * M_PI;
	camEyePolar[2] = baseCamTheta + (float (x-clickedX)/screenWidth) * M_PI;
	glutPostRedisplay (); // calls the display function
  }
}

// This function is executed in an infinite loop.
void idle () {
}

int main (int argc, char ** argv) {
  glutInit (&argc, argv); // Initialize a glut app
  glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); // Setup a RGBA framebuffer to display, with a depth buffer (z-buffer), in double buffer mode (fill a buffer then update the screen)
  glutInitWindowSize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT); // Set the window app size on screen
  window = glutCreateWindow (appTitle.c_str ()); // create the window
  if (argc > 1)
	init (argv[1]); // Your initialization code (OpenGL states, geometry, material, lights, etc)
  else
	init (DEFAULT_SCENE_FILENAME);
  glutReshapeFunc (reshape); // Callback function executed whenever glut need to setup the projection matrix
  glutDisplayFunc (display); // Callback function executed when the window app need to be redrawn
  glutKeyboardFunc (keyboard); // Callback function executed when the keyboard is used
  glutMouseFunc (mouse); // Callback function executed when a mouse button is clicked
  glutMotionFunc (motion); // Callback function executed when the mouse move
  glutIdleFunc (idle); // Callback function executed continuously when no other event happens (good for background procesing or animation for instance).
  printUsage (); // By default, display the usage help of the program
  glutMainLoop ();
  return 0;
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
