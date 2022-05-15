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
  //
  Game g_std{MAXROWS, MAXCOLS};
  addStandardShips(g_std);

  const bool pause = false;

#define TEST_PLAY_AWFUL
#ifdef TEST_PLAY_AWFUL

  Player *awful_1 = createPlayer("awful", "test_awful_1", g_std);
  Player *awful_2 = createPlayer("awful", "test_awful_2", g_std);

  g_std.play(awful_1, awful_2, pause);

  std::cerr
      << "Check the above game between two awful players for correctness.\n"
      << std::endl;

#endif

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

void human_player_tests() {
  Game g{MAXROWS, MAXCOLS};
  add_test_ships(g);
  Player *h = createPlayer("human", "human_test", g);

  Board b{g};

  assert(h->isHuman());
  assert(h->placeShips(b));

  std::cerr << "Enter 0 0 when prompted for the attack point." << std::endl;
  const Point p1 = h->recommendAttack();
  assert(p1.r == 0 && p1.c == 0);

  std::cerr << std::endl;
  std::cerr << "Enter " << MAXROWS << " " << MAXCOLS
            << " when prompted for the attack point.\n";
  const Point p2 = h->recommendAttack();
  assert(p2.r == MAXROWS && p2.c == MAXCOLS);
}

void mediocre_player_tests() {
  Game g{MAXROWS, MAXCOLS};
  addStandardShips(g);

  Player *m = createPlayer("mediocre", "test_mediocre", g);

  Board b{g};

  assert(!m->isHuman());

// #define MEDIOCRE_TEST_PLACING_SHIPS
#ifdef MEDIOCRE_TEST_PLACING_SHIPS

  const int MAX_PLACES = 100;
  int fail_count = 0;

  for (int i = 0; i < MAX_PLACES; i++) {
    if (m->placeShips(b)) {
      b.display(false);
      std::cerr << "Above is Board " << i
                << ": Should contain the 5 standard Battleship vessels, "
                   "arranged randomly.\n\n";
      b.clear();
    } else {
      fail_count++;
      std::cerr << "Above is Board " << i
                << ": Could not place all ships.\n\n ";
    }
  }

  std::cerr << "Could not place " << fail_count << " ships out of "
            << MAX_PLACES << " total.\n";

#endif

  Game g_std{MAXROWS, MAXCOLS};
  addStandardShips(g_std);

  const bool pause = true;

#define TEST_PLAY_MEDIOCRE
#ifdef TEST_PLAY_MEDIOCRE

  Player *mediocre_1 = createPlayer("mediocre", "test_mediocre_1", g_std);
  Player *mediocre_2 = createPlayer("mediocre", "test_mediocre_2", g_std);

  g_std.play(mediocre_1, mediocre_2, pause);

  std::cerr
      << "Check the above game between two mediocre players for correctness.\n"
      << std::endl;

#endif

  std::cerr << "Passed mediocre player test cases." << std::endl;
}

int main() {
// #define PLAY_DEFAULT_GAMES
#ifdef PLAY_DEFAULT_GAMES

  const int NTRIALS = 10;

  cout << "Select one of these choices for an example of the game:" << endl;
  cout << "  1.  A mini-game between two mediocre players" << endl;
  cout << "  2.  A mediocre player against a human player" << endl;
  cout << "  3.  A " << NTRIALS
       << "-game match between a mediocre and an awful player, with no pauses"
       << endl;
  cout << "Enter your choice: ";
  string line;
  getline(cin, line);
  if (line.empty()) {
    cout << "You did not enter a choice" << endl;
  } else if (line[0] == '1') {
    Game g(2, 3);
    g.addShip(2, 'R', "rowboat");
    Player *p1 = createPlayer("mediocre", "Popeye", g);
    Player *p2 = createPlayer("mediocre", "Bluto", g);
    cout << "This mini-game has one ship, a 2-segment rowboat." << endl;
    g.play(p1, p2);
    delete p1;
    delete p2;
  } else if (line[0] == '2') {
    Game g(10, 10);
    addStandardShips(g);
    Player *p1 = createPlayer("mediocre", "Mediocre Midori", g);
    Player *p2 = createPlayer("human", "Shuman the Human", g);
    g.play(p1, p2);
    delete p1;
    delete p2;
  } else if (line[0] == '3') {
    int nMediocreWins = 0;

    for (int k = 1; k <= NTRIALS; k++) {
      cout << "============================= Game " << k
           << " =============================" << endl;
      Game g(10, 10);
      addStandardShips(g);
      Player *p1 = createPlayer("awful", "Awful Audrey", g);
      Player *p2 = createPlayer("mediocre", "Mediocre Mimi", g);
      Player *winner =
          (k % 2 == 1 ? g.play(p1, p2, false) : g.play(p2, p1, false));
      if (winner == p2) {
        nMediocreWins++;
      }
      delete p1;
      delete p2;
    }
    cout << "The mediocre player won " << nMediocreWins << " out of " << NTRIALS
         << " games." << endl;
    // We'd expect a mediocre player to win most of the games against
    // an awful player.  Similarly, a good player should outperform
    // a mediocre player.
  } else {
    cout << "That's not one of the choices." << endl;
  }

#endif

  // Custom tests
  // game_tests();
  // board_tests();
  // human_player_tests();
  mediocre_player_tests();
}
