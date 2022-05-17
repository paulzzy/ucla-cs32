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
  std::string input_line;
  std::getline(std::cin, input_line);

  if (input_line.empty()) {
    return '\0';
  }

  return input_line.at(0);
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
  Point input{-1, -1};
  bool is_integers = false;
  do {
    std::cout << "Enter the row and column to attack (e.g., 3 5): ";

    is_integers = getLineWithTwoIntegers(input.r, input.c);

    if (!is_integers) {
      std::cout << "You must enter two integers." << std::endl;
    } else if (!game().isValid(input)) {
      std::cout << name() << " wasted a shot at (" << input.r << "," << input.c
                << ")." << std::endl;
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

class MediocrePlayer : public Player {
public:
  MediocrePlayer(std::string name, const Game &g);
  virtual bool placeShips(Board &b);
  virtual Point recommendAttack();
  virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                  bool shipDestroyed, int shipId);
  virtual void recordAttackByOpponent(Point p);

private:
  bool recursively_place(Board &b, int ship_id);
  Point random_attack();
  Point cross_pattern_attack();
  bool was_attacked(Point candidate);
  void record_attack(Point point);

  bool attack_hit{false};
  bool attack_destroyed{false};
  bool used_cross_attack{false};
  Point randomly_hit_point{-1, -1};
  std::vector<std::vector<bool>> attacks;
};

MediocrePlayer::MediocrePlayer(std::string name, const Game &g)
    // NOLINTNEXTLINE(performance-unnecessary-value-param)
    : Player{name, g}, attacks{static_cast<size_t>(game().rows()),
                               std::vector<bool>(game().cols(), false)} {}

// NOLINTNEXTLINE(misc-no-recursion)
bool MediocrePlayer::recursively_place(Board &b, int ship_id) {
  // All ships placed
  if (ship_id >= game().nShips()) {
    return true;
  }

  for (int i = 0; i < game().rows(); i++) {
    for (int j = 0; j < game().cols(); j++) {
      const Point current{i, j};
      if (b.placeShip(current, ship_id, Direction::HORIZONTAL)) {
        bool placed_all = recursively_place(b, ship_id + 1);

        if (placed_all) {
          return true;
        }

        // Backtrack if impossible to place current ship
        b.unplaceShip(current, ship_id, Direction::HORIZONTAL);
      }
      if (b.placeShip(current, ship_id, Direction::VERTICAL)) {
        bool placed_all = recursively_place(b, ship_id + 1);

        if (placed_all) {
          return true;
        }

        // Backtrack if impossible to place current ship
        b.unplaceShip(current, ship_id, Direction::VERTICAL);
      }
    }
  }

  // Impossible to place all ships
  return false;
}

// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.
bool MediocrePlayer::placeShips(Board &b) {
  const int MAX_RETRIES = 50;

  for (int i = 0; i < MAX_RETRIES; i++) {
    b.block();

    if (recursively_place(b, 0)) {
      b.unblock();
      return true;
    }

    b.clear();
  }

  return false;
}

void MediocrePlayer::record_attack(Point point) {
  attacks.at(point.r).at(point.c) = true;
}

Point MediocrePlayer::random_attack() {
  Point rand_point = game().randomPoint();

  while (was_attacked(rand_point)) {
    rand_point = game().randomPoint();
  }

  record_attack(rand_point);

  used_cross_attack = false;
  return rand_point;
}

bool MediocrePlayer::was_attacked(Point candidate) {
  return attacks.at(candidate.r).at(candidate.c);
}

Point MediocrePlayer::cross_pattern_attack() {
  std::vector<Point> attack_candidates;

  std::vector<Point> cardinal_offsets{
      Point{-1, 0}, // North
      Point{0, 1},  // East
      Point{1, 0},  // South
      Point{0, -1}  // West
  };

  const int CROSS_ARM_LENGTH = 4;
  for (int i = 1; i <= CROSS_ARM_LENGTH; i++) {
    for (Point unit_offset : cardinal_offsets) {
      Point offset = Point{unit_offset.r * i, unit_offset.c * i};
      Point candidate = add_points(randomly_hit_point, offset);

      if (game().isValid(candidate) && !was_attacked(candidate)) {
        attack_candidates.push_back(candidate);
      }
    }
  }

  // If the game has ships of length 6 or more, it's possible that every
  // position no more than 4 steps from (r,c) has been previously chosen. In
  // that case, switch to state 1 and use its selection algorithm.
  if (attack_candidates.empty()) {
    return random_attack();
  }

  // Return a random, valid point to attack on the cross pattern
  const Point recommended = attack_candidates.at(
      static_cast<size_t>(randInt(static_cast<int>(attack_candidates.size()))));

  record_attack(recommended);

  used_cross_attack = true;
  return recommended;
}

Point MediocrePlayer::recommendAttack() {
  if ((!used_cross_attack && !attack_hit) || attack_destroyed) {
    return random_attack();
  }

  return cross_pattern_attack();
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                        bool shipDestroyed, int /* shipId */) {
  if (!validShot) {
    throw std::invalid_argument("Invalid shot at (" + std::to_string(p.r) +
                                "," + std::to_string(p.c) +
                                "), which `MediocrePlayer` should not allow.");
  }

  attack_hit = shotHit;
  attack_destroyed = shipDestroyed;

  if (!used_cross_attack) {
    randomly_hit_point = p;
  }
}

void MediocrePlayer::recordAttackByOpponent(Point /* p */) {}

//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class GoodPlayer : public Player {
public:
  GoodPlayer(std::string name, const Game &g);
  virtual bool placeShips(Board &b);
  virtual Point recommendAttack();
  virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                  bool shipDestroyed, int shipId);
  virtual void recordAttackByOpponent(Point p);

