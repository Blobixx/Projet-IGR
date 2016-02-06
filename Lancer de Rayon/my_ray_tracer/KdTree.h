#include <cmath>
#include <vector>
#include "Vec3.h"
#include "BoundingBox.h"
#include "tiny_obj_loader.h"

using namespace std;

class KdTree {

public:
	static vector<tinyobj::shape_t> shapes;
	
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

};
