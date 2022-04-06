#include "History.h"
#include "Mesa.h"
#include "globals.h"
#include <iostream>

History::History(int nRows, int nCols) : m_rows(nRows), m_cols(nCols) {
  for (int row = 0; row < m_rows; row++) {
    for (int col = 0; col < m_cols; col++) {
      m_grid[row][col] = '.';
    }
  }
}

bool History::record(int r, int c) {
  if (r < 1 || r > m_rows || c < 1 || c > m_cols) {
    return false;
  }

  const char status = m_grid[r - 1][c - 1];
  switch (status) {
  case '.':
    m_grid[r - 1][c - 1] = 'A';
    break;

  case 'Z':
    break;

  default:
    m_grid[r - 1][c - 1]++;
    break;
  }

  return true;
}

void History::display() const {
  clearScreen();
  for (int row = 0; row < m_rows; row++) {
    for (int col = 0; col < m_cols; col++) {
      std::cout << m_grid[row][col];
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}
