#include "state.h"

SATState::SATState(SATInput* input, Instantiation inst) :
  input(input),
  numSatisfied(0),
  numFailed(input->numClauses),
  satisfied(input->numClauses, false),
  literalInClauses(input->numLiterals, vector<int>(0)) {
  int clauseNum = 0;
  for (Clause clause : input->formula) {
    bool clauseSatisfied = false;
    for (int lit : clause) {
      // put lit -> clause in map also
      int litIndex = (lit > 0 ? lit : -lit) - 1;
      literalInClauses[litIndex].push_back(clauseNum);
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

int SATState::flipDelta(int literal, Instantiation inst) {
  int res = 0;
  for (int clauseIndex : literalInClauses[literal-1]) {
    if (satisfied[clauseIndex]) {
      // flipping one literal in satisfied clause may fail it
      bool stillSatisfied = false;
      for (int otherLiteral : input->formula[clauseIndex]) {
        int otherIndex = (otherLiteral > 0 ? otherLiteral : -otherLiteral) - 1;
        bool thisSatisfies = ((otherLiteral > 0 && inst[otherIndex]) || (otherLiteral < 0 && !inst[otherIndex]));
        if (otherIndex == literal-1) {
          thisSatisfies = !thisSatisfies;
        }
        if (thisSatisfies) {
          stillSatisfied = true;
        }
      }
      if (!stillSatisfied) {
        res++;
      }
    } else {
      // flipping one literal in failed clause will always satisfy it
      res--;
    }
  }
  return res;
}
