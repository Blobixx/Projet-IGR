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
    bool intersect;

    Intersection (Vec3f ptIntersection, Vec3f normal, Vec3f diffuse, float shininess,Vec3f specular, bool intersect = false)
      : ptIntersection(ptIntersection), normal(normal), diffuse(diffuse), shininess(shininess), specular(specular), intersect(intersect){}

};

#endif
