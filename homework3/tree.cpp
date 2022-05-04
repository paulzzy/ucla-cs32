#define DO_NOT_INCLUDE_IN_SUBMISSION

#ifdef DO_NOT_INCLUDE_IN_SUBMISSION

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <string>

#endif

// Return the number of ways that all n2 elements of a2 appear
// in the n1 element array a1 in the same order (though not
// necessarily consecutively).  The empty sequence appears in a
// sequence of length n1 in 1 way, even if n1 is 0.
// For example, if a1 is the 7 element array
//    "merida" "tiana" "raya" "belle" "tiana" "raya" "moana"
// then for this value of a2            the function must return
//    "merida" "belle" "raya"                      1
//    "merida" "raya" "moana"                      2
//    "belle" "merida" "raya"                      0
//    "tiana" "raya" "moana"                       3
// NOLINTNEXTLINE(misc-no-recursion)
int numberOfTimesHas(const std::string a1[], int n1, const std::string a2[],
                     int n2) {
  // Negative sizes
  if (n1 < 0) {
    n1 = 0;
  }

  if (n2 < 0) {
    n2 = 0;
  }

  // Invalid subsequence
  if (n1 == 0 && n2 > 0) {
    return 0;
  }

  // Valid subsequence
  if (n1 >= 0 && n2 == 0) {
    return 1;
  }

  const std::string a1_last = a1[n1 - 1];
  const std::string a2_last = a2[n2 - 1];
  int counter = 0;

  if (a1_last == a2_last) {
    counter += numberOfTimesHas(a1, n1 - 1, a2, n2 - 1);
  }

  counter += numberOfTimesHas(a1, n1 - 1, a2, n2);

  return counter;
}

