#include <algorithm>
#include <iostream>
#include "state.h"

SATState::SATState(SATInput* input, int starttype) :
  input(input),
  inst(input->numLiterals),
  numSatisfied(0),
  numFailed(input->numClauses),
  literalInClauses(input->numLiterals, vector<int>(0)),
  satisfied(input->numClauses, false) {
  // initialize some instantiation
  switch (starttype) {
  case 0: // random instantiation
    for (unsigned int i=0; i<inst.size(); i++) {
      inst[i] = rand()&1;
    }
    break;
  default:
    throw "Unknown start type";
  }
  // compute what is failed and so on
  recomputeFailed(false);
}

void SATState::recomputeFailed(bool zeroOut) {
  if (zeroOut) {
    numSatisfied = 0;
    numFailed = input->numClauses;
    std::fill(satisfied.begin(), satisfied.end(), false);
  }
  int clauseNum = 0;
  for (Clause clause : input->formula) {
    for (int lit : clause) {
      // put lit -> clause in map also
      int litIndex = (lit > 0 ? lit : -lit) - 1;
      literalInClauses[litIndex].push_back(clauseNum);
      if ((lit > 0 && inst[lit-1]) || (lit < 0 && !inst[-lit-1])) {
        satisfied[clauseNum] = true;
        numSatisfied++;
        numFailed--;
        break;
      }
    }
    clauseNum++;
  }
}

int SATState::flipDelta(int literal) {
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
          break;
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

void SATState::flip(int literal) {
  inst[literal-1] = !inst[literal-1];
  // update counts and auxilliary structures
  recomputeFailed(true);
}
