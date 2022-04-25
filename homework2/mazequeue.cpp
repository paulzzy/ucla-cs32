#include <iostream>
#include <queue>

class Coord {
public:
  Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
  int r() const { return m_row; }
  int c() const { return m_col; }

private:
  int m_row;
  int m_col;
};

// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
bool pathExists(char maze[][10], int sr, int sc, int er, int ec) {
  const char WALKWAY = '.';
  const char FILLED = '#';

  std::queue<Coord> maze_filler;

  maze_filler.push(Coord(sr, sc));
  maze[sr][sc] = FILLED;

  while (!maze_filler.empty()) {
    const Coord current = maze_filler.front();
    maze_filler.pop();

    if (current.r() == er && current.c() == ec) {
      return true;
    }

    const Coord west = Coord(current.r(), current.c() - 1);
    const Coord north = Coord(current.r() - 1, current.c());
    const Coord east = Coord(current.r(), current.c() + 1);
    const Coord south = Coord(current.r() + 1, current.c());

    if (maze[west.r()][west.c()] == WALKWAY) {
      maze_filler.push(west);
      maze[west.r()][west.c()] = FILLED;
    }

    if (maze[north.r()][north.c()] == WALKWAY) {
      maze_filler.push(north);
      maze[north.r()][north.c()] = FILLED;
    }

    if (maze[east.r()][east.c()] == WALKWAY) {
      maze_filler.push(east);
      maze[east.r()][east.c()] = FILLED;
    }

    if (maze[south.r()][south.c()] == WALKWAY) {
      maze_filler.push(south);
      maze[south.r()][south.c()] = FILLED;
    }
  }

  return false;
}

int main() {
  char maze[10][10] = {{'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
                       {'X', '.', 'X', '.', '.', '.', 'X', '.', '.', 'X'},
                       {'X', '.', 'X', '.', 'X', 'X', 'X', 'X', '.', 'X'},
                       {'X', '.', 'X', '.', '.', 'X', '.', 'X', '.', 'X'},
                       {'X', '.', 'X', 'X', '.', 'X', '.', '.', '.', 'X'},
                       {'X', '.', '.', '.', '.', '.', '.', 'X', 'X', 'X'},
                       {'X', '.', 'X', 'X', 'X', 'X', '.', 'X', '.', 'X'},
                       {'X', '.', '.', '.', '.', 'X', 'X', 'X', '.', 'X'},
                       {'X', '.', 'X', '.', '.', 'X', '.', '.', '.', 'X'},
                       {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'}};

  if (pathExists(maze, 5, 6, 8, 1)) {
    std::cout << "Solvable!" << std::endl;
  } else {
    std::cout << "Out of luck!" << std::endl;
  }
}
