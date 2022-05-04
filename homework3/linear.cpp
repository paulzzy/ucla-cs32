#define DO_NOT_INCLUDE_IN_SUBMISSION

#ifdef DO_NOT_INCLUDE_IN_SUBMISSION

#include <cassert>
#include <iostream>
#include <string>

bool somePredicate(const std::string &candidate) { return candidate.empty(); }

#endif

// Return false if the somePredicate function returns true for at
// least one of the array elements; return true otherwise.
// NOLINTNEXTLINE(misc-no-recursion)
bool allFalse(const std::string a[], int n) {
  if (n <= 0) {
    return true;
  }

  if (somePredicate(a[n - 1])) {
    return false;
  }

  return allFalse(a, n - 1);
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
// NOLINTNEXTLINE(misc-no-recursion)
int countFalse(const std::string a[], int n) {
  if (n <= 0) {
    return 0;
  }

  if (!somePredicate(a[n - 1])) {
    return 1 + countFalse(a, n - 1);
  }

  return countFalse(a, n - 1);
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
// NOLINTNEXTLINE(misc-no-recursion)
int firstFalse(const std::string a[], int n) {
  // Negative size
  if (n <= 0) {
    return -1;
  }

  // First element that satisfies the condition
  if (!somePredicate(a[0])) {
    return 0;
  }

  // No elements satisfy the condition
  if (firstFalse(a + 1, n - 1) == -1) {
    return -1;
  }

  // Increment the index when checking the next element
  return 1 + firstFalse(a + 1, n - 1);
}

// Return the subscript of the least string in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k] for all
// k from 0 to n-1).  If the function is told that no strings are to
// be considered in the array, return -1.
// NOLINTNEXTLINE(misc-no-recursion)
int positionOfLeast(const std::string a[], int n) {
  // Negative sizes
  if (n <= 0) {
    return -1;
  }

  // Only one element in array
  if (n == 1) {
    return 0;
  }

  const int min_index = positionOfLeast(a, n - 1);
  const std::string candidate = a[n - 1];
  const std::string min = a[min_index];

  // Since `min_index` starts off as 0, avoid replacing `min` with `candidate`
  // if they are equal, since `min_index` should be as small as possible
  if (candidate < min) {
    return n - 1;
  }

  return min_index;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    "merida" "tiana" "raya" "belle" "tiana" "raya" "moana"
// then the function should return true if a2 is
//    "tiana" "belle" "moana"
// or
//    "tiana" "raya" "raya"
// and it should return false if a2 is
//    "tiana" "moana" "belle"
// or
//    "merida" "belle" "belle"
// NOLINTNEXTLINE(misc-no-recursion)
bool has(const std::string a1[], int n1, const std::string a2[], int n2) {
  // Negative sizes
  if (n1 < 0) {
    n1 = 0;
  }

  if (n2 < 0) {
    n2 = 0;
  }

  // Invalid subsequence
  if (n1 == 0 && n2 > 0) {
    return false;
  }

  // Valid subsequence
  if (n1 >= 0 && n2 == 0) {
    return true;
  }

  const std::string a1_current = a1[n1 - 1];
  const std::string a2_current = a2[n2 - 1];

  if (a1_current == a2_current) {
    return has(a1, n1 - 1, a2, n2 - 1);
  }

  return has(a1, n1 - 1, a2, n2);
}

#ifdef DO_NOT_INCLUDE_IN_SUBMISSION

int main() {
  // Test allFalse
  {
    std::string allFalse_True[5]{"o", "o", "o", "o", "o"};
    assert(allFalse(allFalse_True, 5));
    std::string allFalse_False[5][5]{
        {
            "",
            "o",
            "o",
            "o",
            "o",
        },
        {
            "o",
            "",
            "o",
            "o",
            "o",
        },
        {
            "o",
            "o",
            "",
            "o",
            "o",
        },
        {
            "o",
            "o",
            "o",
            "",
            "o",
        },
        {
            "o",
            "o",
            "o",
            "o",
            "",
        },
    };
    for (int i = 0; i < 5; i++) {
      assert(!allFalse(allFalse_False[i], 5));
    }
  }
  // Test countFalse
  {
    std::string countFalseTest[12][5]{
        {"", "", "", "", ""},      {"", "", "", "", ""},
        {"o", "", "", "", ""},     {"", "", "", "", "o"},
        {"o", "o", "", "", ""},    {"", "", "", "o", "o"},
        {"o", "o", "o", "", ""},   {"", "", "o", "o", "o"},
        {"o", "o", "o", "o", ""},  {"", "o", "o", "o", "o"},
        {"o", "o", "o", "o", "o"}, {"o", "o", "o", "o", "o"}};
    for (int i = 0; i < 6; i++) {
      assert(countFalse(countFalseTest[2 * i], 5) == i);
      assert(countFalse(countFalseTest[2 * i + 1], 5) == i);
    }
  }
  // Test firstFalse
  {
    std::string firstFalseLeft[5][5]{{"o", "", "", "", ""},
                                     {"o", "o", "", "", ""},
                                     {"o", "o", "o", "", ""},
                                     {"o", "o", "o", "o", ""},
                                     {"o", "o", "o", "o", "o"}};
    std::string firstFalseRight[5][5]{{"o", "o", "o", "o", "o"},
                                      {"", "o", "o", "o", "o"},
                                      {"", "", "o", "o", "o"},
                                      {"", "", "", "o", "o"},
                                      {"", "", "", "", "o"}};
    std::string firstFalseShift[5][5]{{"o", "", "", "", ""},
                                      {"", "o", "", "", ""},
                                      {"", "", "o", "", ""},
                                      {"", "", "", "o", ""},
                                      {"", "", "", "", "o"}};
    std::string firstFalseEmpty[5]{"", "", "", "", ""};
    for (int i = 0; i < 5; i++) {
      assert(firstFalse(firstFalseLeft[i], 5) == 0);
      assert(firstFalse(firstFalseRight[i], 5) == i);
      assert(firstFalse(firstFalseShift[i], 5) == i);
    }
    assert(firstFalse(firstFalseEmpty, 5) == -1);
  }
  // Test positionOfLeast
  {
    std::string reverseOrdered[5]{"4", "3", "2", "1", "0"};
    std::string ordered[5]{"0", "1", "2", "3", "4"};
    std::string repeated[5]{"0", "1", "2", "3", "0"};
    std::string allLeast[5]{"0", "0", "0", "0", "0"};
    assert(positionOfLeast(reverseOrdered, 0) == -1);
    assert(positionOfLeast(reverseOrdered, 5) == 4);
    assert(positionOfLeast(ordered, 5) == 0);
    assert(positionOfLeast(repeated, 5) == 0);
    assert(positionOfLeast(allLeast, 5) == 0);
  }
  // Test has
  {
    std::string test1_a1[4]{"0", "1", "2", "3"};
    std::string test2_a1[5]{"0", "1", "2", "2", "3"};
    std::string valid1[4][3]{
        {"0", "1", "2"}, {"0", "1", "3"}, {"0", "2", "3"}, {"1", "2", "3"}};
    std::string valid2[6][2]{{"0", "1"}, {"0", "2"}, {"0", "3"},
                             {"1", "2"}, {"1", "3"}, {"2", "3"}};
    std::string valid3[4][1]{{"0"}, {"1"}, {"2"}, {"3"}};

    for (int i = 0; i < 4; i++) {
      assert(has(test1_a1, 4, valid1[i], 3));
      assert(has(test2_a1, 5, valid1[i], 3));
    }
    for (int i = 0; i < 6; i++) {
      assert(has(test1_a1, 4, valid2[i], 2));
      assert(has(test2_a1, 5, valid2[i], 2));
    }
    for (int i = 0; i < 4; i++) {
      assert(has(test1_a1, 4, valid3[i], 1));
      assert(has(test2_a1, 5, valid3[i], 1));
    }
    assert(has(test1_a1, 0, test1_a1, 0));
    assert(has(test1_a1, 4, test1_a1, 0));
    assert(has(test1_a1, 4, test1_a1, 4));
    assert(has(test1_a1, 0, test1_a1, 4) == false);
    assert(has(test1_a1, 4, test2_a1, 5) == false);

    std::string invalid1[2][5]{{"0", "1", "2", "3", "4"},
                               {"4", "0", "1", "2", "3"}};
    std::string invalid2[3][4]{
        {"0", "1", "2", "4"}, {"4", "0", "1", "2"}, {"1", "2", "3", "4"}};
    std::string invalid3[3][3]{
        {"1", "2", "4"}, {"4", "0", "1"}, {"2", "3", "4"}};
    std::string invalid4[3][2]{{"2", "4"}, {"4", "0"}, {"3", "4"}};
    std::string invalid5[1]{"4"};

    for (int i = 0; i < 2; i++) {
      assert(has(test1_a1, 4, invalid1[i], 5) == false);
    }
    for (int i = 0; i < 3; i++) {
      assert(has(test1_a1, 4, invalid2[i], 4) == false);
    }
    for (int i = 0; i < 3; i++) {
      assert(has(test1_a1, 4, invalid3[i], 3) == false);
    }
    for (int i = 0; i < 3; i++) {
      assert(has(test1_a1, 4, invalid4[i], 2) == false);
    }
    assert(has(test1_a1, 4, invalid5, 1) == false);
  }
  std::cout << "           ',\n        .-`-,\\__\n          .\"`   `,\n        "
               ".'_.  ._  `;.\n    __ / `      `  `.\\ .--.\n   /--,| 0)   0)  "
               "   )`_.-,)\n  |    ;.-----.__ _-');   /\n   '--./         `.`/ "
               " `\"`\n      :   '`      |.\n      | \     /  //\n       \\ "
               "'---'  /'\n        `------' \\\n         _/       `--..."
            << std::endl;
}

#endif
