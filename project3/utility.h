#ifndef UTILITY_H
#define UTILITY_H

#include "globals.h"
#include <iostream>
#include <limits>

Point move_dir(Direction dir, Point start, int offset);
bool equal(Point a, Point b);
Point add_points(Point a, Point b);
bool valid_ship_id(int ship_id, int num_ships);

const long STREAM_MAX = std::numeric_limits<std::streamsize>::max();

#endif
