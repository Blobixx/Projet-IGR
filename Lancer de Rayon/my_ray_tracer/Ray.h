// Classe des rayons comportant 1 origine et 1 direction

#ifndef _RAY_H
#define _RAY_H

#include <cmath>
#include <vector>
#include "Vec3.h"
#include "tiny_obj_loader.h"
#include "Intersection.h"
#include "Face.h" 
#include "KdNode.h"



using namespace std;

class Ray {

public:
	Vec3f origin;
	Vec3f direction;

	Ray (){
	
	}
	
	Ray(Vec3f origin, Vec3f direction){
		this->origin = origin;
		this->direction = direction;
	}

	Intersection rayTriangleIntersection(Vec3f p0 , Vec3f p1,  Vec3f p2);
	bool rayRectangleIntersection(Face face);
	bool rayTriangleIntersectionBool(Vec3f p0 , Vec3f p1,  Vec3f p2);
	vector<float> parcoursTree(KdNode node);
};



bool Ray::rayRectangleIntersection(Face face){
	bool b = false;
 	if((rayTriangleIntersectionBool(face.sommet1 , face.sommet2,face.sommet3) )|| (rayTriangleIntersectionBool(face.sommet1 ,face.sommet3, face.sommet4)) ){b=true;}
	return b;
}

vector<float> Ray::parcoursTree(KdNode node){
	vector<float> listeTriangles;
	BoundingBox boundingBox = node.boundingBox ; 

	vector<Face> listeFaces = boundingBox.createBox();

	
	for(unsigned int i=0;i<6;i++) {
		if(this->rayRectangleIntersection(listeFaces[i])) {
			node.intersection = true;
			}
	}

	if(node.feuille.empty()){
		if(node.intersection){
			parcoursTree(*(node.leftChild));
			parcoursTree(*(node.rightChild));
    	      }
		else {
		listeTriangles.resize(1);
		return listeTriangles ;
		}

	}
	else {return node.feuille;}
}

bool Ray::rayTriangleIntersectionBool(Vec3f p0 , Vec3f p1,  Vec3f p2) {

	Vec3f e0 = p1 - p0 ;
	Vec3f e1 = p2 - p0 ;
	direction.normalize();
	Vec3f normale= cross(e0,e1) ;
	normale.normalize();
	Vec3f q = cross( direction, e1) ;
	float a = dot(e0,q) ;
	if ( dot(normale,direction)>=0 || fabs(a)<0.000000001f) {
		return  false;
	}

	Vec3f s = (origin - p0)/a ;
	Vec3f r = cross(s,e0) ;
	float b0 = dot(s,q) ;
	float b1 = dot(r,direction) ;
	float b2 = 1.0f-b0-b1 ;

	if ((b0 < 0) || (b1 < 0) || (b2 < 0)) {
		return false ;
	}

	float t = dot(e1,r) ;
	if ( t>=0) {
	  return true ;
	}
	return false ;
}


//renvoie l'intersection entre le rayon et un triangle
Intersection Ray::rayTriangleIntersection(Vec3f p0 , Vec3f p1,  Vec3f p2) {

  Intersection intersection = Intersection(Vec3f(0.0f), Vec3f(1.f), Vec3f(0.f), 5.f, Vec3f(1.0f),Vec3f(1.f));

	Vec3f e0 = p1 - p0 ;
	Vec3f e1 = p2 - p0 ;
	direction.normalize();
	Vec3f normale= cross(e0,e1) ;
	normale.normalize();
	Vec3f q = cross( direction, e1) ;
	float a = dot(e0,q) ;
	if ( dot(normale,direction)>=0 || fabs(a)<0.000000001f) {
		return  intersection;
	}

	Vec3f s = (origin - p0)/a ;
	Vec3f r = cross(s,e0) ;
	float b0 = dot(s,q) ;
	float b1 = dot(r,direction) ;
	float b2 = 1.0f-b0-b1 ;

	if ((b0 < 0) || (b1 < 0) || (b2 < 0)) {
		return intersection ;
	}

	float t = dot(e1,r) ;
	if ( t>=0) {
	  //return Intersection(b0*p0+b1*p1+b2*p2,normale, Vec3f(1.f),5.f, true);
	  return Intersection(t*direction + origin,normale, Vec3f(1.f),5.f, Vec3f(1.0f), Vec3f(1.f),false,true);
	}
	return intersection ;
}

#endif
