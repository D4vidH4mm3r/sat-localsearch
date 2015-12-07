#ifndef _H_STATE
#define _H_STATE
#include "input.h"
#include <random>

class State {
 public:
  State(Input*, std::minstd_rand&);
  State(const State&);
  Input* input;
  Instantiation inst;
  int cost; // number of failed clauses
  // element i = (sign)j indicates that literal i appears with sign in clause j
  int flipDelta(int literal); // note: not 0-indexed
  void flip(int literal);
  void flip_slow(int literal);
  void randomize(std::minstd_rand&);
  vector<int> numSatisfying;
 private:
  void recomputeFailed(bool zeroOut);
};

std::ostream& operator<<(std::ostream& os, const State& s);
#endif