private:
  bool recursively_place(Board &b, int ship_id);
  Point random_checkboard_attack();
  Point find_ship_direction();
  bool was_attacked(Point candidate);
  void record_attack(Point point);
  Point destroy_ship();
  bool in_checkerboard(Point candidate);

  enum AttackState { RANDOM, FIND_SHIP_DIRECTION, DESTROY_SHIP };
  AttackState prev_attack{RANDOM}; // Always begin attacking randomly

  bool attack_hit{false};
  bool attack_destroyed{false};

  Point initial_hit{-1, -1};
  Point last_hit{-1, -1};
  Point repeated_offset{-1, -1};
  std::vector<std::vector<bool>> attacks;
};

GoodPlayer::GoodPlayer(std::string name, const Game &g)
    // NOLINTNEXTLINE(performance-unnecessary-value-param)
    : Player(name, g), attacks{static_cast<size_t>(game().rows()),
                               std::vector<bool>(game().cols(), false)} {}

// NOLINTNEXTLINE(misc-no-recursion)
bool GoodPlayer::recursively_place(Board &b, int ship_id) {
  // All ships placed
  if (ship_id >= game().nShips()) {
    return true;
  }

  for (int i = 0; i < game().rows(); i++) {
    for (int j = 0; j < game().cols(); j++) {
      const Point current{i, j};
      if (b.placeShip(current, ship_id, Direction::HORIZONTAL)) {
        bool placed_all = recursively_place(b, ship_id + 1);

        if (placed_all) {
          return true;
        }

        // Backtrack if impossible to place current ship
        b.unplaceShip(current, ship_id, Direction::HORIZONTAL);
      }
      if (b.placeShip(current, ship_id, Direction::VERTICAL)) {
        bool placed_all = recursively_place(b, ship_id + 1);

        if (placed_all) {
          return true;
        }

        // Backtrack if impossible to place current ship
        b.unplaceShip(current, ship_id, Direction::VERTICAL);
      }
    }
  }

  // Impossible to place all ships
  return false;
}

bool GoodPlayer::placeShips(Board &b) {
  const int MAX_RETRIES = 50;

  for (int i = 0; i < MAX_RETRIES; i++) {
    b.block();

    if (recursively_place(b, 0)) {
      b.unblock();
      return true;
    }

    b.clear();
  }

  return false;
}

void GoodPlayer::record_attack(Point point) {
  attacks.at(point.r).at(point.c) = true;
}

bool GoodPlayer::was_attacked(Point candidate) {
  if (game().isValid(candidate)) {
    return attacks.at(candidate.r).at(candidate.c);
  }

  return false;
}

bool GoodPlayer::in_checkerboard(Point candidate) {
  if (game().isValid(candidate)) {
    return (candidate.r + candidate.c) % 2 == 0;
  }

  return false;
}

Point GoodPlayer::random_checkboard_attack() {
  Point recommended = game().randomPoint();
  const int MAX_RETRIES = 50;
  int retries = 0;

  while (!in_checkerboard(recommended) || was_attacked(recommended)) {
    if (retries > MAX_RETRIES && !was_attacked(recommended)) {
      break;
    }

    recommended = game().randomPoint();
    retries++;
  }

  prev_attack = RANDOM;
  std::cerr << "Below attack uses random_checkboard_attack() 🥵" << std::endl;
  std::cerr << "\tRecommended point is (" << recommended.r << ","
            << recommended.c << ")" << std::endl;
  return recommended;
}

