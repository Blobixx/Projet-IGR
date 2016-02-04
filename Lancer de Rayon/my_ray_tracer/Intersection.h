#include "Vec3.h"


class Intersection {

public:
    Vec3f ptIntersection;
    Vec3f normal;
    Vec3f diffuse ;
    float shininess;
    bool intersect;

    Intersection (Vec3f ptIntersection, Vec3f normal, Vec3f diffuse, float shininess, bool intersect = false)
      : ptIntersection(ptIntersection), normal(normal), diffuse(diffuse), shininess(shininess), intersect(intersect){}

};
