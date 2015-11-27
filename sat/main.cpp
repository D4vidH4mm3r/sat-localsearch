#include "input.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

using std::string;

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
  for (Clause clause : input.formula) {
    std::cout << clause << std::endl;
  }
}
