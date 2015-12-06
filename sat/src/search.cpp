#include <iostream>
#include "search.h"

using std::cout;
using std::endl;


void anneal(SATState& state, std::minstd_rand randGen, bool verbose) {
  std::uniform_int_distribution<int> randLit(1, state.input->numLiterals);
  std::uniform_real_distribution<double> randReal(0.0, 1.0);
  unsigned long iterMax = state.input->numClauses*state.input->numClauses;
  double Tmax = static_cast<double>(iterMax);
  double T = Tmax;
  // TODO: experiment with number of steps and temperatures
  for (unsigned long j=0; j<=iterMax; j++) {

    if (state.cost == 0) {
      if (verbose) {
        cout << "Nothing is failed :D" << endl;
      }
      break;
    }

    // choose randomly a literal to maybe flip
    int literal = randLit(randGen);
    int delta = state.flipDelta(literal);
    if (delta <= 0) {
      state.flip(literal);
      T = T * 0.95;
      continue;
    }
    // random cutoff
    double cutoff = randReal(randGen);
    double P = 2.0/(1 + exp(static_cast<double>(delta)/T));
    if (P > cutoff) {
      state.flip(literal);
    }

    // TODO: experiment with temperature distribution
    T = T * 0.95;
  }
}
