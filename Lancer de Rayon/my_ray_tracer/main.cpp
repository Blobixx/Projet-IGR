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

using namespace std;

// App parameters
static const unsigned int DEFAULT_SCREENWIDTH = 500;
static const unsigned int DEFAULT_SCREENHEIGHT = 500;
static const char * DEFAULT_SCENE_FILENAME = "scenes/cornell_box/cornell_box.obj";
static string appTitle ("MCRT - Monte Carlo Ray Tracer");
static GLint window;
static bool rayDisplayMode = false;
static float Pi = 3.14159265359;

// Camera parameters
static float fovAngle = 45.f ;
static float aspectRatio = float(DEFAULT_SCREENWIDTH) / float(DEFAULT_SCREENHEIGHT);
static float nearPlane;
static float farPlane;
static Vec3f camTarget;
static Vec3f camUp = Vec3f(0.0f,1.0f,0.0f);
// Expressing the camera position in polar coordinate, in the frame of the target

// Scene elements
static Vec3f lightPos = Vec3f (1.f);
static Vec3f lightColor = Vec3f (1.f);
static Vec3f sceneCenter = Vec3f (0.f, 0.f, 0.f);
static float sceneRadius = 1.f;
static vector<tinyobj::shape_t> shapes;
static vector<tinyobj::material_t> materials;

// Mouse parameters
static bool mouseLeftButtonClicked = false;
static int clickedX, clickedY;
static float baseCamPhi;
static float baseCamTheta;

static Vec3f camEyePolar = Vec3f(2.f*500.f, M_PI/2.0f, M_PI/2.f);
static Vec3f camEyeCartesian = polarToCartesian(camEyePolar);

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

