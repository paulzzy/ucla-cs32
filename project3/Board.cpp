#include "Board.h"
#include "Game.h"
#include "globals.h"
#include "utility.h"
#include <iostream>

using namespace std;

class BoardImpl {
public:
  BoardImpl(const Game &g);
  void clear();
  void block();
  void unblock();
  bool placeShip(Point topOrLeft, int shipId, Direction dir);
  bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
  void display(bool shotsOnly) const;
  bool attack(Point p, bool &shotHit, bool &shipDestroyed, int &shipId);
  bool allShipsDestroyed() const;

private:
  const Game &m_game;

  struct PositionData {
    int type_id;
    bool attacked;
    bool blocked;
  };

  struct ShipData {
    std::vector<Point> positions;
    bool placed;
    bool destroyed;
  };

  std::vector<std::vector<PositionData>> board_data;
  std::vector<ShipData>
      ships_data; // Each index should correspond with a ship from `Game::ships`

  char status(PositionData position, bool shots_only) const;
  PositionData &pos(Point point);
};

BoardImpl::BoardImpl(const Game &g)
    : m_game{g}, board_data{static_cast<size_t>(m_game.rows()),
                            std::vector<PositionData>{
                                static_cast<size_t>(m_game.cols()),
                                {-1,       // type_id
                                 false,    // attacked
                                 false}}}, // blocked
      ships_data{static_cast<size_t>(m_game.nShips()),
                 {std::vector<Point>(), // positions
                  false,                // placed
                  false}}               // destroyed
{}

void BoardImpl::clear() {
  for (std::vector<PositionData> &row : board_data) {
    for (PositionData &position : row) {
      position.type_id = -1;
      position.attacked = false;
      position.blocked = false;
    }
  }

  for (ShipData &ship : ships_data) {
    ship.positions.clear();
    ship.placed = false;
    ship.destroyed = false;
  }
}

void BoardImpl::block() {
  // Randomly block 50% of cells
  int block_num = m_game.rows() * m_game.cols() / 2;

  while (block_num > 0) {
    const Point random_point =
        Point{randInt(m_game.rows()), randInt(m_game.cols())};
    PositionData &random_position = pos(random_point);

    if (!random_position.blocked) {
      random_position.blocked = true;
      block_num--;
    }
  }
}

void BoardImpl::unblock() {
  for (std::vector<PositionData> &row : board_data) {
    for (PositionData &position : row) {
      position.blocked = false;
    }
  }
}

BoardImpl::PositionData &BoardImpl::pos(Point point) {
  return board_data.at(point.r).at(point.c);
}

// Validation checks:
// 1. The shipId is invalid
// 2. The ship would be partly or fully outside the board.
// 3. The ship would overlap an already-placed ship.
// 4. The ship would overlap one or more positions that were blocked by a
// previous call to the block function.
// 5. The ship with that ship ID has previously been placed on this Board and
// not yet been unplaced since its most recent placement.
bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir) {
  // Validation: Check 1
  if (!(shipId >= 0 && shipId < m_game.nShips())) {
    return false;
  }

  // Validation: Check 5
  if (ships_data.at(shipId).placed) {
    return false;
  }

  const int length = m_game.shipLength(shipId);

  for (int i = 0; i < length; i++) {
    const Point point = move_dir(dir, topOrLeft, i);

    // Validation: Check 2
    if (!m_game.isValid(point)) {
      return false;
    }

    const PositionData position = pos(point);

    // Validation: Check 3
    if (position.type_id >= 0) {
      return false;
    }

    // Validation: Check 4
    if (position.blocked) {
      return false;
    }
  }

  // Ship placement
  ShipData &ship = ships_data.at(shipId);
  for (int i = 0; i < length; i++) {
    const Point point = move_dir(dir, topOrLeft, i);

    PositionData &position = pos(point);

    position.type_id = shipId;
    ship.positions.push_back(point);
  }

  ship.placed = true;
  assert(ship.positions.size() == length);
  return true;
}

