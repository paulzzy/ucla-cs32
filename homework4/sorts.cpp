#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric> // for std::accumulate
#include <random>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

//========================================================================
//  Set this to false to skip the insertion sort tests; you'd do this if
//  you're sorting so many items that insertion_sort would take more time
//  than you're willing to wait.

const bool TEST_INSERTION_SORT = false;

//========================================================================

//========================================================================
// Timer t;                 // create and start a timer
// t.start();               // restart the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>

class Timer {
public:
  Timer() { start(); }
  void start() { m_time = std::chrono::high_resolution_clock::now(); }
  double elapsed() const {
    std::chrono::duration<double, std::milli> diff =
        std::chrono::high_resolution_clock::now() - m_time;
    return diff.count();
  }

private:
  std::chrono::high_resolution_clock::time_point m_time;
};

//========================================================================

// These are used for random number generation

std::random_device
    aRandomDevice; // NOLINT(cert-err58-cpp,
                   // cppcoreguidelines-avoid-non-const-global-variables)
std::default_random_engine
    generator( // NOLINT (cert-err58-cpp,
               // cppcoreguidelines-avoid-non-const-global-variables)
        aRandomDevice());

// Here's a class that is not cheap to copy because the objects contain a
// large array.

// We'll simplify writing our timing tests by declaring everything public
// in this class.  (We wouldn't make data public in a class intended for
// wider use.)

using IdType = int;

struct Sensor {
  IdType id;  // NOLINT(misc-non-private-member-variables-in-classes)
  double avg; // NOLINT(misc-non-private-member-variables-in-classes)
  static const int NREADINGS = 200;
  double readings[NREADINGS]{}; // NOLINT(cpp-coreguidelines-avoid-c-arrays,
                                // misc-non-private-member-variables-in-classes)
  Sensor(IdType i) : id(i) {
    const int UPPER_BOUND = 99;
    std::uniform_int_distribution<> distro(0, UPPER_BOUND);

    // create random sensor readings (from 0 to 99)
    for (size_t k = 0; k < NREADINGS; k++) {
      readings[k] // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
          = distro(generator);
    }
    // (accumulate computes 0.0 + readings[0] + readings[1] + ...)
    avg =
        accumulate(
            readings, // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
            readings // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
                +    // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
                NREADINGS,
            0.0) /
        NREADINGS;
  }
};

inline bool compareSensor(const Sensor &lhs, const Sensor &rhs) {
  // The Sensor with the higher average should come first.  If they have
  // the same average, then the Sensor with the smaller id number should
  // come first.  Return true iff lhs should come first.  Notice that
  // this means that a false return means EITHER that rhs should come
  // first, or there's a tie, so we don't care which comes first,

  if (lhs.avg > rhs.avg) {
    return true;
  }
  if (lhs.avg < rhs.avg) {
    return false;
  }
  return lhs.id < rhs.id;
}

inline bool compareSensorPtr(const Sensor *lhs, const Sensor *rhs) {
  // DONE: You implement this.  Using the same criteria as compareSensor,
  //       compare two Sensors POINTED TO by lhs and rhs.  Think about
  //       how you can do it in one line by calling compareSensor.

  // Just so this will compile for now, we'll pretend every comparison
  // results in a tie, so there's no preferred ordering.
  return compareSensor(*lhs, *rhs);
}

void insertion_sort(vector<Sensor> &s,
                    bool comp(const Sensor &, const Sensor &)) {
  // DONE: Using the insertion sort algorithm (pp. 332-333 in the
  //       Carrano book; also <a href="bookcode.html">here</a>), sort s
  //       according to the ordering relationship passed in as the
  //       parameter comp.

  // Note:  The insertion sort algorithm on pp. 312-313 of the Carrano
  // book 6th edition is incorrect; someone made a change from the 5th
  // edition and messed it up.  See the errata page entry for page 313 at
  // http://homepage.cs.uri.edu/~carrano/WMcpp6e

  // Just to show you how to use the second parameter, we'll write code
  // that sorts only a vector of 2 elements.  (This is *not* the
  // insertion sort algorithm.)

  // Note that if comp(x,y) is true, it means x must end up before y in the
  // final ordering.
  for (size_t unsorted{1}; unsorted < s.size(); unsorted++) {
    const Sensor current{s.at(unsorted)};
    size_t insert_location{unsorted};

    while (insert_location > 0 && comp(current, s.at(insert_location - 1))) {
      s.at(insert_location) = s.at(insert_location - 1);
      insert_location--;
    }

    s.at(insert_location) = current;
  }
}

// Report the results of a timing test

void report(const string &caption, double t, const vector<Sensor> &s) {
  if (t >= 1) {
    const int TWO_DECIMAL_PLACES = 100;
    t = round(TWO_DECIMAL_PLACES * t) / TWO_DECIMAL_PLACES;
  }
  cout << t << " milliseconds; " << caption << "; first few sensors are\n\t";
  size_t n = s.size();
  if (n > 4) {
    n = 4;
  }
  for (size_t k = 0; k < n; k++) {
    cout << " (" << s[k].id << ", " << s[k].avg << ")";
  }
  cout << endl;
}

// Determine if two vectors have the same elements (in a possibly different
// order).

