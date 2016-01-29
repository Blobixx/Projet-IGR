#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "Mesh.h"
#include "Vec3.h"
#include <GL/glew.h>
#include <GL/glut.h>
 using namespace std;


class Board : public Mesh {

  public :
    Vec3f position1 ;
    Vec3f position2 ;
    Vec3f position3 ;
    Vec3f position4 ;

// Méthode pour tracer le board


  drawBoard(Vec3f point_1, Vec3f point_2, Vec3f point_3, Vec3f point_4) {
    glBegin(GL_TRIANGLES) ;
    glColor(0.5,0.4,0.9) ;
    glVertex3f(point_1.p[0],point_1.p[1],point_1.p[2]) ;
    glVertex3f(point_2.p[0],point_2.p[1],point_2.p[2]) ;
    glVertex3f(point_3.p[0],point_3.p[1],point_3.p[2]) ;

    glVertex3f(point_3.p[0],point_3.p[1],point_3.p[2]) ;
    glVertex3f(point_4.p[0],point_4.p[1],point_4.p[2]) ;
    glVertex3f(point_1.p[0],point_1.p[1],point_1.p[2]) ;


  }

  // On cree le board comme un mesh de deux triangles
  //V.resize(4);
  //T.resize(3);
  //V[0] = (-1,-1,0);
  //V[1] = (1,-1,0);
  //V[2] = (1,1,0);
  //V[3] = (-1,1,0);

  //T[0].v[0] = 0;
  //T[0].v[1] = 1;
  //T[0].v[2] = 2;
  //T[1].v[0] = 0;
  //T[1].v[1] = 2;
  //T[1].v[2] = 3;


} ;
