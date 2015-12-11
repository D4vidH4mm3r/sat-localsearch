#include <algorithm>
#include <iostream>
#include <random>
#include "state.h"

State::State(Input* input, std::minstd_rand& randGen) :
  input(input),
  inst(input->numLiterals),
  cost(input->numClauses),
  numSatisfying(input->numClauses, 0) {
  // initialize some instantiation
  std::uniform_int_distribution<int> randDist(0, 1);
  for (unsigned int i=0; i<inst.size(); i++) {
    inst[i] = randDist(randGen);
  }
  // compute what is failed and so on
  recomputeFailed(false);
}

State::State(const State& state) :
  input(state.input),
  inst(state.inst),
  cost(state.cost),
  numSatisfying(state.numSatisfying) {
}

void State::randomize(std::minstd_rand& randGen) {
  std::uniform_int_distribution<int> randDist(0, 1);
  for (unsigned int i=0; i<inst.size(); i++) {
    inst[i] = randDist(randGen);
  }
  recomputeFailed(true);
}

void State::recomputeFailed(bool zeroOut) {
  if (zeroOut) {
    cost = input->numClauses;
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
      cost--;
    }
    clauseNum++;
  }
}

int State::flipDelta(int literal) {
  int res = 0;
  bool valAfterFlip = !inst[literal-1];
  int litIndex = literal-1;
  int clauseIndex;

  for (int clause : input->posInClause[litIndex]) {
    clauseIndex = clause - 1;
    if (numSatisfying[clauseIndex] == 0) {
      // flipping one literal in failed clause will always satisfy it
      res--;
    } else if (numSatisfying[clauseIndex] == 1 && !valAfterFlip) {
      // flipping the literal may fail this one if it was the one satisfying
      res++;
    } // if more than two satisfy, nothing changes for this clause
  }

  for (int clause : input->negInClause[literal-1]) { // symmetric
    int clauseIndex = clause - 1;
    if (numSatisfying[clauseIndex] == 0) {
      res--;
    } else if (numSatisfying[clauseIndex] == 1 && valAfterFlip) {
      res++;
    }
  }

  return res;
}

void State::flip_slow(int literal) {
  inst[literal-1] = !inst[literal-1];
  // update counts and auxilliary structures
  recomputeFailed(true);
}

void State::flip(int literal) {
  bool valAfterFlip = !inst[literal-1];
  inst[literal-1] = valAfterFlip;
  // update counts and auxilliary structures
  int clauseIndex;

  for (int clause : input->posInClause[literal-1]) {
    clauseIndex = clause - 1;
    if (!valAfterFlip) {
      // this used to satisfy the clause
      numSatisfying[clauseIndex]--;
      if (numSatisfying[clauseIndex] == 0) {
        cost++;
      }
    } else {
      // this used to not satisfy the clause
      numSatisfying[clauseIndex]++;
      if (numSatisfying[clauseIndex] == 1) {
        cost--;
      }
    }
  }

  for (int clause : input->negInClause[literal-1]) { // symmetric
    clauseIndex = clause - 1;
    if (valAfterFlip) {
      numSatisfying[clauseIndex]--;
      if (numSatisfying[clauseIndex] == 0) {
        cost++;
      }
    } else {
      numSatisfying[clauseIndex]++;
      if (numSatisfying[clauseIndex] == 1) {
        cost--;
      }
    }
  }
}

std::ostream& operator<<(std::ostream& os, const State& s) {
  os << "v";
  int litNum = 1;
  for (bool val : s.inst) {
    if (val) {
      os << " " << litNum;
    } else {
      os << " " << -litNum;
    }
    litNum++;
  }
  os << " 0" << std::endl << std::endl;
  os << "c Cost: " << s.cost << std::endl;
  return os;
}
