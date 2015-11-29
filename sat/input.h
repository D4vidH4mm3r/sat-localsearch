#ifndef _H_INPUT
#define _H_INPUT
#include <vector>
#include <string>

using std::vector;
using std::string;

typedef vector<int> Clause;
typedef vector<Clause> Formula;
typedef vector<bool> Instantiation;

class SATInput {
 public:
  int numClauses;
  int numLiterals;
  Formula formula;
  SATInput(string fileName);
};

std::ostream& operator<<(std::ostream& os, const Clause& c);
#endif
