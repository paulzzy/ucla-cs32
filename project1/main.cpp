#include "Game.h"
#include "Gark.h"
#include "History.h"
#include "Mesa.h"
#include "Player.h"
#include "globals.h"

int main() {
  Game test_game = Game(15, 18, 100);
  test_game.play();

  return 0;
}
