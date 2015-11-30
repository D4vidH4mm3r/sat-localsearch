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
  // TODO: store instantiation in state
  int flipDelta(int literal); // note: not 0-indexed
  void flip(int literal);
 private:
  vector<bool> satisfied;
  void recomputeFailed(bool zeroOut);
};

#endif
