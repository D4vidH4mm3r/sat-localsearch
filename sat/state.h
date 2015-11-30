#ifndef _H_STATE
#define _H_STATE
#include "input.h"

class SATState {
 public:
  SATState(SATInput*, int);
  SATInput* input;
  Instantiation inst;
  int numSatisfied;
  int numFailed;
  vector<vector<int> > literalInClauses;
  // element i = (sign)j indicates that literal i appears with sign in clause j
  int flipDelta(int literal); // note: not 0-indexed
  void flip(int literal);
 private:
  vector<int> numSatisfying;
  void recomputeFailed(bool zeroOut);
};

#endif
