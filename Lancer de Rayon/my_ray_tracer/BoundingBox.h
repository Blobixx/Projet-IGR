#include <cmath>
#include <vector>
#include "Vec3.h"
#include "KdNode.h"
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
  BoundingBox(float xMin, float xMax,float yMin, float yMax,float zMin, float zMax ) ;
  virtual ~BoundingBox() {}

  float getXMin (BoundingBox boundingBox) {
    return boundingBox.xMin ;
  }
  float getXMax (BoundingBox boundingBox) {
    return boundingBox.xMax ;
  }
  float getYMin (BoundingBox boundingBox) {
    return boundingBox.yMin ;
  }
  float getYMax (BoundingBox boundingBox) {
    return boundingBox.yMax ;
  }
  float getZMin (BoundingBox boundingBox) {
    return boundingBox.zMin ;
  }
  float getZMax (BoundingBox boundingBox) {
    return boundingBox.zMax ;
  }

  char BoundingBox::maxAxis(BoundingBox boundingBox) ;

} ;

char BoundingBox::maxAxis(BoundingBox boundingBox) {
  float xMin = boundingBox.getXMin(boundingBox);
  float xMax = boundingBox.getXMax(boundingBox);
  float yMin = boundingBox.getYMin(boundingBox);
  float yMax = boundingBox.getYMax(boundingBox);
  float zMin = boundingBox.getZMin(boundingBox);
  float zMax = boundingBox.getZMax(boundingBox);

  float distX = xMax - xMin;
  float distY = yMax - yMin;
  float distZ = zMax - zMin;

  //axe choisi
  float tmp = max(distX,distY);
  float axis = max(tmp,distZ);

  if (axis = distX) {
    return 'x' ;
  }
  if (axis = distY) {
    return 'y' ;
  }
  if (axis = distZ) {
    return 'z' ;
  }
}
