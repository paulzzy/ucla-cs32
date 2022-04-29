// Return false if the somePredicate function returns true for at
// least one of the array elements; return true otherwise.
bool allFalse(const string a[], int n) {
  return false; // This is not always correct.
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const string a[], int n) {
  return -999; // This is incorrect.
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const string a[], int n) {
  return -999; // This is incorrect.
}

// Return the subscript of the least string in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k] for all
// k from 0 to n-1).  If the function is told that no strings are to
// be considered in the array, return -1.
int positionOfLeast(const string a[], int n) {
  return -999; // This is incorrect.
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
bool has(const string a1[], int n1, const string a2[], int n2) {
  return false; // This is not always correct.
}
