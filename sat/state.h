#ifndef _H_STATE
#define _H_STATE
#include "input.h"

class SATState {
 public:
  SATState(SATInput*, int);
  SATInput* input;
  Instantiation inst;
  int cost; // number of failed clauses
  // element i = (sign)j indicates that literal i appears with sign in clause j
  int flipDelta(int literal); // note: not 0-indexed
  void flip(int literal);
  void flip_slow(int literal);
  vector<int> numSatisfying;
 private:
  void recomputeFailed(bool zeroOut);
};

std::ostream& operator<<(std::ostream& os, const SATState& s);
#endif
