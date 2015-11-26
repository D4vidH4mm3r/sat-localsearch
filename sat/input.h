#include <vector>
#include <string>

using std::vector;
using std::string;

typedef vector<int> Clause;
typedef vector<Clause> Formula;

Formula readInstance(string);