// This function attempts to remove the specified ship from the board, so the
// positions it occupied may be used to place other ships. The parameters are
// the same as for placeShip. This function returns false if the ship cannot be
// removed because:
// 1. The shipId is invalid
// 2. The board does not contain the entire ship at the indicated locations.
// If this function returns false, then the board must remain unchanged (i.e. no
// part of the ship may be removed upon failure). If this function successfully
// removes the ship, the board is updated to reflect that, and the function
// returns true.
bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir) {
  // Invalid `shipId`
  if (!valid_ship_id(shipId, m_game.nShips())) {
    return false;
  }

  ShipData &ship_data = ships_data.at(shipId);

  // Validation
  for (int i = 0; i < ship_data.positions.size(); i++) {
    const Point current = move_dir(dir, topOrLeft, i);
    const Point ship_point = ship_data.positions.at(i);

    // Invalid locations
    if (!equal(current, ship_point)) {
      return false;
    }
  }

  // Replace ship with water on the board
  for (Point point : ship_data.positions) {
    PositionData &position = pos(point);
    position.type_id = -1;
    position.attacked = false;
  }

  // Reset `positions`
  ship_data.positions.clear();
  ship_data.placed = false;

  return true;
}

char BoardImpl::status(PositionData position, bool shots_only) const {
  const char WATER = '.';
  const char DAMAGED = 'X';
  const char MISSED = 'o';

  if (position.type_id == -1) {
    return position.attacked ? MISSED : WATER;
  }

  if (shots_only) {
    return position.attacked ? DAMAGED : WATER;
  }

  const char symbol = m_game.shipSymbol(position.type_id);
  return position.attacked ? DAMAGED : symbol;
}

void BoardImpl::display(bool shotsOnly) const {
  // 1. First line: The function must print two spaces followed by the digits
  // for each column, starting at 0, followed by a newline. You may assume there
  // will be no more than 10 columns.
  std::cout << "  ";
  for (int i = 0; i < m_game.cols(); i++) {
    std::cout << i;
  }
  std::cout << "\n";

  // 2. Remaining lines: The function must print a digit specifying the row
  // number, starting at 0, followed by a space, followed by the contents of the
  // current row, followed by a newline. You may assume there will be no more
  // than 10 rows:
  //
  //   a. If the shotOnly parameter is false, use the ship's symbol to display
  //   an undamaged ship segment; if the shotsOnly parameter is true, show a
  //   period to display an undamaged ship segment.
  //
  //   b. Use an X character to display any damaged ship segment.
  //
  //   c. Use a period to display water where no attack has been made.
  //
  //   d. Use a lower case letter o character to display water where an attack
  //   has been made that missed a ship.

  for (int i = 0; i < m_game.rows(); i++) {
    std::cout << i << " ";

    const std::vector<PositionData> row = board_data.at(i);

    for (PositionData position : row) {
      std::cout << status(position, shotsOnly);
    }

    std::cout << "\n";
  }

  std::cout << std::flush;
}

// Validation checks:
// The function must return false if the attack is invalid (the attack point is
// outside of the board area, or an attack is made on a previously attacked
// location).
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
bool BoardImpl::attack(Point p, bool &shotHit, bool &shipDestroyed,
                       int &shipId) {
  // Validation: `shotHit` and `shipDestroyed` are the same reference
  // These are meant to be different variables.
  if (&shotHit == &shipDestroyed) {
    return false;
  }

  // Validation: Attack point outside of board
  if (!m_game.isValid(p)) {
    return false;
  }

  PositionData &position = pos(p);

  // Validation: Attack on previously attacked location
  if (position.attacked) {
    return false;
  }

  position.attacked = true;
  if (position.type_id == -1) {
    // Hit water
    shotHit = false;
    shipDestroyed = false;
    shipId = -1;
  } else {
    // Hit a ship
    shotHit = true;

    shipDestroyed = true; // Start by assuming the ship is destroyed
    ShipData &ship_data = ships_data.at(position.type_id);

    ship_data.destroyed = true;
    for (Point ship_point : ship_data.positions) {
      const PositionData ship_pos = pos(ship_point);

      if (!ship_pos.attacked) {
        shipDestroyed = false; // Assumption is incorrect
        ship_data.destroyed = false;
      }
    }

    if (shipDestroyed) {
      shipId = position.type_id;
    }
  }

  return true;
}

bool BoardImpl::allShipsDestroyed() const {
  // NOLINTNEXTLINE(readability-use-anyofallof)
  for (const ShipData &ship_data : ships_data) {
    if (!ship_data.destroyed) {
      return false;
    }
  }

  return true;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game &g) { m_impl = new BoardImpl(g); }

Board::~Board() { delete m_impl; }

void Board::clear() { m_impl->clear(); }

void Board::block() { return m_impl->block(); }

void Board::unblock() { return m_impl->unblock(); }

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir) {
  return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir) {
  return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const { m_impl->display(shotsOnly); }

bool Board::attack(Point p, bool &shotHit, bool &shipDestroyed, int &shipId) {
  return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const { return m_impl->allShipsDestroyed(); }
