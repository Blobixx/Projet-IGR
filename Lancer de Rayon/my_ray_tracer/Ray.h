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

	Ray(Vec3f origin, Vec3f direction){
		this->origin = origin;
		this->direction = direction;
	}

	Intersection rayTriangleIntersection(Vec3f p0 , Vec3f p1,  Vec3f p2);
	Intersection rayRectangleIntersection(Face face);

	vector<float> parcoursTree(KdNode &node);
};



Intersection Ray::rayRectangleIntersection(Face face){
	Intersection intersection = Intersection(Vec3f(0.0f), Vec3f(1.f), Vec3f(0.f), 5.f, Vec3f(1.0f));

 	Intersection intersection1 = this->rayTriangleIntersection(face.sommet1, face.sommet2, face.sommet3) ;
	Intersection intersection2 = this->rayTriangleIntersection(face.sommet1, face.sommet3, face.sommet4) ;

	if (intersection1.intersect) {
		return intersection1 ;
	}
	if (intersection2.intersect) {
		return intersection2 ;
	}
	else return intersection ;
}

vector<float> Ray::parcoursTree(KdNode &node){
	vector<float> listeTriangles;
	BoundingBox boundingBox = node.boundingBox ; 

	vector<Face> listeFaces = boundingBox.createBox();

	for(unsigned int i=0;i<6;i++) {
		if(this->rayRectangleIntersection(listeFaces[i]).intersect) {
			node.intersection = true;
		}
	}

	if(node.intersection){
		if(node.feuille.empty()){
			node = *(node.leftChild) ; 
			parcoursTree(node);}
		else {return node.feuille;}
	}
	else if(!node.intersection) {
		if(node.feuille.empty()){
			node = *(node.rightChild) ; 
			parcoursTree(node);	
	      }
		else {return node.feuille;}
    	}
	/*else {
		listeTriangles.resize(1);
		return listeTriangles ;
	}*/
}

//renvoie l'intersection entre le rayon et un triangle
Intersection Ray::rayTriangleIntersection(Vec3f p0 , Vec3f p1,  Vec3f p2) {

  Intersection intersection = Intersection(Vec3f(0.0f), Vec3f(1.f), Vec3f(0.f), 5.f, Vec3f(1.0f));

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
	  return Intersection(t*direction + origin,normale, Vec3f(1.f),5.f, Vec3f(1.0f), true);
	}
	return intersection ;
}

#endif
