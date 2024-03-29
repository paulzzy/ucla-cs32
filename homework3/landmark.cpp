#define DO_NOT_INCLUDE_IN_SUBMISSION

#ifdef DO_NOT_INCLUDE_IN_SUBMISSION

#include <iostream>
#include <string>

#endif

class Landmark {
public:
  // NOLINTNEXTLINE(performance-unnecessary-value-param)
  Landmark(std::string name) : m_name(name) {}
  virtual ~Landmark() {}

  std::string name() const { return m_name; }
  virtual std::string color() const { return "yellow"; }
  virtual std::string icon() const = 0;

private:
  std::string m_name;
};

class Hotel : public Landmark {
public:
  // NOLINTNEXTLINE(performance-unnecessary-value-param)
  Hotel(std::string name) : Landmark(name) {}
  ~Hotel() { std::cout << "Destroying the hotel " << name() << ".\n"; }

  std::string icon() const { return "bed"; }
};

class Restaurant : public Landmark {
public:
  Restaurant(std::string name, int capacity)
      // NOLINTNEXTLINE(performance-unnecessary-value-param)
      : Landmark(name), m_capacity(capacity) {}
  ~Restaurant() {
    std::cout << "Destroying the restaurant " << name() << ".\n";
  }

  std::string icon() const {
    if (m_capacity < 40) { // NOLINT(readability-magic-numbers)
      return "small knife/fork";
    }

    return "large knife/fork";
  }

private:
  int m_capacity;
};

class Hospital : public Landmark {
public:
  // NOLINTNEXTLINE(performance-unnecessary-value-param)
  Hospital(std::string name) : Landmark(name) {}
  ~Hospital() { std::cout << "Destroying the hospital " << name() << ".\n"; }

  std::string color() const { return "blue"; }
  std::string icon() const { return "H"; }
};

#ifdef DO_NOT_INCLUDE_IN_SUBMISSION

void display(const Landmark *lm) {
  std::cout << "Display a " << lm->color() << " " << lm->icon() << " icon for "
            << lm->name() << "." << std::endl;
}

int main() {
  Landmark *landmarks[4];
  landmarks[0] = new Hotel("Westwood Rest Good");
  // Restaurants have a name and seating capacity.  Restaurants with a
  // capacity under 40 have a small knife/fork icon; those with a capacity
  // 40 or over have a large knife/fork icon.
  landmarks[1] =
      new Restaurant("Bruin Bite", 30); // NOLINT(readability-magic-numbers)
  landmarks[2] = new Restaurant("La Morsure de l'Ours",
                                100); // NOLINT(readability-magic-numbers)
  landmarks[3] = new Hospital("UCLA Medical Center");

  std::cout << "Here are the landmarks." << std::endl;
  for (int k = 0; k < 4; k++) {
    display(landmarks[k]);
  }

  // Clean up the landmarks before exiting
  std::cout << "Cleaning up." << std::endl;
  for (int k = 0; k < 4; k++) {
    delete landmarks[k];
  }
}

#endif
