#include <algorithm>
#include <iostream>
#include <random>
#include "state.h"

SATState::SATState(SATInput* input, int starttype) :
  input(input),
  inst(input->numLiterals),
  numSatisfied(0),
  numFailed(input->numClauses),
  literalInClauses(input->numLiterals, vector<int>(0)),
  numSatisfying(input->numClauses, 0) {
  // initialize some instantiation
  if (starttype == 0) {
    std::random_device randDev;
    std::minstd_rand randGen(randDev());
    std::uniform_int_distribution<int> randDist(0, 1);
    for (unsigned int i=0; i<inst.size(); i++) {
      inst[i] = randDist(randGen);
    }
  } else {
    throw "Unknown start type";
  }
  int clauseNum = 1;
  for (Clause clause : input->formula) {
    for (int lit : clause) {
      // put lit -> clause in map also
      if (lit > 0) {
        int litIndex = lit - 1;
        literalInClauses[litIndex].push_back(clauseNum);
      } else {
        int litIndex = -lit - 1;
        literalInClauses[litIndex].push_back(-clauseNum);
      }
    }
    clauseNum++;
  }
  // compute what is failed and so on
  recomputeFailed(false);
}

void SATState::recomputeFailed(bool zeroOut) {
  if (zeroOut) {
    numSatisfied = 0;
    numFailed = input->numClauses;
    std::fill(numSatisfying.begin(), numSatisfying.end(), 0);
  }
  int clauseNum = 0;
  for (Clause clause : input->formula) {
    bool clauseSatisfied = false;
    for (int lit : clause) {
      if (lit > 0) {
        int litIndex = lit - 1;
        if (inst[litIndex]) {
          numSatisfying[clauseNum]++;
          clauseSatisfied = true;
        }
      } else {
        int litIndex = -lit - 1;
        if (!inst[litIndex]) {
          numSatisfying[clauseNum]++;
          clauseSatisfied = true;
        }
      }
    }
    if (clauseSatisfied) {
      numSatisfied++;
      numFailed--;
    }
    clauseNum++;
  }
}

int SATState::flipDelta(int literal) {
  int res = 0;
  bool valAfterFlip = !inst[literal-1];
  for (int clause : literalInClauses[literal-1]) {
    if (clause > 0) {
      int clauseIndex = clause - 1;
      if (numSatisfying[clauseIndex] == 0) {
        // flipping one literal in failed clause will always satisfy it
        res--;
      } else if (numSatisfying[clauseIndex] == 1 && !valAfterFlip) {
        // flipping the literal may fail this one if it was the one satisfying
        res++;
      } // if more than two satisfy, nothing changes for this clause
    } else { // symmetric
      int clauseIndex = -clause -1;
      if (numSatisfying[clauseIndex] == 0) {
        res--;
      } else if (numSatisfying[clauseIndex] == 1 && valAfterFlip) {
        res++;
      }
    }
  }
  return res;
}

void SATState::flip_slow(int literal) {
  inst[literal-1] = !inst[literal-1];
  // update counts and auxilliary structures
  recomputeFailed(true);
}

void SATState::flip(int literal) {
  bool valAfterFlip = !inst[literal-1];
  inst[literal-1] = valAfterFlip;
  // update counts and auxilliary structures
  for (int clause : literalInClauses[literal-1]) {
    int clauseIndex;
    if (clause > 0) {
      clauseIndex = clause - 1;
      if (!valAfterFlip) {
        // this used to satisfy the clause
        numSatisfying[clauseIndex]--;
        if (numSatisfying[clauseIndex] == 0) {
          numSatisfied--;
          numFailed++;
        }
      } else {
        // this used to not satisfy the clause
        numSatisfying[clauseIndex]++;
        if (numSatisfying[clauseIndex] == 1) {
          numSatisfied++;
          numFailed--;
        }
      }
    } else { // symmetric
      clauseIndex = -clause - 1;
      if (valAfterFlip) {
        numSatisfying[clauseIndex]--;
        if (numSatisfying[clauseIndex] == 0) {
          numSatisfied--;
          numFailed++;
        }
      } else {
        numSatisfying[clauseIndex]++;
        if (numSatisfying[clauseIndex] == 1) {
          numSatisfied++;
          numFailed--;
        }
      }
    }
  }
}

std::ostream& operator<<(std::ostream& os, const SATState& s) {
  os << "v:";
  int litNum = 1;
  for (bool val : s.inst) {
    if (val) {
      os << " " << litNum;
    } else {
      os << " " << -litNum;
    }
    litNum++;
  }
  os << std::endl << std::endl;
  os << "Cost: " << s.numFailed << std::endl;
  os << "Time: " << "NOT COMPUTED :(" << std::endl;
  return os;
}
