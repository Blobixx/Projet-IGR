// Classe des rayons comportant 1 origine et 1 direction

#include <cmath>
#include <vector>
#include "Vec3.h"
#include "tiny_obj_loader.h"
#include "Intersection.h"

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

};


//renvoie l'intersection entre le rayon et un triangle
Intersection Ray::rayTriangleIntersection(Vec3f p0 , Vec3f p1,  Vec3f p2) {

  Intersection intersection = Intersection(Vec3f(0.0f), Vec3f(1.f), Vec3f(0.f), 5.f);

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
	  //		return Intersection(p0*b0+p1*b1+p2*b2,normale);
	  //return Intersection(b0*p0+b1*p1+b2*p2,normale, Vec3f(1.f),5.f, true);
	  return Intersection(t*direction + origin,normale, Vec3f(1.f),5.f, true);
	}
	return intersection ;
}
