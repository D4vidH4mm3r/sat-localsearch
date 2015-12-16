#ifndef _H_STATE
#define _H_STATE
#include "input.h"
#include <random>

class State {
 public:
  State(Input*, std::minstd_rand&, int);
  State(const State&);
  Input* input;
  Instantiation inst;
  int cost; // number of failed clauses
  int flipDelta(int literal); // note: not 0-indexed
  void flip(int literal);
  void flip_slow(int literal);
  void randomize(std::minstd_rand&);
  vector<int> S; // S[i] = how many literals satisfy clause i
 private:
  void recomputeFailed(bool zeroOut);
};

std::ostream& operator<<(std::ostream& os, const State& s);
bool operator<(const State& lhs, const State& rhs);
#endif
