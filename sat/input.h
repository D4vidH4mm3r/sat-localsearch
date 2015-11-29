#include <vector>
#include <string>
#include <map>

using std::vector;
using std::string;

typedef vector<int> Clause;
typedef vector<Clause> Formula;

class SATInput {
 public:
  int numClauses;
  int numLiterals;
  Formula formula;
  vector<vector<int> > literalInClauses;
  SATInput(string fileName);
};

std::ostream& operator<<(std::ostream& os, const Clause& c);
