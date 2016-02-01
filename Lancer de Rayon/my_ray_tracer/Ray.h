// Classe des rayons comportant 1 origine et 1 direction

#include <cmath>
#include <vector>
#include "Vec3.h"
#include "tiny_obj_loader.h"

using namespace std;

static Vec3f camPosPolar = Vec3f(2.f*1.f, M_PI/2.f, M_PI/2.f);
static Vec3f camPos = polarToCartesian(camPosPolar) ;


class Ray {

public:
	Vec3f origin;
	Vec3f direction;

	Ray(Vec3f origin, Vec3f direction){
		this->origin = origin;
		this->direction = direction;
	}

	vector<Vec3f> RayTriangleIntersection(Vec3f p0 , Vec3f p1,  Vec3f p2);

};



vector<Vec3f> Ray::RayTriangleIntersection(Vec3f p0 , Vec3f p1,  Vec3f p2) {

	vector<Vec3f> tableauRetour;
	tableauRetour.push_back(Vec3f(0.0f,0.0f,0.0f));
	tableauRetour.push_back(Vec3f(0.0f,0.0f,0.0f));
  tableauRetour.resize(2);

	Vec3f e0 = p1 - p0 ;
	Vec3f e1 = p2 - p0 ;
	direction.normalize();
	Vec3f normale = cross(e0,e1) ;
	normale.normalize() ;
	Vec3f q = cross( direction, e1) ;
	float a = dot(e0,q) ;

	if ( (dot(normale,direction)>=0) ) {
		return  tableauRetour;
	}

	Vec3f s = (origin - p0)/a ;
	Vec3f r = cross(s,e0) ;
	float b0 = dot(s,q) ;
	float b1 = dot(r,direction) ;
	float b2 = 1.0-b0-b1 ;

	if ( (b0 < 0.0) | (b1<0.0) | (b2 < 0.0)) {
		return tableauRetour ;
		}

		float t = dot(e1,r) ;
		if ( t>=0)
		tableauRetour.clear();
		tableauRetour.push_back(p0*b0+p1*b1+p2*b2);
		tableauRetour.push_back(normale);
    tableauRetour.resize(2);
		return tableauRetour;


		return tableauRetour ;
}
