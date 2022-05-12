#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include "utility.h"
#include <iostream>
#include <string>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player {
public:
  AwfulPlayer(string nm, const Game &g);
  virtual bool placeShips(Board &b);
  virtual Point recommendAttack();
  virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                  bool shipDestroyed, int shipId);
  virtual void recordAttackByOpponent(Point p);

private:
  Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game &g)
    // NOLINTNEXTLINE(performance-unnecessary-value-param)
    : Player(nm, g), m_lastCellAttacked(0, 0) {}

bool AwfulPlayer::placeShips(Board &b) {
  // Clustering ships is bad strategy
  for (int k = 0; k < game().nShips(); k++) {
    if (!b.placeShip(Point(k, 0), k, HORIZONTAL)) {
      return false;
    }
  }
  return true;
}

Point AwfulPlayer::recommendAttack() {
  if (m_lastCellAttacked.c > 0) {
    m_lastCellAttacked.c--;
  } else {
    m_lastCellAttacked.c = game().cols() - 1;
    if (m_lastCellAttacked.r > 0) {
      m_lastCellAttacked.r--;
    } else {
      m_lastCellAttacked.r = game().rows() - 1;
    }
  }
  return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */,
                                     bool /* shipDestroyed */,
                                     int /* shipId */) {
  // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */) {
  // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int &r, int &c) {
  bool result(cin >> r >> c);
  if (!result) {
    cin.clear(); // clear error state so can do more input operations
  }
  cin.ignore(STREAM_MAX, '\n');
  return result;
}

char get_one_char() {
  char value;
  if (!(std::cin >> value)) {
    std::cin.clear();
  }
  std::cin.ignore(STREAM_MAX, '\n');
  return value;
}

class HumanPlayer : public Player {
public:
  HumanPlayer(std::string name, const Game &g);
  virtual bool isHuman() const { return true; }

  virtual bool placeShips(Board &b);
  virtual Point recommendAttack();

  // No-op
  virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                  bool shipDestroyed, int shipId);
  // No-op
  virtual void recordAttackByOpponent(Point p);
};

// NOLINTNEXTLINE(performance-unnecessary-value-param)
HumanPlayer::HumanPlayer(std::string name, const Game &g) : Player(name, g) {}

bool HumanPlayer::placeShips(Board &b) {
  std::cout << name() << " must place " << game().nShips() << " ships."
            << std::endl;

  for (int ship_id = 0; ship_id < game().nShips(); ship_id++) {
    b.display(false);

    // Get direction
    char input_dir = '\0';
    bool success_dir = false;
    do {
      std::cout << "Enter h or v for direction of " << game().shipName(ship_id)
                << " (length " << game().shipLength(ship_id) << "): ";
      input_dir = get_one_char();

      success_dir = input_dir == 'h' || input_dir == 'v';
      if (!success_dir) {
        std::cout << "Direction must be h or v." << std::endl;
      }
    } while (!success_dir);

    Direction dir =
        (input_dir == 'h') ? Direction::HORIZONTAL : Direction::VERTICAL;

    // Get point to place ship at
    Point input_pt{-1, -1};
    bool success_pt = false;
    do {
      std::cout << "Enter row and column of "
                << (dir == Direction::HORIZONTAL ? "leftmost" : "topmost")
                << " cell (e.g., 3 5): ";

      const bool is_integers = getLineWithTwoIntegers(input_pt.r, input_pt.c);

      if (!is_integers) {
        std::cout << "You must enter two integers." << std::endl;
      } else {
        success_pt = b.placeShip(input_pt, ship_id, dir);
        if (!success_pt) {
          std::cout << "The ship can not be placed there." << std::endl;
        }
      }

    } while (!success_pt);
  }

  return true;
}

Point HumanPlayer::recommendAttack() {
  std::cout << "Enter the row and column to attack (e.g., 3 5): ";

  Point input{-1, -1};
  bool is_integers = false;
  do {
    is_integers = getLineWithTwoIntegers(input.r, input.c);

    if (!is_integers) {
      std::cout << "You must enter two integers." << std::endl;
    }
  } while (!is_integers);

  return input;
}

void HumanPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */,
                                     bool /* shipDestroyed */,
                                     int /* shipId */) {}

void HumanPlayer::recordAttackByOpponent(Point /* p */) {}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
typedef AwfulPlayer MediocrePlayer;
// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
typedef AwfulPlayer GoodPlayer;

//*********************************************************************
//  createPlayer
//*********************************************************************

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
Player *createPlayer(string type, // NOLINT(performance-unnecessary-value-param)
                     string nm,   // NOLINT(performance-unnecessary-value-param)
                     const Game &g) {
  static string types[] = {"human", "awful", "mediocre", "good"};

  int pos;
  for (pos = 0; pos != sizeof(types) / sizeof(types[0]) && type != types[pos];
       pos++) {
    ;
  }
  switch (pos) {
  case 0:
    return new HumanPlayer(nm, g);
  case 1:
    return new AwfulPlayer(nm, g);
  case 2:
    return new MediocrePlayer(nm, g);
  case 3:
    return new GoodPlayer(nm, g);
  default:
    return nullptr;
  }
}
