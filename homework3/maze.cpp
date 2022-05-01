class Coord {
public:
  Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
  int row() const { return m_row; }
  int col() const { return m_col; }

private:
  int m_row;
  int m_col;
};

// Return true if there is a path from (start_row, start_col) to
// (end_row, end_col) through the maze; return false otherwise
bool pathExists(char maze[][10], int start_row, int start_col, int end_row,
                int end_col) {
  const char WALKWAY = '.';
  const char FILLED = '#';

  if (start_row == end_row && start_col == end_col) {
    return true;
  }

  maze[start_row][start_col] = FILLED;

  const Coord west = Coord(start_row, start_col - 1);
  const Coord north = Coord(start_row - 1, start_col);
  const Coord east = Coord(start_row, start_col + 1);
  const Coord south = Coord(start_row + 1, start_col);

  const Coord locations[] = {west, north, east, south};

  for (Coord loc : locations) {
    if (maze[loc.row()][loc.col()] == WALKWAY) {
      maze[loc.row()][loc.col()] = FILLED;
      if (pathExists(maze, loc.row(), loc.col(), end_row, end_col)) {
        return true;
      }
    }
  }

  return false;
}
