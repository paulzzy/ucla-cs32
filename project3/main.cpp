#include "Board.h"
#include "Game.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;

bool addStandardShips(Game &g) {
  return g.addShip(5, 'A', "aircraft carrier") &&
         g.addShip(4, 'B', "battleship") && g.addShip(3, 'D', "destroyer") &&
         g.addShip(3, 'S', "submarine") && g.addShip(2, 'P', "patrol boat");
}

struct TestShip {
  int id;
  int length;
  char symbol;
  std::string name;
};

std::vector<TestShip> add_test_ships(Game &g) {
  const int NUM_SHIPS =
      g.cols() - 1; // Longest ship will be shorter than the number of columns,
                    // to ensure there's water for testing purposes
  std::vector<TestShip> test_ships;

  for (int i = 0; i < NUM_SHIPS; i++) {
    const TestShip temp{i, i + 1, static_cast<char>('A' + i),
                        "valid " + std::to_string(i)};
    test_ships.push_back(temp);
    assert(g.addShip(test_ships[i].length, test_ships[i].symbol,
                     test_ships[i].name));
  }

  return test_ships;
}

// `Game` class tests
void game_tests() {
  Game g{MAXROWS, MAXCOLS};

  // `const` methods
  //
  assert(g.rows() == MAXROWS);
  assert(g.cols() == MAXCOLS);
  assert(g.isValid(Point{0, 0}));
  assert(g.isValid(Point{MAXROWS - 1, MAXCOLS - 1}));

  // `Game::addShip()
  //
  // 10 valid test_ships
  std::vector<TestShip> test_ships = add_test_ships(g);
  assert(g.nShips() == 9);

  // `Game::shipLength()`, `Game::shipSymbol()`, `Game::shipName()`
  //
  for (const TestShip &ship : test_ships) {
    assert(g.shipLength(ship.id) == ship.length);
    assert(g.shipSymbol(ship.id) == ship.symbol);
    assert(g.shipName(ship.id) == ship.name);
  }

  // `Game::play()`
  // TOOO

  std::cerr << "Passed `Game` test cases ✨" << std::endl;
}

// `Board` class tests
void board_tests() {
  Game g{MAXROWS, MAXCOLS};

  std::vector<TestShip> test_ships = add_test_ships(g);

  Board b{g};

  b.block();
  b.clear();
  b.display(false);
  std::cerr << "The above board should be cleared." << std::endl;

  // `Board::placeShip()`
  //
  assert(g.nShips() == 9);
  assert(!b.placeShip(Point{0, 0}, -1, HORIZONTAL)); // Invalid ID
  assert(!b.placeShip(Point{0, MAXCOLS - 1}, test_ships.at(1).id,
                      HORIZONTAL)); // Invalid direction, ship of length 2

  for (const TestShip &ship : test_ships) {
    assert(b.placeShip(Point{ship.id, 0}, ship.id, HORIZONTAL));
    std::cerr << "Placed ship with id " << ship.id << std::endl;
  }
  b.display(false);
  std::cerr << "The above board should display nine horizontal ships stacked "
               "top-down"
            << std::endl;

  assert(!b.placeShip(Point{0, 0}, test_ships.at(0).id,
                      HORIZONTAL)); // Cannot overlap
  assert(!b.placeShip(Point{9, 0}, test_ships.at(0).id,
                      HORIZONTAL)); // Cannot have duplicate ships

  // `Board::unplaceShips()`
  //
  assert(!b.unplaceShip(Point{0, 0}, -1, HORIZONTAL));         // Invalid ID
  assert(!b.unplaceShip(Point{0, 0}, g.nShips(), HORIZONTAL)); // Invalid ID
  for (const TestShip &ship : test_ships) {
    if (ship.id != 0) {
      assert(!b.unplaceShip(
          Point{ship.id, 0}, ship.id,
          VERTICAL)); // Does not contain ship at indicated locations
    }
    assert(!b.unplaceShip(
        Point{ship.id, 1}, ship.id,
        HORIZONTAL)); // Does not contain ship at indicated locations
    assert(b.unplaceShip(Point{ship.id, 0}, ship.id, HORIZONTAL));
    std::cerr << "Unplaced ship with id " << ship.id << std::endl;
  }
  assert(!b.allShipsDestroyed()); // Unplacing ships does not destroy them
  b.display(false);
  std::cerr << "The above board should contain no ships." << std::endl;

  // `Board::attack()`
  //
  b.clear();
  for (const TestShip &ship : test_ships) {
    assert(b.placeShip(Point{ship.id, 0}, ship.id, HORIZONTAL));
  }

  for (const TestShip &ship : test_ships) {
    // Set to values that don't make sense for a failed hit
    bool hit = true;
    bool destroyed = true;
    int ship_id = ship.id;
    assert(!b.attack(Point{MAXROWS, MAXCOLS}, hit, destroyed,
                     ship_id)); // Invalid point
    assert(b.attack(Point{ship.id, MAXCOLS - 1}, hit, destroyed,
                    ship_id)); // Missed hit

    assert(!hit);
    assert(!destroyed);
    assert(ship_id == -1);

    for (int i = 0; i < ship.length - 1; i++) {
      // Set to values that don't make sense for a successful hit
      hit = false;
      destroyed = true;

      assert(b.attack(Point{ship.id, i}, hit, destroyed, ship_id));
      assert(hit);
      assert((ship.id == 0) ? destroyed : !destroyed);
    }

    // Set to values that don't make sense for a successful hit
    hit = false;
    destroyed = true;
    ship_id = -1;

    assert(!b.attack(Point{ship.id, ship.length - 2}, hit, destroyed,
                     ship_id)); // Attack on previouly attacked location

    b.attack(Point{ship.id, ship.length - 1}, hit, destroyed, ship_id);
    assert(hit);
    assert(destroyed);
    assert(ship_id == ship.id);

    std::cerr << "Destroyed ship with id " << ship.id << std::endl;
  }
  assert(b.allShipsDestroyed());

  std::cerr << "Passed `Board` test cases 🤩" << std::endl;
}
}
