#include <cmath>
#include <vector>
#include "Vec3.h"
#include "tiny_obj_loader.h"
#include "BoundingBox.h"


using namespace std;

class KdNode {

public :

  KdNode leftChild;
  vector<int> *rightChild;

  vector<float> createListPointOrigine() ;
  BoundingBox computeBoundingBox(vector<float> pointList) ;
  vector<float> TriSelection(vector<float> liste,unsigned int n) ;
  float findMedianSample(BoundingBox boundingBox, vector<float> pointList) ;
  vector<float> upperPartition(vector<float> listPoint, char axeMax, float val) ;
  vector<float> lowerPartition(vector<float> listPoint, char axeMax, float val) ;

} ;

vector<float> KdNode::createListPointOrigine() {

  for (unsigned int s = 0; s < shapes.size (); s++){
     for (unsigned int p = 0; p < shapes[s].mesh.positions.size () / 3; p++) {
       pointList.resize(shapes[s].mesh.position.size()) ;
       pointList.push_back(shapes[s].mesh.positions[3*p]) ;
       pointList.push_back(shapes[s].mesh.positions[3*p+1]) ;
       pointList.push_back(shapes[s].mesh.positions[3*p]+2) ;

       return pointList ;
     }
   }
}


BoundingBox KdNode::computeBoundingBox(vector<float> pointList) {
  float xMin = 10000.f;
  float xMax = -10000.f;
  float yMin = 10000.f;
  float yMax = -10000.f;
  float zMin = 10000.f;
  float zMax = -10000.f;

  for(unsigned i =0; i<pointlist. )
}

vector<float> KdNode::TriSelection(vector<float> liste,unsigned int n) {
	for(unsigned int i=0 ; i < n ; i++) {				//on parcourt le tableau
		unsigned int p = i ;
		for( unsigned int j = i ; j < n ; j++) {			//on cherche la position p du plus petit élément
			if(liste[j] < liste[p] ) {
        p=j;
      }
			//resultat.nbComparaisons++;	//comptage des opérations
		}
	float c = liste[i] ;						//on échange
	liste[i] = liste[p] ;					//les deux
	liste[p] = c ;						//nombres
	//resultat.nbAffectations+=2;			//comptage des opérations
  }
  vector<float> listeRetour ;
  listeRetour.resize(n) ;
  for(unsigned int k = 0 ; k < n ; k++) {
    liste[k] = listeRetour[k] ;
  }
  return listeRetour ;
}

float KdNode::findMedianSample(BoundingBox boundingBox, vector<float> pointList) {
  char axeMax = maxAxis(boundingBox) ;
  int sizeList = pointList.size()/3 ;
  vector<float> listeATrier ;
  listeATrier.resize(pointList.size()/3) ;
  unsigned int i = 0 ;

  if (axeMax = 'x') {
    for(unsigned j = 0 ; j<pointList.size(); j+=3) {
      listeATrier[i] = pointList[j] ;
      i++ ;
    }
  }
  if (axeMax = 'y') {
    for(unsigned j = 1 ; j<pointList.size(); j+=3) {
      listeATrier[i] = pointList[j] ;
      i++ ;
    }
  }
  if (axeMax = 'z') {
    for(unsigned j = 2 ; j<pointList.size(); j+=3) {
      listeATrier[i] = pointList[j] ;
      i++ ;
    }
  }

  vector<float> listeTrier ;
  listeTrier = TriSelection(listeATrier, sizeList) ;

  int indiceMedian = floor(listeTrier.size()/2) +1 ;

  return listeTrier[indiceMedian] ;

}

vector<float> upperPartition(vector<float> listPoint, char axeMax, float val) {

    vector<float> upperPartition ;
    upperPartition.resize(listPoint.size()) ;
    unsigned int indice =0;
    if (axeMax = 'x') {
      for(unsigned j = 0 ; j<pointList.size(); j+=3) {
        if(listPoint[j] > val ) {
          upperPartition[indice]=listPoint[j];
          upperPartition[indice+1]=listPoint[j+1];
          upperPartition[indice+2]=listPoint[j+2];
          indice+=3;

        }
      }
    }
    if (axeMax = 'y') {
      for(unsigned j = 1 ; j<pointList.size(); j+=3) {
        if(listPoint[j] > val ) {
          upperPartition[indice]=listPoint[j-1];
          upperPartition[indice+1]=listPoint[j];
          upperPartition[indice+2]=listPoint[j+1];
          indice+=3;

        }
      }
    }
    if (axeMax = 'z') {
      for(unsigned j = 2 ; j<pointList.size(); j+=3) {
        if(listPoint[j] > val ) {
          upperPartition[indice]=listPoint[j-2];
          upperPartition[indice+1]=listPoint[j-1];
          upperPartition[indice+2]=listPoint[j];
          indice+=3;

        }
      }
    }

    return upperPartition ;
}

vector<float> lowerPartition(vector<float> listPoint, char axeMax, float val) {

    vector<float> lowerPartition ;
    lowerPartition.resize(listPoint.size()) ;
    unsigned int indice =0;

    if (axeMax = 'x') {
      for(unsigned j = 0 ; j<pointList.size(); j+=3) {
        if(listPoint[j] < val ) {
          upperPartition[indice]=listPoint[j];
          upperPartition[indice+1]=listPoint[j+1];
          upperPartition[indice+2]=listPoint[j+2];
          indice+=3;

        }
      }
    }
    if (axeMax = 'y') {
      for(unsigned j = 1 ; j<pointList.size(); j+=3) {
        if(listPoint[j] < val ) {
          upperPartition[indice]=listPoint[j-1];
          upperPartition[indice+1]=listPoint[j];
          upperPartition[indice+2]=listPoint[j+1];
          indice+=3;

        }
      }
    }
    if (axeMax = 'z') {
      for(unsigned j = 2 ; j<pointList.size(); j+=3) {
        if(listPoint[j] < val ) {
          upperPartition[indice]=listPoint[j-2];
          upperPartition[indice+1]=listPoint[j-1];
          upperPartition[indice+2]=listPoint[j];
          indice+=3;

        }
      }
    }

    return lowerPartition ;
}


KdNode buildKdTree(PointList pointList) {
  BoundingBox B = computeBoundingBox(pointList) ;

}
