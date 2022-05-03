#define DO_NOT_INCLUDE_IN_SUBMISSION

#ifdef DO_NOT_INCLUDE_IN_SUBMISSION

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
  if (n <= 0) {
    return -1;
  }

  if (n == 1) {
    return 0;
  }

  const int min_index = positionOfLeast(a, n - 1);
  const std::string candidate = a[n - 1];
  const std::string min = a[min_index];

  if (candidate <= min) {
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
  if (n1 < 0 || n2 < 0) {
    return false;
  }

  // Invalid subsequence
  if (n1 == 0 && n2 > 0) {
    return false;
  }

  // Valid subsequence
  if (n1 == 0 && n2 == 0) {
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
  std::string test[] = {"a", "b", "c", "", "e"};
  const int test_size = 5;

  std::cerr << std::boolalpha << has(test, test_size, test, test_size) << "\n";
}

#endif
