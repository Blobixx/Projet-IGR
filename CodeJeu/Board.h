#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "Mesh.h"
#include "Vec3.h"
 using namespace std;


class Board : public Mesh{

  // On cree le board comme un mesh de deux triangles
  V.resize(4);
  T.resize(3);
  V[0] = (-1,-1,0);
  V[1] = (1,-1,0);
  V[2] = (1,1,0);
  V[3] = (-1,1,0);

  T[0].v[0] = 0;
  T[0].v[1] = 1;
  T[0].v[2] = 2;
  T[1].v[0] = 0;
  T[1].v[1] = 2;
  T[1].v[2] = 3;


}
