#ifndef _INTERSECTION_H
#define _INTERSECTION_H

#include "Vec3.h"


class Intersection {

public:
    Vec3f ptIntersection;
    Vec3f normal;
    Vec3f diffuse ;
    float shininess;
    Vec3f specular ;
    Vec3f ambient;
    bool dansOmbre;
    bool intersect;

    Intersection (Vec3f ptIntersection, Vec3f normal, Vec3f diffuse, float shininess,Vec3f specular, Vec3f ambient, bool dansOmbre = false, bool intersect = false)
      : ptIntersection(ptIntersection), normal(normal), diffuse(diffuse), shininess(shininess), specular(specular), ambient(ambient), dansOmbre(dansOmbre), intersect(intersect){}

};

#endif
