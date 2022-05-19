#define DO_NOT_SUBMIT_FOR_HOMEWORK_SIX
#ifdef DO_NOT_SUBMIT_FOR_HOMEWORK_SIX

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

vector<int> destroyedOnes;

class Restaurant {
public:
  Restaurant(int s) : m_stars(s) {}
  ~Restaurant() { destroyedOnes.push_back(m_stars); }
  int stars() const { return m_stars; }

private:
  int m_stars;
};

#endif // DO_NOT_SUBMIT_FOR_HOMEWORK_SIX

// Remove the restaurants in v with 2 or fewer stars and destroy them.
// It is acceptable if the order of the remaining restaurants is not
// the same as in the original vector.
void removeBad(vector<Restaurant *> &v) {
  auto it = v.begin();

  while (it != v.end()) {
    if ((*it)->stars() <= 2) {
      delete *it;
      it = v.erase(it);
    } else {
      it++;
    }
  }
}

#ifdef DO_NOT_SUBMIT_FOR_HOMEWORK_SIX

void test() {
  int a[8] = {4, 3, 2, 4, 1, 1, 5, 2};
  vector<Restaurant *> x;
  x.reserve(8);
  for (int k = 0; k < 8; k++) {
    x.push_back(new Restaurant(a[k]));
  }
  assert(x.size() == 8 && x.front()->stars() == 4 && x.back()->stars() == 2);
  removeBad(x);
  assert(x.size() == 4 && destroyedOnes.size() == 4);
  vector<int> v;
  v.reserve(4);
  for (int k = 0; k < 4; k++) {
    v.push_back(x[k]->stars());
  }
  sort(v.begin(), v.end());
  int expect[4] = {3, 4, 4, 5};
  for (int k = 0; k < 4; k++) {
    assert(v[k] == expect[k]);
  }
  sort(destroyedOnes.begin(), destroyedOnes.end());
  int expectGone[4] = {1, 1, 2, 2};
  for (int k = 0; k < 4; k++) {
    assert(destroyedOnes[k] == expectGone[k]);
  }
  for (vector<Restaurant *>::iterator p = x.begin(); p != x.end(); p++) {
    delete *p;
  }
}

int main() {
  test();
  cout << "Passed" << endl;
}

#endif // DO_NOT_SUBMIT_FOR_HOMEWORK_SIX
