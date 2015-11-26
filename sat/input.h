#include <vector>
#include <string>

using std::vector;
using std::string;

typedef vector<int> Clause;
typedef vector<Clause> Formula;

class SATInput {
 public:
  int numClauses;
  int numLiterals;
  Formula formula;
  SATInput(string fileName);
};