// Exchange two strings
void exchange(std::string &x, std::string &y) {
  std::string t = x;
  x = y;
  y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is < splitter come before all the other elements,
// and all the elements whose value is > splitter come after all
// the other elements.  Upon return, firstNotLess is set to the
// index of the first element in the rearranged array that is
// >= splitter, or n if there is no such element, and firstGreater is
// set to the index of the first element that is > splitter, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotLess, a[i] < splitter
//   * for firstNotLess <= i < firstGreater, a[i] == splitter
//   * for firstGreater <= i < n, a[i] > splitter
// All the elements < splitter end up in no particular order.
// All the elements > splitter end up in no particular order.
// NOLINTNEXTLINE(performance-unnecessary-value-param)
void split(std::string a[], int n, std::string splitter,
           // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
           int &firstNotLess, int &firstGreater) {
  if (n < 0) {
    n = 0;
  }

  // It will always be the case that just before evaluating the loop
  // condition:
  //  firstNotLess <= firstUnknown and firstUnknown <= firstGreater
  //  Every element earlier than position firstNotLess is < splitter
  //  Every element from position firstNotLess to firstUnknown-1 is
  //    == splitter
  //  Every element from firstUnknown to firstGreater-1 is not known yet
  //  Every element at position firstGreater or later is > splitter

  firstNotLess = 0;
  firstGreater = n;
  int firstUnknown = 0;
  while (firstUnknown < firstGreater) {
    if (a[firstUnknown] > splitter) {
      firstGreater--;
      exchange(a[firstUnknown], a[firstGreater]);
    } else {
      if (a[firstUnknown] < splitter) {
        exchange(a[firstNotLess], a[firstUnknown]);
        firstNotLess++;
      }
      firstUnknown++;
    }
  }
}

// Rearrange the elements of the array so that
// a[0] <= a[1] <= a[2] <= ... <= a[n-2] <= a[n-1]
// If n <= 1, do nothing.
// NOLINTNEXTLINE(misc-no-recursion)
void order(std::string a[], int n) {
  if (n <= 1) {
    return;
  }

  int left_upper_bound = 0;
  int right_lower_bound = 0;
  split(a, n, a[n / 2], left_upper_bound, right_lower_bound);

  order(a, left_upper_bound);
  order(a + right_lower_bound, n - right_lower_bound);
}

#ifdef DO_NOT_INCLUDE_IN_SUBMISSION

int main() {
  // `numberOfTimesHas`
  const std::string test1_1[] = {"merida", "tiana", "raya", "belle",
                                 "tiana",  "raya",  "moana"};
  const std::string test1_2[] = {"merida", "belle", "raya"};
  const std::string test1_3[] = {"merida", "raya", "moana"};
  const std::string test1_4[] = {"belle", "merida", "raya"};
  const std::string test1_5[] = {"tiana", "raya", "moana"};

  assert(numberOfTimesHas(test1_1, 7, test1_2, 3) == 1);
  assert(numberOfTimesHas(test1_1, 7, test1_3, 3) == 2);
  assert(numberOfTimesHas(test1_1, 7, test1_4, 3) == 0);
  assert(numberOfTimesHas(test1_1, 7, test1_5, 3) == 3);

  {
    std::string test1_a1[4]{"0", "1", "2", "3"};
    std::string test2_a1[5]{"0", "1", "2", "2", "3"};
    std::string valid1[4][3]{
        {"0", "1", "2"}, {"0", "1", "3"}, {"0", "2", "3"}, {"1", "2", "3"}};
    std::string valid2[6][2]{{"0", "1"}, {"0", "2"}, {"0", "3"},
                             {"1", "2"}, {"1", "3"}, {"2", "3"}};
    std::string valid3[4][1]{{"0"}, {"1"}, {"2"}, {"3"}};

    for (int i = 0; i < 4; i++) {
      assert(numberOfTimesHas(test1_a1, 4, valid1[i], 3) == 1);
    }
    assert(numberOfTimesHas(test2_a1, 5, valid1[0], 3) == 2);
    assert(numberOfTimesHas(test2_a1, 5, valid1[1], 3) == 1);
    assert(numberOfTimesHas(test2_a1, 5, valid1[2], 3) == 2);
    assert(numberOfTimesHas(test2_a1, 5, valid1[3], 3) == 2);
    for (int i = 0; i < 6; i++) {
      assert(numberOfTimesHas(test1_a1, 4, valid2[i], 2) == 1);
    }
    assert(numberOfTimesHas(test2_a1, 5, valid2[0], 2) == 1);
    assert(numberOfTimesHas(test2_a1, 5, valid2[1], 2) == 2);
    assert(numberOfTimesHas(test2_a1, 5, valid2[2], 2) == 1);
    assert(numberOfTimesHas(test2_a1, 5, valid2[3], 2) == 2);
    assert(numberOfTimesHas(test2_a1, 5, valid2[4], 2) == 1);
    assert(numberOfTimesHas(test2_a1, 5, valid2[5], 2) == 2);
    for (int i = 0; i < 4; i++) {
      assert(numberOfTimesHas(test1_a1, 4, valid3[i], 1));
      assert(numberOfTimesHas(test2_a1, 5, valid3[i], 1));
    }
    assert(numberOfTimesHas(test1_a1, 0, test1_a1, 0) == 1);
    assert(numberOfTimesHas(test1_a1, 4, test1_a1, 0) == 1);
    assert(numberOfTimesHas(test1_a1, 4, test1_a1, 4) == 1);
    assert(numberOfTimesHas(test1_a1, 0, test1_a1, 4) == 0);
    assert(numberOfTimesHas(test1_a1, 4, test2_a1, 5) == 0);

    std::string invalid1[2][5]{{"0", "1", "2", "3", "4"},
                               {"4", "0", "1", "2", "3"}};
    std::string invalid2[3][4]{
        {"0", "1", "2", "4"}, {"4", "0", "1", "2"}, {"1", "2", "3", "4"}};
    std::string invalid3[3][3]{
        {"1", "2", "4"}, {"4", "0", "1"}, {"2", "3", "4"}};
    std::string invalid4[3][2]{{"2", "4"}, {"4", "0"}, {"3", "4"}};
    std::string invalid5[1]{"4"};

    for (int i = 0; i < 2; i++) {
      assert(numberOfTimesHas(test1_a1, 4, invalid1[i], 5) == 0);
    }
    for (int i = 0; i < 3; i++) {
      assert(numberOfTimesHas(test1_a1, 4, invalid2[i], 4) == 0);
    }
    for (int i = 0; i < 3; i++) {
      assert(numberOfTimesHas(test1_a1, 4, invalid3[i], 3) == 0);
    }
    for (int i = 0; i < 3; i++) {
      assert(numberOfTimesHas(test1_a1, 4, invalid4[i], 2) == 0);
    }
    assert(numberOfTimesHas(test1_a1, 4, invalid5, 1) == 0);
  }

  {
    std::string a[7] = {"10", "50", "40", "20", "50", "40", "30"};
    std::string a1[3] = {"10", "20", "40"};
    std::string a2[3] = {"10", "40", "30"};
    std::string a3[3] = {"20", "10", "40"};
    std::string a4[3] = {"50", "40", "30"};
    assert(numberOfTimesHas(a, 7, a1, 3) == 1);
    assert(numberOfTimesHas(a, 7, a2, 3) == 2);
    assert(numberOfTimesHas(a, 7, a3, 3) == 0);
    assert(numberOfTimesHas(a, 7, a4, 3) == 3);
    assert(numberOfTimesHas(a, -1, a4, 0) == 1);
    assert(numberOfTimesHas(a, -1, a4, 1) == 0);
  }

  // `order`
  std::string test2[] = {"5", "3", "4", "2", "1"};
  const int size2 = 5;
  order(test2, size2);
  assert(std::is_sorted(std::begin(test2), std::end(test2)));

  {
    //    10 20 40            1
    //    10 40 30            2
    //    20 10 40            0
    //    50 40 30            3

    std::string b[5] = {"4", "3", "5", "1", "1"};
    order(b, 5);

    for (int i = 0; i < 5; i++) {
      std::cerr << b[i] << " ";
    }

    std::cerr << std::endl;
  }

  std::cerr << "noice passed" << std::endl;
}

#endif
