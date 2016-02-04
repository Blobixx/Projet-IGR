#include "Vec3.h"


class Intersection {

public:

    Vec3f ptIntersection;
    Vec3f normal;
    bool intersect;

    Intersection (Vec3f ptIntersection, Vec3f normal, bool intersect = false)
      : ptIntersection(ptIntersection),
        normal(normal),
        intersect(intersect)
	{
	}

};
