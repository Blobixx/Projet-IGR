#include "Vec3.h"


class Intersection {

public:

    Vec3f ptIntersection;
    Vec3f normal;
    float material;
    Vec3f color;
    Vec3f texture;

    Intersection (Vec3f ptIntersection, Vec3f normal){
      this->ptIntersection = ptIntersection;
      this->normal = normal;
    }

};
