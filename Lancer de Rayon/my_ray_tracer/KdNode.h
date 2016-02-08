// Classe qui definit un noeud d'un KdTree
#ifndef _KDNODE_H
#define _KDNODE_H

#include <cmath>
#include <vector>
#include "Vec3.h"
#include "BoundingBox.h"
#include "tiny_obj_loader.h"

using namespace std;

class KdNode {

public :

  BoundingBox boundingBox ; 
  KdNode * leftChild = NULL;
  KdNode *rightChild = NULL;
//intersection rayon - BBox
  bool intersection ;
  
  vector<float> feuille ;

  KdNode() {}
  ~KdNode() {}

  KdNode(BoundingBox boundingBox,  bool intersection = false) : boundingBox(boundingBox) {} 

} ;

#endif
