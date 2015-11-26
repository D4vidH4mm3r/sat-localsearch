#include "input.h"
#include <iostream>

using std::string;

int main(int argc, const char* argv[]) {
  string inputName;
  if (argc < 2) {
    std::cout << "No args given, running default instance thing" << std::endl;
    inputName = "data/uf20-010.cnf";
  }
  SATInput input(inputName);
  std::cout << "Read instance with " << input.numLiterals << " literals and ";
  std::cout << input.numClauses << " clauses" << std::endl;
  for (auto clause : input.formula) {
    for (auto literal : clause) {
      std::cout << literal << " ";
    }
    std::cout << std::endl;
  }
}
