#ifndef HISTORY_H
#define HISTORY_H

class Mesa;

class History {
public:
  History(int nRows, int nCols);
  bool record(int r, int c);
  void display() const;

private:
};

#endif // HISTORY_H
