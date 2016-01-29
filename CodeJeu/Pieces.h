#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

class Rook : public Pieces {

}

class Queen: public Pieces{

}

class Bishop : public Pieces {

}

class Pawn : public Pieces {

}

class Knight : public Pieces {

}

class King : public Pieces{

}

class Pieces {

private:
  std::string nom;
  int couleur;
  int x,y,z; //position 
  Mesh mesh;


}
