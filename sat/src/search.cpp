#include <cmath>
#include <iostream>
#include "search.h"
#include "util.h"

using std::cout;
using std::endl;


State anneal(State state, std::minstd_rand& randGen, std::atomic<bool>& stop) {
  std::uniform_int_distribution<int> randLit(1, state.input->numLiterals);
  std::uniform_real_distribution<double> randReal(0.0, 1.0);
  // TODO: experiment with number of steps and temperatures
  // TODO: experiment with temperature distribution
  unsigned stepsPerTemperature = state.input->numLiterals * (state.input->numLiterals-1);
  unsigned accepted = 0;
  unsigned rejected = 0;
  unsigned stepsWithoutImprovement = 0;
  int bestCost = state.input->numClauses + 1;
  double T = -1 / log(0.97);
  State best = state;

  while (true) {
    bool improved = false;
    for (unsigned i=0; i<stepsPerTemperature; i++) {
      if (state.cost == 0) {
        stop = true;
        return state;
      } else if (stop) {
        return best.cost < state.cost ? best : state;
      }
      // choose randomly a literal to maybe flip
      int literal = randLit(randGen);
      int delta = state.flipDelta(literal);
      if (delta <= 0) {
        state.flip(literal);
      } else {
        // random cutoff
        double P = exp(static_cast<double>(-delta)/T);
        double cutoff = randReal(randGen);
        if (P > cutoff) {
          accepted++;
          state.flip(literal);
        } else {
          rejected++;
        }
      }
      // TODO: is improvement understood as local or improving best known?
      if (state.cost < bestCost) {
        bestCost = state.cost;
        improved = true;
      }
    }
    if (improved) {
      stepsWithoutImprovement = 0;
    } else {
      stepsWithoutImprovement++;
      double ratio = static_cast<double>(accepted)/static_cast<double>(accepted+rejected);
      if (stepsWithoutImprovement >= 5 && ratio < 0.02) {
        break;
      }
    }
    T = T * 0.95;
  }
  return state;
}

State minConflict(State state, std::minstd_rand& randGen, std::atomic<bool>& stop) {
  std::uniform_int_distribution<int> randInt;
  std::uniform_real_distribution<double> randReal(0.0, 1.0);
  unsigned long iterMax = state.input->numLiterals*state.input->numClauses;
  // TODO: tweak - though it seems pretty good actually
  double wp = 0.2; // probability to flip random instead of best
  State best = state;

  while (true) { // run until stopped
    for (unsigned long i=0; i<iterMax; i++) {

      if (state.cost == 0) {
        stop = true;
        return state;
      } else if (stop) {
        return best.cost < state.cost ? best : state;
      }

      // choose randomly a failed clause
      randInt.param(std::uniform_int_distribution<int>::param_type(0, state.cost-1));
      int failedNumber = randInt(randGen);
      vector<int>::iterator failedClause = nth_where(state.S.begin(), state.S.end(), failedNumber, [] (int const n) {return n==0;});
      int failedClauseIndex = std::distance(state.S.begin(), failedClause);
      const Clause& chosenClause = state.input->formula[failedClauseIndex];
      int flipLiteral = -1;
      if (randReal(randGen) < wp) {
        randInt.param(std::uniform_int_distribution<int>::param_type(0, chosenClause.size()-1));
        flipLiteral = chosenClause[randInt(randGen)];
        flipLiteral = flipLiteral > 0 ? flipLiteral : -flipLiteral;
      } else {
        int bestDelta = state.input->numClauses+1;
        for (int lit : chosenClause) {
          int absLit = lit > 0 ? lit : -lit;
          int delta = state.flipDelta(absLit);
          if (delta<bestDelta) {
            bestDelta = delta;
            flipLiteral = absLit;
          }
        }
      }
      state.flip(flipLiteral);
    }
    if (best.cost > state.cost) {
      best = state;
    }
    if (stop) {
      return best;
    }
    state.randomize(randGen);
  }
}
