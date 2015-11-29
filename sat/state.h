#ifndef _H_STATE
#define _H_STATE
#include "input.h"

class SATState {
 public:
  SATState(SATInput*, Instantiation);
  SATInput* input;
  int numSatisfied;
  int numFailed;
  vector<bool> satisfied;
  vector<vector<int> > literalInClauses;
  // TODO: store instantiation in state
  int flipDelta(int literal, Instantiation inst); // note: not 0-indexed
};

#endif
