#include "input.h"
#include <iostream>
#include <fstream>
#include <sstream>

using std::ifstream;

SATInput::SATInput(string file_name) : formula() {
  ifstream filestream(file_name.c_str());
  if (!filestream) {
    std::cerr << "Cannot open file " << file_name << std::endl;
    exit(1);
  }
  string line;
  std::istringstream iss;
  do {
    getline(filestream, line);
  } while (line[0] == 'c');

  iss.str(line);
  // first interesting line has "p cnf <nlits> <nclauses>"
  string sub;
  iss >> sub;
  iss >> sub;
  iss >> numLiterals;
  iss >> numClauses;
  getline(filestream, line);
  while (line[0] != '%' && line[0] != '0') {
    Clause clause(0);
    iss.clear();
    iss.str(line);
    int lit;
    iss >> lit;
    while (lit != 0) {
      clause.push_back(lit);
      iss >> lit;
    }
    formula.push_back(clause);
    getline(filestream, line);
  }
}

std::ostream& operator<<(std::ostream& os, const Clause& c) {
  for (auto iter = c.begin(); iter < c.end()-1; iter++) {
    os << *iter << " ∨ ";
  }
  if (c.size() > 0) {
    os << c.back();
  }
  return os;
}
