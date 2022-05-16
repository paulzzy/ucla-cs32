#include "utility.h"
#include "globals.h"
#include <stdexcept>

// Helper function for `BoardImpl::placeShip()` and `BoardImpl::unplaceShip()`
Point move_dir(Direction dir, Point start, int offset) {
  if (dir == Direction::VERTICAL) {
    return Point{start.r + offset, start.c};
  }

  if (dir == Direction::HORIZONTAL) {
    return Point{start.r, start.c + offset};
  }

  throw std::invalid_argument("Unrecognized `Direction` enum");
}

bool equal(Point a, Point b) { return a.r == b.r && a.c == b.c; }

Point add_points(Point a, Point b) { return Point{a.r + b.r, a.c + b.c}; }

bool valid_ship_id(int ship_id, int num_ships) {
  return ship_id >= 0 && ship_id < num_ships;
}
