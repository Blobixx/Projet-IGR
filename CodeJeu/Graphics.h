#include <cmath>
#include <iostream>
#pragma once
#include <vector>
#include "Vec3.h"
#include "Mesh.h"

// Classe permettant de gerer les graphiques de toute les pieces et du plateau

class Graphics {

  public :

    Mesh meshPawn ;
    Mesh meshBishop ;
    Mesh meshKing ;
    Mesh meshQueen ;
    Mesh meshRook ;
    Mesh meshKnight ;
    Mesh meshBoard ;

  public :

    void drawPawn(Mesh _meshPawn) ;
    void drawBishop(Mesh _meshBishop) ;
    void drawKing(Mesh _meshKing) ;
    void drawQueen(Mesh _meshQueen) ;
    void drawRook(Mesh _meshRook) ;
    void drawKnight(Mesh _meshKnight) ; 

} ;