bool is_permutation(const vector<Sensor> &v1, const vector<Sensor> &v2) {
  if (v1.size() != v2.size()) {
    return false;
  }
  unordered_multiset<double> m;
  m.reserve(v1.size());
  for (const auto &s : v1) {
    m.insert(s.avg);
  }
  for (const auto &s : v2) {
    auto p = m.find(s.avg);
    if (p == m.end()) {
      return false;
    }
    m.erase(p);
  }
  return true;
}

void sortUsingPtrs(vector<Sensor> &sensors,
                   bool comp(const Sensor *, const Sensor *));

int main() {
  size_t nsensors{0};
  cout << "Enter number of sensors to sort: ";
  cin >> nsensors;
  if (!cin || nsensors <= 0) {
    cout << "You must enter a positive number.  Goodbye." << endl;
    return 1;
  }

  // Create a random ordering of id numbers 0 through nsensors-1
  vector<IdType> ids;
  for (size_t j = 0; j < nsensors; j++) {
    ids.push_back(IdType(j));
  }
  shuffle(ids.begin(), ids.end(), generator); // from <algorithm>

  // Create a bunch of Sensors
  vector<Sensor> unorderedSensors;
  for (size_t k = 0; k < ids.size(); k++) {
    unorderedSensors.push_back(Sensor(ids[k]));
  }

  // Create a timer

  Timer timer;

  // Sort the Sensors using the STL sort algorithm.  It uses a variant
  // of quicksort called introsort.

  vector<Sensor> sensors(unorderedSensors);
  timer.start();
  sort(sensors.begin(), sensors.end(), compareSensor);
  report("STL sort", timer.elapsed(), sensors);
  assert(is_sorted(sensors.begin(), sensors.end(),
                   compareSensor)); // from <algorithm>
  assert(is_permutation(sensors, unorderedSensors));

  // Sort the already sorted array using the STL sort.  This should be
  // fast.

  timer.start();
  sort(sensors.begin(), sensors.end(), compareSensor);
  report("STL sort if already sorted", timer.elapsed(), sensors);
  assert(is_sorted(sensors.begin(), sensors.end(), compareSensor));
  assert(is_permutation(sensors, unorderedSensors));

  if (TEST_INSERTION_SORT) {
    // Sort the original unsorted array using insertion sort.  This
    // should be really slow.  If you have to wait more than a minute,
    // try rerunning the program with a smaller number of Sensors.

    sensors = unorderedSensors;
    timer.start();
    insertion_sort(sensors, compareSensor);
    double elapsed = timer.elapsed();
    if (is_sorted(sensors.begin(), sensors.end(), compareSensor) &&
        is_permutation(sensors, unorderedSensors)) {
      report("insertion sort if not already sorted", elapsed, sensors);

      // Sort the already sorted array using insertion sort.  This
      // should be fast.

      timer.start();
      insertion_sort(sensors, compareSensor);
      report("insertion sort if already sorted", timer.elapsed(), sensors);
      assert(is_sorted(sensors.begin(), sensors.end(), compareSensor));
      assert(is_permutation(sensors, unorderedSensors));
    } else {
      cout << "[Skipping tests of insertion_sort, since it's not correctly "
              "implemented.]"
           << endl;
    }
  }

  // Since Sensors are expensive to copy, and since the STL's sort copies
  // Sensors O(N log N) times, let's sort POINTERS to the Sensors, then
  // make one final pass to rearrange the Sensors according to the
  // reordered pointers.  We'll write some code; you write the rest
  // below in sortUsingPtrs.

  // Set sensors to the original unsorted sequence
  sensors = unorderedSensors;

  // Start the timing
  timer.start();

  // Sort using pointers
  sortUsingPtrs(sensors, compareSensorPtr);

  // Report the timing and verify that the sort worked
  report("STL sort of pointers", timer.elapsed(), sensors);

  // Until you correctly implement sortUsingPointers, this assertion will fail.
  assert(is_sorted(sensors.begin(), sensors.end(), compareSensor));
  assert(is_permutation(sensors, unorderedSensors));
}

void sortUsingPtrs(vector<Sensor> &sensors,
                   bool comp(const Sensor *, const Sensor *)) {
  // Since Sensors are expensive to copy, and since the STL's sort copies
  // Sensors O(N log N) times, let's sort POINTERS to the Sensors, then
  // make one final pass to rearrange the Sensors according to the
  // reordered pointers.  We'll write some code; you write the rest.

  // Create an auxiliary copy of sensors to faciliate the later reordering.
  vector<Sensor> auxSensors(sensors.begin(), sensors.end());

  // DONE:  Create a vector of Sensor pointers, and set each pointer
  //        to point to the corresponding Sensor in auxSensors.
  vector<Sensor *> sensor_ptrs;
  sensor_ptrs.reserve(auxSensors.size());

  for (Sensor &sensor : auxSensors) {
    sensor_ptrs.push_back(&sensor);
  }

  // DONE:  Sort the vector of pointers using the STL sort algorithm
  //        with comp as the ordering relationship.
  std::sort(sensor_ptrs.begin(), sensor_ptrs.end(), comp);

  // DONE:  Using the now-sorted vector of pointers, replace each Sensor
  //        in sensors with the Sensors from auxSensors in the correct
  //        order.
  for (size_t i = 0; i < sensors.size(); i++) {
    sensors.at(i) = *sensor_ptrs.at(i);
  }

  // auxSensors will be destroyed upon return from the function
}
