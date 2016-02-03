#include <cmath>
#include <vector>
#include "Vec3.h"
#include "tiny_obj_loader.h"

using namespace std;

class BoundingBox {

  public :
  float xMin ;
  float xMax ;
  float yMin ;
  float yMax ;
  float zMin ;
  float zMax ;

  BoundingBox() ;
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
