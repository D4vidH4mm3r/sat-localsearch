#include <algorithm>
#include <iostream>
#include <random>
#include "state.h"

State::State(Input* input, std::minstd_rand& randGen, int initType) :
  input(input),
  inst(input->numLiterals),
  cost(input->numClauses),
  S(input->numClauses, 0) {
  // initialize some instantiation
  if (initType == 0) {
    std::uniform_int_distribution<int> randDist(0, 1);
    for (unsigned int i=0; i<inst.size(); i++) {
      inst[i] = randDist(randGen);
    }
  } else if (initType == 1) {
    // wish to examine each literal once, but in random order
    vector<int> literalsToExamine(input->numLiterals);
    std::iota(literalsToExamine.begin(), literalsToExamine.end(), 0);
    std::shuffle(literalsToExamine.begin(), literalsToExamine.end(), randGen);
    vector<bool> clauseSatisfied(input->numClauses, 0);
    for (auto v: literalsToExamine) {
      int posWouldSatisfy = std::count_if(input->posInClause[v].begin(), input->posInClause[v].end(), [&] (int const i) {return clauseSatisfied[i];});
      int negWouldSatisfy = std::count_if(input->negInClause[v].begin(), input->negInClause[v].end(), [&] (int const i) {return clauseSatisfied[i];});
      if (posWouldSatisfy > negWouldSatisfy) {
        inst[v] = true;
        for (auto c: input->posInClause[v]) {
          clauseSatisfied[c] = true;
        }
      } else { // NOTE: setting to false redundant in init
        inst[v] = false;
        for (auto c: input->negInClause[v]) {
          clauseSatisfied[c] = true;
        }
      }
    }
  } else {
    throw "Unknown init type (only have 0 for random and 1 for randomized greedy)";
  }
  // compute what is failed and so on
  recomputeFailed(false);
}

State::State(const State& state) :
  input(state.input),
  inst(state.inst),
  cost(state.cost),
  S(state.S) {
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
    std::fill(S.begin(), S.end(), 0);
  }
  int clauseNum = 0;
  for (Clause clause : input->formula) {
    bool clauseSatisfied = false;
    for (int lit : clause) {
      if (lit > 0) {
        int litIndex = lit - 1;
        if (inst[litIndex]) {
          S[clauseNum]++;
          clauseSatisfied = true;
        }
      } else {
        int litIndex = -lit - 1;
        if (!inst[litIndex]) {
          S[clauseNum]++;
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
    if (S[clauseIndex] == 0) {
      // flipping one literal in failed clause will always satisfy it
      res--;
    } else if (S[clauseIndex] == 1 && !valAfterFlip) {
      // flipping the literal may fail this one if it was the one satisfying
      res++;
    } // if more than two satisfy, nothing changes for this clause
  }

  for (int clause : input->negInClause[literal-1]) { // symmetric
    int clauseIndex = clause - 1;
    if (S[clauseIndex] == 0) {
      res--;
    } else if (S[clauseIndex] == 1 && valAfterFlip) {
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
      S[clauseIndex]--;
      if (S[clauseIndex] == 0) {
        cost++;
      }
    } else {
      // this used to not satisfy the clause
      S[clauseIndex]++;
      if (S[clauseIndex] == 1) {
        cost--;
      }
    }
  }

  for (int clause : input->negInClause[literal-1]) { // symmetric
    clauseIndex = clause - 1;
    if (valAfterFlip) {
      S[clauseIndex]--;
      if (S[clauseIndex] == 0) {
        cost++;
      }
    } else {
      S[clauseIndex]++;
      if (S[clauseIndex] == 1) {
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

bool operator<(const State& lhs, const State& rhs) {
  return lhs.cost < rhs.cost;
}
