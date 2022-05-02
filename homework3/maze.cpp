struct Coord {
  int row;
  int col;
};

// Return true if there is a path from (start_row, start_col) to
// (end_row, end_col) through the maze; return false otherwise
// NOLINTNEXTLINE(misc-no-recursion, readability-magic-numbers)
bool pathExists(char maze[][10], int start_row, int start_col, int end_row,
                int end_col) {
  const char WALKWAY = '.';
  const char FILLED = '#';

  if (start_row == end_row && start_col == end_col) {
    return true;
  }

  maze[start_row][start_col] = FILLED;

  const Coord west = {start_row, start_col - 1};
  const Coord north = {start_row - 1, start_col};
  const Coord east = {start_row, start_col + 1};
  const Coord south = {start_row + 1, start_col};

  const Coord locations[] = {west, north, east, south};

  // Linter suggests `std::any_of`, which requires `#include <algorithm>`. Prof.
  // Smallberg might not allow that include directive.
  for (Coord loc : locations) { // NOLINT(readability-use-anyofallof)
    if (maze[loc.row][loc.col] == WALKWAY) {
      maze[loc.row][loc.col] = FILLED;
      if (pathExists(maze, loc.row, loc.col, end_row, end_col)) {
        return true;
      }
    }
  }

  return false;
}
