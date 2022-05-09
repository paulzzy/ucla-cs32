#ifndef UTILITY_H
#define UTILITY_H

#include "globals.h"

Point move_dir(Direction dir, Point start, int offset);
bool equal(Point a, Point b);
bool valid_ship_id(int ship_id, int num_ships);

#endif