//retourne la liste de tous les points de la scene
vector<float> getListOfAllPoints(){
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

//renvoie la couleur
Vec3f evaluateResponse(Intersection intersection) {

	Vec3f wi = intersection.ptIntersection - lightPos;
	wi.normalize();
	Vec3f w0 = camEyeCartesian-intersection.ptIntersection;
	w0.normalize();
	Vec3f wh = w0 + wi;
	Vec3f n = intersection.normal;
	wh.normalize();
	n.normalize();

	float norwh = dot(n,wh);
	float norw0 = dot(n,w0);
	float norwi = dot(n,wi);

	Vec3f fd = intersection.diffuse;
	//float alpha = sqrt(2/(intersection.shininess +2));
	float alpha =0.01;

	float D = pow(alpha,2)/(  Pi*pow( 1.f+(pow(alpha,2)-1.f)*pow(norwh,2) ,2) );
	float F = 0.022f + (1.f-0.022f)*pow(  1.f-max(0.f,dot(wi,wh))  ,5);
	float G01 = 2.f*norwi/(norwi+sqrt(pow(alpha,2)+(1.f-pow(alpha,2))*pow(norwi,2)));
	float G02 = 2.f*norw0/(norw0+sqrt(pow(alpha,2)+(1.f-pow(alpha,2))*pow(norw0,2))) ;
	float G = G01*G02;


	Vec3f fs = Vec3f(D*F*G/(4*norwi*norw0));


	// Modele Blinn Phong
	Vec3f fs = ks*pow(norwh,shininess) ;
	
	Vec3f f = fd + fs;

	Vec3f color = lightColor*f*norwi ; //on multipliera par la réflectance dans rayTrace()
	return color ;

}


vector<float> triListe (vector<float>  pointList){
vector<float> pointListRetour;
	for (unsigned int s = 0; s < shapes.size(); s++) {
		//on tourne sur les triangles
		for (unsigned int t = 0; t < shapes[s].mesh.indices.size() / 3; t++) {
			//pour chaque triangle
			//on obtient les float x,y,z des vertices du triangle via index, index+1, index+2
			unsigned int indexV1 = 3*shapes[s].mesh.indices[3*t];
			unsigned int indexV2 = 3*shapes[s].mesh.indices[3*t+1];
			unsigned int indexV3 = 3*shapes[s].mesh.indices[3*t+2];

			for (unsigned int i=0; i<3;i++){
				pointListRetour.push_back(shapes[s].mesh.positions[indexV1+i]);			

			}
			for (unsigned int i=0; i<3;i++){
				pointListRetour.push_back(shapes[s].mesh.positions[indexV2+i]);			

			}
			for (unsigned int i=0; i<3;i++){
				pointListRetour.push_back(shapes[s].mesh.positions[indexV3+i]);			

			}

		}
	}

return pointListRetour ;
}

//calcul de la boite englobatnte miniaml d'une liste de point
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

//algorithme de TriSelection
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

//retourne la coordonnee du point median selon le grand axe de la boite
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

//partie superieur selon le point median
vector<float> upperPartition(vector<float> listPoint, char axeMax, float val) {
int compteur =0;
	vector<float> upperPartition ;
	upperPartition.resize(listPoint.size()) ;
	unsigned int indice =0;
	if (axeMax == 'x') {
		for(unsigned j = 0 ; j<listPoint.size(); j+=3) {
			compteur++;
			if(listPoint[j] > val ) {
//on teste la position du point
if(compteur%3==0){
				upperPartition[indice]=listPoint[j];
				upperPartition[indice+1]=listPoint[j+1];
				upperPartition[indice+2]=listPoint[j+2];

				upperPartition[indice+3]=listPoint[j+3];
				upperPartition[indice+4]=listPoint[j+4];
				upperPartition[indice+5]=listPoint[j+5];

				upperPartition[indice+6]=listPoint[j+6];
				upperPartition[indice+7]=listPoint[j+7];
				upperPartition[indice+8]=listPoint[j+8];

				indice+=9;
				j+=6; }
if(compteur%3==1) {
				upperPartition[indice+3]=listPoint[j];
				upperPartition[indice+4]=listPoint[j+1];
				upperPartition[indice+5]=listPoint[j+2];

				upperPartition[indice]=listPoint[j-3];
				upperPartition[indice+1]=listPoint[j-2];
				upperPartition[indice+2]=listPoint[j-1];

				upperPartition[indice+6]=listPoint[j+3];
				upperPartition[indice+7]=listPoint[j+4];
				upperPartition[indice+8]=listPoint[j+5];

				indice+=9;
				j+=3;	 }

if(compteur%3==2) {
				upperPartition[indice+6]=listPoint[j];
				upperPartition[indice+7]=listPoint[j+1];
				upperPartition[indice+8]=listPoint[j+2];

				upperPartition[indice+3]=listPoint[j-3];
				upperPartition[indice+4]=listPoint[j-2];
				upperPartition[indice+5]=listPoint[j-1];

				upperPartition[indice]=listPoint[j-6];
				upperPartition[indice+1]=listPoint[j-5];
				upperPartition[indice+2]=listPoint[j-4];

				indice+=9; }

			}
		}
	}
	if (axeMax == 'y') {
		for(unsigned j = 1 ; j<listPoint.size(); j+=3) {
			compteur++;
			if(listPoint[j] > val ) {
//on teste la position du point
if(compteur%3==0) {
				upperPartition[indice]=listPoint[j-1];
				upperPartition[indice+1]=listPoint[j];
				upperPartition[indice+2]=listPoint[j+1];

				upperPartition[indice+3]=listPoint[j+2];
				upperPartition[indice+4]=listPoint[j+3];
				upperPartition[indice+5]=listPoint[j+4];

				upperPartition[indice+6]=listPoint[j+5];
				upperPartition[indice+7]=listPoint[j+6];
				upperPartition[indice+8]=listPoint[j+7];

				indice+=9;
				j+=6; }
if(compteur%3==1) {
				upperPartition[indice+3]=listPoint[j-1];
				upperPartition[indice+4]=listPoint[j];
				upperPartition[indice+5]=listPoint[j+1];

				upperPartition[indice]=listPoint[j-4];
				upperPartition[indice+1]=listPoint[j-3];
				upperPartition[indice+2]=listPoint[j-2];

				upperPartition[indice+6]=listPoint[j+2];
				upperPartition[indice+7]=listPoint[j+3];
				upperPartition[indice+8]=listPoint[j+4];

				indice+=9;
				j+=3; }

if(compteur%3==2) {
				upperPartition[indice+6]=listPoint[j-1];
				upperPartition[indice+7]=listPoint[j];
				upperPartition[indice+8]=listPoint[j+1];

				upperPartition[indice+3]=listPoint[j-4];
				upperPartition[indice+4]=listPoint[j-3];
				upperPartition[indice+5]=listPoint[j-2];

				upperPartition[indice]=listPoint[j-7];
				upperPartition[indice+1]=listPoint[j-6];
				upperPartition[indice+2]=listPoint[j-5];

				indice+=9; }

			}
		}
	}
if (axeMax == 'z') {
		for(unsigned j = 2 ; j<listPoint.size(); j+=3) {
			compteur++;
			if(listPoint[j] > val ) {
//on teste la position du point
if(compteur%3==0) {
				upperPartition[indice]=listPoint[j-2];
				upperPartition[indice+1]=listPoint[j-1];
				upperPartition[indice+2]=listPoint[j];

				upperPartition[indice+3]=listPoint[j+1];
				upperPartition[indice+4]=listPoint[j+2];
				upperPartition[indice+5]=listPoint[j+3];

				upperPartition[indice+6]=listPoint[j+4];
				upperPartition[indice+7]=listPoint[j+5];
				upperPartition[indice+8]=listPoint[j+6];

				indice+=9;
				j+=6; }
if(compteur%3==1) {
				upperPartition[indice+3]=listPoint[j-2];
				upperPartition[indice+4]=listPoint[j-1];
				upperPartition[indice+5]=listPoint[j];

				upperPartition[indice]=listPoint[j-5];
				upperPartition[indice+1]=listPoint[j-4];
				upperPartition[indice+2]=listPoint[j-3];

				upperPartition[indice+6]=listPoint[j+1];
				upperPartition[indice+7]=listPoint[j+2];
				upperPartition[indice+8]=listPoint[j+3];

				indice+=9;
				j+=3; }

if(compteur%3==2) {
				upperPartition[indice+6]=listPoint[j-2];
				upperPartition[indice+7]=listPoint[j-1];
				upperPartition[indice+8]=listPoint[j];

				upperPartition[indice+3]=listPoint[j-3];
				upperPartition[indice+4]=listPoint[j-2];
				upperPartition[indice+5]=listPoint[j-1];

				upperPartition[indice]=listPoint[j-6];
				upperPartition[indice+1]=listPoint[j-5];
				upperPartition[indice+2]=listPoint[j-4];

				indice+=9; }

			}
		}
	}

	return upperPartition ;
}

//partie  inferieure
vector<float> lowerPartition(vector<float> listPoint, char axeMax, float val) {
int compteur =0;
	vector<float> lowerPartition ;
	lowerPartition.resize(listPoint.size()) ;
	unsigned int indice =0;
	if (axeMax == 'x') {
		for(unsigned j = 0 ; j<listPoint.size(); j+=3) {
			compteur++;
			if(listPoint[j] < val ) {
//on teste la position du point
if(compteur%3==0){
				lowerPartition[indice]=listPoint[j];
				lowerPartition[indice+1]=listPoint[j+1];
				lowerPartition[indice+2]=listPoint[j+2];

				lowerPartition[indice+3]=listPoint[j+3];
				lowerPartition[indice+4]=listPoint[j+4];
				lowerPartition[indice+5]=listPoint[j+5];

				lowerPartition[indice+6]=listPoint[j+6];
				lowerPartition[indice+7]=listPoint[j+7];
				lowerPartition[indice+8]=listPoint[j+8];

				indice+=9;
				j+=6; }
if(compteur%3==1) {
				lowerPartition[indice+3]=listPoint[j];
				lowerPartition[indice+4]=listPoint[j+1];
				lowerPartition[indice+5]=listPoint[j+2];

				lowerPartition[indice]=listPoint[j-3];
				lowerPartition[indice+1]=listPoint[j-2];
				lowerPartition[indice+2]=listPoint[j-1];

				lowerPartition[indice+6]=listPoint[j+3];
				lowerPartition[indice+7]=listPoint[j+4];
				lowerPartition[indice+8]=listPoint[j+5];

				indice+=9;
				j+=3;	 }

if(compteur%3==2) {
				lowerPartition[indice+6]=listPoint[j];
				lowerPartition[indice+7]=listPoint[j+1];
				lowerPartition[indice+8]=listPoint[j+2];

				lowerPartition[indice+3]=listPoint[j-3];
				lowerPartition[indice+4]=listPoint[j-2];
				lowerPartition[indice+5]=listPoint[j-1];

				lowerPartition[indice]=listPoint[j-6];
				lowerPartition[indice+1]=listPoint[j-5];
				lowerPartition[indice+2]=listPoint[j-4];

				indice+=9; }

			}
		}
	}
	if (axeMax == 'y') {
		for(unsigned j = 1 ; j<listPoint.size(); j+=3) {
			compteur++;
			if(listPoint[j] < val ) {
//on teste la position du point
if(compteur%3==0) {
				lowerPartition[indice]=listPoint[j-1];
				lowerPartition[indice+1]=listPoint[j];
				lowerPartition[indice+2]=listPoint[j+1];

				lowerPartition[indice+3]=listPoint[j+2];
				lowerPartition[indice+4]=listPoint[j+3];
				lowerPartition[indice+5]=listPoint[j+4];

				lowerPartition[indice+6]=listPoint[j+5];
				lowerPartition[indice+7]=listPoint[j+6];
				lowerPartition[indice+8]=listPoint[j+7];

				indice+=9;
				j+=6; }
if(compteur%3==1) {
				lowerPartition[indice+3]=listPoint[j-1];
				lowerPartition[indice+4]=listPoint[j];
				lowerPartition[indice+5]=listPoint[j+1];

				lowerPartition[indice]=listPoint[j-4];
				lowerPartition[indice+1]=listPoint[j-3];
				lowerPartition[indice+2]=listPoint[j-2];

				lowerPartition[indice+6]=listPoint[j+2];
				lowerPartition[indice+7]=listPoint[j+3];
				lowerPartition[indice+8]=listPoint[j+4];

				indice+=9;
				j+=3; }

if(compteur%3==2) {
				lowerPartition[indice+6]=listPoint[j-1];
				lowerPartition[indice+7]=listPoint[j];
				lowerPartition[indice+8]=listPoint[j+1];

				lowerPartition[indice+3]=listPoint[j-4];
				lowerPartition[indice+4]=listPoint[j-3];
				lowerPartition[indice+5]=listPoint[j-2];

				lowerPartition[indice]=listPoint[j-7];
				lowerPartition[indice+1]=listPoint[j-6];
				lowerPartition[indice+2]=listPoint[j-5];

				indice+=9; }

			}
		}
	}
if (axeMax == 'z') {
		for(unsigned j = 2 ; j<listPoint.size(); j+=3) {
			compteur++;
			if(listPoint[j] < val ) {
//on teste la position du point
if(compteur%3==0) {
				lowerPartition[indice]=listPoint[j-2];
				lowerPartition[indice+1]=listPoint[j-1];
				lowerPartition[indice+2]=listPoint[j];

				lowerPartition[indice+3]=listPoint[j+1];
				lowerPartition[indice+4]=listPoint[j+2];
				lowerPartition[indice+5]=listPoint[j+3];

				lowerPartition[indice+6]=listPoint[j+4];
				lowerPartition[indice+7]=listPoint[j+5];
				lowerPartition[indice+8]=listPoint[j+6];

				indice+=9;
				j+=6; }
if(compteur%3==1) {
				lowerPartition[indice+3]=listPoint[j-2];
				lowerPartition[indice+4]=listPoint[j-1];
				lowerPartition[indice+5]=listPoint[j];

				lowerPartition[indice]=listPoint[j-5];
				lowerPartition[indice+1]=listPoint[j-4];
				lowerPartition[indice+2]=listPoint[j-3];

				lowerPartition[indice+6]=listPoint[j+1];
				lowerPartition[indice+7]=listPoint[j+2];
				lowerPartition[indice+8]=listPoint[j+3];

				indice+=9;
				j+=3; }

if(compteur%3==2) {
				lowerPartition[indice+6]=listPoint[j-2];
				lowerPartition[indice+7]=listPoint[j-1];
				lowerPartition[indice+8]=listPoint[j];

				lowerPartition[indice+3]=listPoint[j-3];
				lowerPartition[indice+4]=listPoint[j-2];
				lowerPartition[indice+5]=listPoint[j-1];

				lowerPartition[indice]=listPoint[j-6];
				lowerPartition[indice+1]=listPoint[j-5];
				lowerPartition[indice+2]=listPoint[j-4];

				indice+=9; }

			}
		}
	}
	return lowerPartition ;
}

KdNode buildKdTree(vector<float> pointList) {

  vector<float> upperPart;
  vector<float> lowerPart;
  BoundingBox boundingBox ;
  KdNode node ;
    //la dernière boundingBox est composée de 30 points (10 triangles)
    if(pointList.size() > 90) {
      boundingBox = computeBoundingBox(pointList) ;
	node = KdNode(boundingBox);
      char maxAxe = boundingBox.maxAxis() ;
      float point = findMedianSample(boundingBox,pointList) ;
      vector<float> upperPart = upperPartition(pointList, maxAxe, point) ;
      vector<float> lowerPart = lowerPartition(pointList, maxAxe, point) ;

      *(node.leftChild) = buildKdTree(upperPart);
      *(node.rightChild) = buildKdTree(lowerPart);
	return node; 
    }
    else{
      BoundingBox boundingBox1 = computeBoundingBox(upperPart);
      BoundingBox boundingBox2 = computeBoundingBox(lowerPart);
	KdNode node1 = KdNode(boundingBox1);
	KdNode node2 = KdNode(boundingBox2);
	KdNode * node11 = &node1;
	KdNode * node22 = &node2;
	
      node.leftChild= node11;
      node.rightChild= node22;

      node1.feuille = upperPart;
      node2.feuille = lowerPart;
	return node; 
   }
}

Intersection raySceneIntersection(Ray ray) {

	//diffuse
	Vec3f valDiffuse = Vec3f(0.f,1.f,0.0f);
	float valShininess = 5.f;
	Vec3f valSpecular = Vec3f(1.0f) ;

	//chaque intersection est suivie de sa normale dans la liste;
	Intersection retour = Intersection(camEyeCartesian, Vec3f(0.f,0.f,0.f), valDiffuse, 5.f, Vec3f(1.0f));
	float distanceMin = 1000000.f;


	for (unsigned int s = 0; s < shapes.size (); s++) {
		//on tourne sur les triangles
		for (unsigned int t = 0; t < shapes[s].mesh.indices.size() / 3; t++) {

			//on recupere le diffuse du mesh
			if (!materials.empty ()) {
				unsigned int m = shapes[s].mesh.material_ids[t];
				valDiffuse = Vec3f(materials[m].diffuse[0], materials[m].diffuse[1], materials[m].diffuse[2]);
				valShininess = materials[m].shininess;
				valSpecular = Vec3f(materials[m].specular[0], materials[m].specular[1], materials[m].specular[2]);
			}
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
			Intersection intersection = ray.rayTriangleIntersection(vertex1, vertex2, vertex3);

			//on retournera celle dont la distance est minimale
			float d = dist(intersection.ptIntersection, camEyeCartesian);
			if(intersection.intersect) {
				if(  d < distanceMin){
					distanceMin = d;
					retour = Intersection(intersection.ptIntersection, intersection.normal, valDiffuse, valShininess, valSpecular, true);
				}
			}

		}
	}
	return retour;
}

int calculOmbre(Intersection intersection){

	Vec3f pointDecale = intersection.ptIntersection+0.0001f*intersection.normal;
	Ray rayon = Ray(pointDecale,lightPos-pointDecale);
	if( !raySceneIntersection(rayon).intersect ) {
		return 0;
	}
	return 1;
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

	for (unsigned int s = 0; s < shapes.size (); s++)
	{
		for (unsigned int p = 0; p < shapes[s].mesh.positions.size () / 3; p++)
		{
			shapes[s].mesh.positions[3*p] -= sceneCenter[0];
			shapes[s].mesh.positions[3*p+1] -= sceneCenter[1];
			shapes[s].mesh.positions[3*p+2] -= sceneCenter[2];
		}
	}
	sceneCenter = Vec3f (0.f, 0.f, 0.f);

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
	lightPos = Vec3f (sceneRadius, sceneRadius, sceneRadius);
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

	camEyeCartesian = eye;

	gluLookAt (eye[0], eye[1], eye[2],
			camTarget[0], camTarget[1], camTarget[2],
			0.0, 1.0, 0.0); // Set up the current modelview matrix with camera transform
}

void reshape (int w, int h) {
	aspectRatio = static_cast<float>(w)/static_cast<float>(h);
	glViewport (0, 0, (GLint)w, (GLint)h); // Dimension of the drawing region in the window
	setupCamera ();
	if (rayImage != NULL)
		delete [] rayImage;
	unsigned int l = 3*DEFAULT_SCREENWIDTH*DEFAULT_SCREENHEIGHT;
	rayImage = new unsigned char [l];
	memset (rayImage, 177, l);
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
	glDrawPixels (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, GL_RGB, GL_UNSIGNED_BYTE, static_cast<void*>(rayImage));
	glutSwapBuffers ();
	glEnable (GL_DEPTH_TEST);
}

void rayTrace () {

	//Direction dans la quelle regarde la camera
	Vec3f d = camTarget - camEyeCartesian;

	// Repere de l'image norme
	Vec3f directionU = cross(d, camUp) ;
	Vec3f directionV = cross(-d, directionU ) ;
	directionU.normalize() ;
	directionV.normalize() ;
	// Calcul de la largeur et de la longueur de l'image centree en camTarget
	float h = 2*d.length()*tan(fovAngle * Pi / 360.0) ;
	float w = aspectRatio*h ;
	// Calcul des pas de parcours de l'image
	Vec3f Dv =  directionV*(h/(DEFAULT_SCREENHEIGHT - 1));
	Vec3f Du = directionU*(w/(DEFAULT_SCREENWIDTH - 1));

	// Calcul de la position du pixel P(0,0)
	Vec3f positionDepart = camTarget - directionV * h / 2.0f - directionU * w / 2.0f;
	// Debut de la boucle de remplissage de l'image
	for (unsigned int i = 0; i < DEFAULT_SCREENWIDTH; i++) {
		for (unsigned int  j = 0; j < DEFAULT_SCREENHEIGHT; j++) {

			// Calcul de la couleur du pixel
			unsigned int index = 3*(i+j*DEFAULT_SCREENWIDTH);
			Vec3f positionPixel = positionDepart + Du*i+Dv*j;

			Ray rayon = Ray(camEyeCartesian, positionPixel-camEyeCartesian) ;
			Intersection intersection = raySceneIntersection(rayon) ;
			Vec3f pixelColor = evaluateResponse(intersection);
			rayImage[index] = pixelColor[0] * 255;
			rayImage[index+1] = pixelColor[1] * 255;
			rayImage[index+2] = pixelColor[2] * 255;
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
			<< DEFAULT_SCREENWIDTH << " " << DEFAULT_SCREENHEIGHT << endl
			<< "255" << endl;
		for (unsigned int i = 0; i < 3*DEFAULT_SCREENWIDTH*DEFAULT_SCREENHEIGHT; i++)
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
		camEyePolar[1] =  baseCamPhi  + (float (clickedY-y)/DEFAULT_SCREENHEIGHT) * M_PI;
		camEyePolar[2] = baseCamTheta + (float (x-clickedX)/DEFAULT_SCREENWIDTH) * M_PI;
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
