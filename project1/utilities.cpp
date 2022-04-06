#include "globals.h"
#include <cstring>
#include <iostream>
#include <random>

using namespace std;

// Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max) {
  if (max < min) {
    swap(max, min);
  }

  static random_device rd;
  static default_random_engine generator(rd());
  uniform_int_distribution<> distro(min, max);
  return distro(generator);
}

void clearScreen() // will just write a newline in an Xcode output window
{
  static const char *term = getenv("TERM");
  if (term == nullptr || strcmp(term, "dumb") == 0) {
    cout << endl;
  } else {
    static const char *ESC_SEQ = "\x1B["; // ANSI Terminal esc seq:  ESC [
    cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
  }
}
