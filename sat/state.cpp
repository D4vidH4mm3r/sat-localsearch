#include "state.h"

SATState::SATState(SATInput input, Instantiation inst) :
  numSatisfied(0),
  numFailed(input.numClauses),
  satisfied(input.numClauses, false),
  literalInClauses(input.numLiterals, vector<int>(0)) {
  int clauseNum = 0;
  for (Clause clause : input.formula) {
    bool clauseSatisfied = false;
    for (int lit : clause) {
      // put lit -> clause in map also
      int absLit = lit > 0 ? lit : -lit;
      literalInClauses[absLit-1].push_back(clauseNum);
      if ((lit > 0 && inst[lit-1]) || (lit < 0 && !inst[-lit-1])) {
        satisfied[clauseNum] = true;
        clauseSatisfied = true;
      }
    }
    if (clauseSatisfied) {
      numSatisfied++;
      numFailed--;
    }
    clauseNum++;
  }
}
