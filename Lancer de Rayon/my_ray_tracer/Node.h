#include <cmath>
#include <vector>
#include "Vec3.h"
#include "KdTree.h"
#include "tiny_obj_loader.h"

using namespace std;

class Node {

public :

  vector<int> *leftChild;
  vector<int> *rightChild;
  Vec3f value;
  

} ;
