#ifndef MESA_H
#define MESA_H

#include "History.h"
#include "globals.h"

class Gark;
class Player;
class History;

class Mesa {
public:
  // Constructor/destructor
  Mesa(int nRows, int nCols);
  ~Mesa();

  // Accessors
  int rows() const;
  int cols() const;
  Player *player() const;
  int garkCount() const;
  int numGarksAt(int r, int c) const;
  bool determineNewPosition(int &r, int &c, int dir) const;
  void display() const;

  // Mutators
  bool addGark(int r, int c);
  bool addPlayer(int r, int c);
  bool attackGarkAt(int r, int c, int dir);
  bool moveGarks();

  History &history();

private:
  int m_rows;
  int m_cols;
  Player *m_player;
  Gark *m_garks[MAXGARKS];
  int m_nGarks;
  History m_history;
};

#endif // MESA_H
