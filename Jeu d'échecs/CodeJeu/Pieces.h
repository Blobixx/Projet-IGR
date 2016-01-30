#include <vector>
#include <string>
#include "Vec3.h"
#include "Mesh.h"

using namespace std;

class Pawn : public Pieces {

};

class Rook : public Pieces {

};

class Bishop : public Pieces {

};

class Queen : public Pieces {

};

class King : public Pieces {

};

class Knight : public Pieces {

};

class Pieces {

  //position on the board
  Vec3f position; // on suppose que pour toutes les pieces z=0
  Mesh mesh;
  Vec3f albedo;

  vector<Int2> possibleMoves;

};
