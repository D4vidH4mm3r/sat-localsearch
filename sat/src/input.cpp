#include <iostream>
#include <fstream>
#include <sstream>
#include "input.h"


Input::Input(string file_name) :
  formula(),
  posInClause(0),
  negInClause(0)
{
  std::ifstream filestream(file_name.c_str());
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
  posInClause.resize(numLiterals, vector<int>(0));
  negInClause.resize(numLiterals, vector<int>(0));
  getline(filestream, line);
  int clauseNum = 1;
  int lit;
  int litIndex;
  while (line[0] != '%' && line[0] != '0') {
    Clause clause(0);
    iss.clear();
    iss.str(line);
    iss >> lit;
    while (lit != 0) {
      clause.push_back(lit);
      if (lit > 0) {
        litIndex = lit - 1;
        posInClause[litIndex].push_back(clauseNum);
      } else {
        litIndex = -lit - 1;
        negInClause[litIndex].push_back(clauseNum);
      }
      iss >> lit;
    }
    formula.push_back(clause);
    clauseNum++;
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
