#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include "utility.h"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

class GameImpl {
public:
  GameImpl(int nRows, int nCols);
  int rows() const;
  int cols() const;
  bool isValid(Point p) const;
  Point randomPoint() const;
  bool addShip(int length, char symbol, string name);
  int nShips() const;
  int shipLength(int shipId) const;
  char shipSymbol(int shipId) const;
  string shipName(int shipId) const;
  static Player *play(Player *p1, Player *p2, Board &b1, Board &b2,
                      bool shouldPause);

private:
  int m_rows;
  int m_cols;

  struct Ship {
    int length;
    char symbol;
    std::string name;
  };

  std::vector<Ship> ships;

  struct AttackData {
    Point point;
    bool valid;
    bool hit_ship;
    bool destroyed_ship;
    int ship_id;
  };
};

void waitForEnter() {
  cout << "Press enter to continue: ";
  cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
GameImpl::GameImpl(int nRows, int nCols) : m_rows{nRows}, m_cols{nCols} {}

int GameImpl::rows() const { return m_rows; }

int GameImpl::cols() const { return m_cols; }

bool GameImpl::isValid(Point p) const {
  return p.r >= 0 && p.r < rows() && p.c >= 0 && p.c < cols();
}

Point GameImpl::randomPoint() const {
  return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name) {
  const Ship temp{length, symbol,
                  name}; // NOLINT(performance-unnecessary-value-param)
  ships.push_back(temp);
  return true;
}

int GameImpl::nShips() const { return static_cast<int>(ships.size()); }

int GameImpl::shipLength(int shipId) const { return ships.at(shipId).length; }

char GameImpl::shipSymbol(int shipId) const { return ships.at(shipId).symbol; }

string GameImpl::shipName(int shipId) const { return ships.at(shipId).name; }

std::string attack_message(bool hit_ship, bool destroyed_ship,
                           const std::string &name) {
  if (destroyed_ship) {
    return "destroyed the " + name;
  }

  if (hit_ship) {
    return "hit something";
  }

  return "missed";
}

void prompt_to_continue() {
  std::cout << "Press enter to continue: ";
  std::cin.ignore(STREAM_MAX, '\n');
}

Player *GameImpl::play(Player *p1, Player *p2, Board &b1, Board &b2,
                       bool shouldPause = true) {
  if (!p1->placeShips(b1) || !p2->placeShips(b2)) {
    return nullptr;
  }

  Player *attacking_player = p1;
  Player *other_player = p2;
  Board *other_board = &b2;

  while (!(b1.allShipsDestroyed() || b2.allShipsDestroyed())) {
    const bool only_shots = attacking_player->isHuman();

    std::cout << attacking_player->name() << "'s turn.  Board for "
              << other_player->name() << ":\n";
    other_board->display(only_shots);

    AttackData attack{Point{-1, -1}, false, false, false, -1};
    attack.point = attacking_player->recommendAttack();

    // If the attacking player is a human, the user is prompted to attack
    attack.valid = other_board->attack(attack.point, attack.hit_ship,
                                       attack.destroyed_ship, attack.ship_id);

    attacking_player->recordAttackResult(attack.point, attack.valid,
                                         attack.hit_ship, attack.destroyed_ship,
                                         attack.ship_id);
    other_player->recordAttackByOpponent(attack.point);

    // Show results of attack
    const std::string ship_name =
        attack.destroyed_ship ? other_player->game().shipName(attack.ship_id)
                              : "";
    std::cout << attacking_player->name() << " attacked (" << attack.point.r
              << "," << attack.point.c << ") and "
              << attack_message(attack.hit_ship, attack.destroyed_ship,
                                ship_name)
              << ", resulting in:\n";

    other_board->display(only_shots);

    if (shouldPause && !b1.allShipsDestroyed() && !b2.allShipsDestroyed()) {
      prompt_to_continue();
    }

    // Switch players
    attacking_player = (attacking_player == p1) ? p2 : p1;
    other_player = (other_player == p1) ? p2 : p1;
    other_board = (other_board == &b1) ? &b2 : &b1;
  }

  Player *winner = b1.allShipsDestroyed() ? p2 : p1;
  Board *winners_board = b1.allShipsDestroyed() ? &b2 : &b1;
  Player *loser = b1.allShipsDestroyed() ? p1 : p2;

  std::cout << winner->name() << " wins!" << std::endl;

  if (loser->isHuman()) {
    std::cout << "Here is where " << winner->name()
              << "'s ships were:" << std::endl;
    winners_board->display(false);
  }

  return winner;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols) {
  if (nRows < 1 || nRows > MAXROWS) {
    cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
    exit(1);
  }
  if (nCols < 1 || nCols > MAXCOLS) {
    cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
    exit(1);
  }
  m_impl = new GameImpl(nRows, nCols);
}

Game::~Game() { delete m_impl; }

int Game::rows() const { return m_impl->rows(); }

int Game::cols() const { return m_impl->cols(); }

bool Game::isValid(Point p) const { return m_impl->isValid(p); }

Point Game::randomPoint() const { return m_impl->randomPoint(); }

bool Game::addShip(int length, char symbol, string name) {
  if (length < 1) {
    cout << "Bad ship length " << length << "; it must be >= 1" << endl;
    return false;
  }
  if (length > rows() && length > cols()) {
    cout << "Bad ship length " << length << "; it won't fit on the board"
         << endl;
    return false;
  }
  if (isascii(symbol) == 0 || isprint(symbol) == 0) {
    cout << "Unprintable character with decimal value " << symbol
         << " must not be used as a ship symbol" << endl;
    return false;
  }
  if (symbol == 'X' || symbol == '.' || symbol == 'o') {
    cout << "Character " << symbol << " must not be used as a ship symbol"
         << endl;
    return false;
  }
  int totalOfLengths = 0;
  for (int s = 0; s < nShips(); s++) {
    totalOfLengths += shipLength(s);
    if (shipSymbol(s) == symbol) {
      cout << "Ship symbol " << symbol
           << " must not be used for more than one ship" << endl;
      return false;
    }
  }
  if (totalOfLengths + length > rows() * cols()) {
    cout << "Board is too small to fit all ships" << endl;
    return false;
  }
  return m_impl->addShip(length, symbol,
                         name); // NOLINT(performance-unnecessary-value-param)
}

int Game::nShips() const { return m_impl->nShips(); }

int Game::shipLength(int shipId) const {
  assert(shipId >= 0 && shipId < nShips());
  return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const {
  assert(shipId >= 0 && shipId < nShips());
  return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const {
  assert(shipId >= 0 && shipId < nShips());
  return m_impl->shipName(shipId);
}

Player *Game::play(Player *p1, Player *p2, bool shouldPause) {
  if (p1 == nullptr || p2 == nullptr || nShips() == 0) {
    return nullptr;
  }
  Board b1(*this);
  Board b2(*this);
  return m_impl->play(p1, p2, b1, b2, shouldPause);
}
