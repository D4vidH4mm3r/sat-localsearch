#include "input.h"
#include <iostream>
#include <fstream>
#include <sstream>

using std::ifstream;

Formula readInstance(string file_name) {
  ifstream filestream(file_name.c_str());
  Formula formula(0);
  if (!filestream) {
    std::cerr << "Cannot open file " << file_name << std::endl;
    exit(1);
  }
  string line;
  std::istringstream iss;
  do {
    getline(filestream, line);
  } while (line[0] == 'c');

  // please remember to end file with at least one of these
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
  return formula;
}
