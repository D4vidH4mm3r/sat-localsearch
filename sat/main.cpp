#include "input.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

typedef vector<bool> Instantiation;

int main(int argc, const char* argv[]) {
  srand(time(0)); // seed random number generator
  string inputName;
  if (argc < 2) {
    std::cout << "No args given, running default instance thing" << std::endl;
    inputName = "data/uf20-010.cnf";
  }
  SATInput input(inputName);
  std::cout << "Read instance with " << input.numLiterals << " literals and ";
  std::cout << input.numClauses << " clauses" << std::endl;
  Instantiation inst(input.numLiterals);
  for (unsigned int i=0; i<inst.size(); i++) {
    inst[i] = rand()&1;
  }
  for (int i=0; i<input.numLiterals; i++) {
    std::cout << i+1 << " appears in";
    for (int j : input.literalInClauses[i]) {
      std::cout << " " << j;
    }
    std::cout << std::endl;
  }
  int numSatisfied = 0;
  int numFailed = 0;
  for (Clause clause : input.formula) {
    bool clauseSatisfied = false;
    for (int lit : clause) {
      if ((lit > 0 && inst[lit-1]) || (lit < 0 && !inst[-lit-1])) {
        clauseSatisfied = true;
        numSatisfied++;
        break;
      }
    }
    if (!clauseSatisfied) {
      numFailed++;
    }
  }
  std::cout << "Random instance satisfied " << numSatisfied << " and failed " << numFailed << std::endl;
  std::cout << "(" << static_cast<float>(numSatisfied) / static_cast<float>(input.numClauses) * 100 << "%)" << std::endl;
}
