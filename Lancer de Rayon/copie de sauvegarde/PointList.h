#ifndef _POINTLIST_H
#define _POINTLIST_H

#include <cmath>
#include <vector>
#include "Vec3.h"
#include "tiny_obj_loader.h"

using namespace std;

class PointList {

public :
vector<float> pointList;

PointList() {} 
~PointList() {}

PointList(vector<float> liste) : pointList(liste) {}

} ;

//algorithme de TriSelection
vector<float> TriSelection(vector<float> liste,unsigned int n) {
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

#endif
