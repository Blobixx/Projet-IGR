#ifndef _BOUNDINGBOX_H
#define _BOUNDINGBOX_H

#include <cmath>
#include <vector>
#include "Vec3.h"
#include "tiny_obj_loader.h"
#include "Face.h"

using namespace std;

class BoundingBox {

  public :
  float xMin ;
  float xMax ;
  float yMin ;
  float yMax ;
  float zMin ;
  float zMax ;

  BoundingBox() {}
  BoundingBox(float xMin, float xMax,float yMin, float yMax,float zMin, float zMax ){
    this->xMin=xMin;
    this->xMax=xMax;
    this->yMin=yMin;
    this->yMax=yMax;
    this->zMin=zMin;
    this->zMax=zMax ;
  }
  virtual ~BoundingBox() {}

  float getXMin () {
    return this->xMin ;
  }
  float getXMax () {
    return this->xMax ;
  }
  float getYMin () {
    return this->yMin ;
  }
  float getYMax () {
    return this->yMax ;
  }
  float getZMin () {
    return this->zMin ;
  }
  float getZMax () {
    return this->zMax ;
  }

  char maxAxis() ;
vector<Face> createBox();

} ;



//renvoie la char correspondant a l'axe le plus grand
char BoundingBox::maxAxis() {
  float xMin = this->getXMin();
  float xMax = this->getXMax();
  float yMin = this->getYMin();
  float yMax = this->getYMax();
  float zMin = this->getZMin();
  float zMax = this->getZMax();

  float distX = xMax - xMin;
  float distY = yMax - yMin;
  float distZ = zMax - zMin;

  //axe choisi
  float tmp = max(distX,distY);
  float axis = max(tmp,distZ);

  if (axis == distX) {
    return 'x' ;
  }
  if (axis == distY) {
    return 'y' ;
  }
  if (axis == distZ) {
    return 'z' ;
  }

    return 'o' ;

}

vector<Face> BoundingBox::createBox(){
	vector<Face> listeFace ;
	float xMin = this->xMin;
	float xMax = this->xMax;
	float yMin = this->yMin;
	float yMax = this->yMax;
	float zMin = this->zMin;
	float zMax = this->zMax;

	Vec3f p1 = Vec3f(xMin, yMax,zMax);
	Vec3f p2 = Vec3f(xMax, yMax,zMax);
	Vec3f p3 = Vec3f(xMax, yMin,zMax);
	Vec3f p4 = Vec3f(xMin, yMin,zMax);
	Vec3f p5 = Vec3f(xMax, yMin,zMin);
	Vec3f p6 = Vec3f(xMax, yMax,zMin);
	Vec3f p7 = Vec3f(xMin, yMax,zMin);
	Vec3f p8 = Vec3f(xMin, yMin,zMin);
	
	Face face1 = Face(p1,p2,p3,p4);
	Face face2 = Face(p2,p6,p5,p3);
	Face face3 = Face(p6,p5,p8,p7);
	Face face4 = Face(p7,p1,p4,p8);
	Face face5 = Face(p4,p8,p5,p3);
	Face face6 = Face(p1,p7,p6,p2);

	listeFace.push_back(face1);
	listeFace.push_back(face2);
	listeFace.push_back(face3);
	listeFace.push_back(face4);
	listeFace.push_back(face5);
	listeFace.push_back(face6);

	listeFace.resize(6);

return listeFace;

}

#endif
