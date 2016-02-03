#include <cmath>
#include <vector>
#include "Vec3.h"
#include "tiny_obj_loader.h"

using namespace std;

class KdNode {

public :

  //KdNode leftChild;
  vector<int> * leftChild;
  vector<int> *rightChild;

} ;
