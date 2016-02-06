#include <cmath>
#include <vector>
#include "Vec3.h"
#include "BoundingBox.h"
#include "tiny_obj_loader.h"

using namespace std;

class KdNode {

public :

  //KdNode leftChild;
  BoundingBox boundingBox ; 
  KdNode * leftChild;
  KdNode *rightChild;
  
  vector<float> feuille ;

  KdNode() {}
  ~KdNode() {}

  KdNode(BoundingBox boundingBox) : boundingBox(boundingBox) {} 

} ;
