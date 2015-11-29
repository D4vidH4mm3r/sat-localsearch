#ifndef _H_STATE
#define _H_STATE
#include "input.h"

class SATState {
 public:
  int numSatisfied;
  int numFailed;
  vector<bool> satisfied;
  vector<vector<int> > literalInClauses;
  SATState(SATInput, Instantiation);
};

#endif
