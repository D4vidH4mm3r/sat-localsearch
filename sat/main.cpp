#include "input.h"
#include "state.h"
#include <iostream>
#include <cassert>
#include <ctime>
#include <cstdlib>


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
  Instantiation inst(input->numLiterals);
  for (unsigned int i=0; i<inst.size(); i++) {
    inst[i] = rand()&1;
  }
  SATState state(input, inst);
  for (int i=0; i<input->numLiterals; i++) {
    std::cout << i+1 << "(" << inst[i] << ") appears in";
    for (int j : state.literalInClauses[i]) {
      std::cout << " " << j;
    }
    std::cout << std::endl;
  }
  std::cout << "Random instance satisfied " << state.numSatisfied << " and failed " << state.numFailed << std::endl;
  std::cout << "(" << static_cast<float>(state.numSatisfied) / static_cast<float>(input->numClauses) * 100 << "%)" << std::endl;
  for (int i=0; i<input->numLiterals; i++) {
    int delta = state.flipDelta(i+1, inst);
    std::cout << "Flipping " << i+1 << " would yield " << delta << std::endl;
    Instantiation copy(inst);
    copy[i] = !copy[i];
    assert(copy != inst);
    SATState stateAfter(input, copy);
    assert(stateAfter.numFailed == state.numFailed + delta);
  }
}
