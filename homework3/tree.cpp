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
  if (n1 < 0 || n2 < 0) {
    return 0;
  }

  // Invalid subsequence
  if (n1 == 0 && n2 > 0) {
    return 0;
  }

  // Valid subsequence
  if (n1 == 0 && n2 == 0) {
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
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void split(std::string a[], int n, const std::string &splitter,
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

}