// Checks the four adjacent points to the hit ship segment, starting from north
// and rotating clockwise. If a second ship segment is hit, `repeated_offset`
// will be set to the direction of the two known ship segments. This allows
// `destroy_ship()` to be called.
Point GoodPlayer::find_ship_direction() {
  Point recommended{-1, -1};

  std::vector<Point> cardinal_offsets{
      Point{-1, 0}, // North
      Point{0, 1},  // East
      Point{1, 0},  // South
      Point{0, -1}  // West
  };

  for (Point offset : cardinal_offsets) {
    Point candidate = add_points(initial_hit, offset);
    std::cerr << "Ship direction candidate is (" << candidate.r << ","
              << candidate.c << "): " << std::boolalpha
              << game().isValid(candidate) << ", " << !was_attacked(candidate)
              << std::endl;

    if (game().isValid(candidate) && !was_attacked(candidate)) {
      // Found a valid point adjacent to the hit ship segment
      recommended = candidate;
      repeated_offset = offset;
      break;
    }
  }

  // None of the four adjacent points are valid
  if (equal(recommended, Point{-1, -1})) {
    std::cerr
        << "find_ship_direction() -> random_checkboard_attack(), offset of ("
        << repeated_offset.r << "," << repeated_offset.c << ")" << std::endl;
    return random_checkboard_attack();
  }

  prev_attack = FIND_SHIP_DIRECTION;
  std::cerr << "Below attack uses find_ship_direction() 😭" << std::endl;
  std::cerr << "\tUses the offset (" << repeated_offset.r << ", "
            << repeated_offset.c << ")" << std::endl;
  return recommended;
}

Point GoodPlayer::destroy_ship() {
  Point recommended{-1, -1};

  recommended = add_points(last_hit, repeated_offset);

  // Attack in the other direction if:
  // 1. One end of the ship has been reached (i.e. an attack fails)
  // 2. The point is outside the board
  // 3. The point has already been attacked
  if (!attack_hit || !game().isValid(recommended) ||
      was_attacked(recommended)) {
    repeated_offset = Point{repeated_offset.r * -1, repeated_offset.c * -1};
    recommended = add_points(initial_hit, repeated_offset);

    // In the rare case that a column of adjacently stacked ships is attacked,
    // `destroy_ship()` will switch to a different strategy (rather than being
    // terminated when a ship is destroyed, since that would never occur)
    if (!game().isValid(recommended) || was_attacked(recommended)) {
      std::cerr << "destroy_ship() -> random_attack(), offset of ("
                << repeated_offset.r << "," << repeated_offset.c << ")"
                << std::endl;
      return random_checkboard_attack();
    }
  }

  prev_attack = DESTROY_SHIP;
  std::cerr << "Below attack uses destroy_ship() 🤡\n";
  std::cerr << "\tUses the repeated offset (" << repeated_offset.r << ", "
            << repeated_offset.c << ")" << std::endl;
  return recommended;
}

Point GoodPlayer::recommendAttack() {
  std::cerr << "Initial hit is (" << initial_hit.r << "," << initial_hit.c
            << "), last hit is (" << last_hit.r << "," << last_hit.c << ")"
            << std::endl;
  Point recommended{-1, -1};

  switch (prev_attack) {
  case RANDOM:
    if (attack_hit && !attack_destroyed) {
      recommended = find_ship_direction();
    } else {
      recommended = random_checkboard_attack();
    }
    break;

  case FIND_SHIP_DIRECTION:
    if (attack_destroyed) {
      recommended = random_checkboard_attack();
    } else if (attack_hit) {
      recommended = destroy_ship();
    } else {
      recommended = find_ship_direction();
    }
    break;

  case DESTROY_SHIP:
    if (attack_destroyed) {
      recommended = random_checkboard_attack();
    } else {
      recommended = destroy_ship();
    }
    break;

  default:
    throw std::invalid_argument("Invalid state for previous attack.");
    break;
  }

  record_attack(recommended);
  return recommended;
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int /* shipId */) {
  if (!validShot) {
    throw std::invalid_argument("Invalid shot at (" + std::to_string(p.r) +
                                "," + std::to_string(p.c) +
                                "), which `GoodPlayer` should not allow.");
  }

  attack_hit = shotHit;
  attack_destroyed = shipDestroyed;

  if (attack_hit) {
    last_hit = p;

    if (!attack_destroyed && prev_attack == RANDOM) {
      initial_hit = p;
    }
  }
}

void GoodPlayer::recordAttackByOpponent(Point /* p */) {}

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
