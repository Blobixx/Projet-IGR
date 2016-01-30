// Classe des rayons comportant 1 origine et 1 direction

#include <cmath>
#include "Vec3.h"
#include "tiny_obj_loader.h"

static Vec3f camPos = Vec3f(2.f*1.f, M_PI/2.f, M_PI/2.f);

using namespace std;

class Ray {

public:
	Vec3f origin;
	Vec3f direction;

	Ray(Vec3f origin, Vec3f direction){
		this->origin = origin;
		this->direction = direction;
	}

};

Vec3f RayTriangleintersection(Vec3f p0 , Vec3f p1,  Vec3f p2) {

	Vec3f e0 = p1 - p0 ;
	direction.normalize() ;
	Vec3f e1 = p2 - p0 ;
	Vec3f normale = cross(e0,e1) ;
	normale.normalize() ;
	Vec3f q = cross( direction, e1) ;
	float a = dot(e0,q) ;

	if ( (dot(normale,direction)>=0) ) {
		return Vec3f(0.0f,0.0f,0.0f) ;
	}

	Vec3f s = (origin - p0)/a ;
	Vec3f r = cross(s,e0) ;
	float b0 = dot(s,q) ;
	float b1 = dot(r,direction) ;
	float b2 = 1.0-b0-b1 ;

	if ( (b0 < 0.0) | (b1<0.0) | (b2 < 0.0)) {
		return Vec3f(0.0f,0.0f,0.0f) ;
		}

		float t = dot(e1,r) ;
		if ( t>=0)
		return p0*b0+p1*b1+p2*b2 ;

		return Vec3f(0.0f,0.0f,0.0f) ;
}

Vec3f raySceneIntersection(Ray ray) {

	vector<Vec3f> listeIntersections ;
	vector<float> listeDistances;
	for (unsigned int s = 0; s < shapes.size (); s++) {
		for (unsigned int p = 0; p < shapes[s].mesh.positions.size() / 3; p++) {
			Vec3f vecteurCourant = Vec3f(shapes[s].mesh.positions[3*p], shapes[s].mesh.positions[3*p+1], shapes[s].mesh.positions[3*p+2]) ;
			Vec3f ptIntersection = ray.RayTriangleintersection(vecteurCourant[0], vecteurCourant[1], vecteurCourant[2]) ;
			if(ptIntersection != Vec3f(0.0f,0.0f,0.0f)) {
				listeIntersections.push_back(ptIntersection);
				listeDistances.push_back(dist(ptIntersection, camPos);
			}
		}
	}
	float d = 100000 ;
	unsigned int indice = 0;
	for(unsigned i = 0; i<listeDistances.size(); i++) {
		if (listeDistances[i] < d) {
			d = listeDistances[i] ;
			indice = i ;
		}
	}
	return listeIntersections[indice] ;
}
