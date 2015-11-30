#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "input.h"
#include "state.h"


int main(int argc, const char* argv[]) {
  srand(time(0)); // seed random number generator
  string inputName;
  if (argc < 2) {
    std::cout << "No args given, running default instance thing" << std::endl;
    inputName = "data/uf20-010.cnf";
  }
  SATInput* input = new SATInput(inputName);
  std::cout << "Read instance with " << input->numLiterals << " literals and ";
  std::cout << input->numClauses << " clauses" << std::endl;
  SATState state(input, 0);
  std::cout << "Random instance satisfied " << state.numSatisfied << " and failed " << state.numFailed << std::endl;
  std::cout << "(" << static_cast<float>(state.numSatisfied) /\
    static_cast<float>(input->numClauses) * 100 << "%)" << std::endl;
  for (int i=0; i<input->numLiterals; i++) {
    int delta = state.flipDelta(i+1);
    std::cout << "Flipping " << i+1 << " would yield " << delta << std::endl;
    int oldFailed = state.numFailed;
    state.flip(i+1);
    std::cout << "Before: " << oldFailed << " after: " << state.numFailed << std::endl;
    assert(oldFailed + delta == state.numFailed);
  }
}
