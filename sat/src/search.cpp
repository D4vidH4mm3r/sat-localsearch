#include <cmath>
#include <iostream>
#include "search.h"
#include "util.h"

using std::cout;
using std::endl;


void anneal(State& best, std::minstd_rand& randGen, std::atomic<bool>& stop, int goal) {
  std::uniform_int_distribution<int> randLit(1, best.input->numLiterals);
  std::uniform_real_distribution<double> randReal(0.0, 1.0);
  // TODO: experiment with number of steps and temperatures
  // TODO: experiment with temperature distribution
  unsigned stepsPerTemperature = best.input->numLiterals * (best.input->numLiterals-1);
  unsigned accepted = 0;
  unsigned rejected = 0;
  unsigned stepsWithoutImprovement = 0;
  int bestCost = best.input->numClauses + 1;
  double T = -1 / log(0.97);
  State state = best;

  while (true) {
    bool improved = false;
    for (unsigned i=0; i<stepsPerTemperature; i++) {

      if (state.cost <= goal || stop) {
        if (state.cost <= goal) {
          stop = true;
        }
        if (state < best) {
          best = state;
        }
        return;
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
  // TODO: reheat
}

void minConflict(State& best, std::minstd_rand& randGen, std::atomic<bool>& stop, double p, int goal) {
  std::uniform_int_distribution<int> randInt;
  std::uniform_real_distribution<double> randReal(0.0, 1.0);
  unsigned long iterMax = best.input->numLiterals*best.input->numClauses;
  State state = best;

  while (true) { // run until stopped
    for (unsigned long i=0; i<iterMax; i++) {

      if (state.cost <= goal || stop) {
        if (state.cost <= goal) {
          stop = true;
        }
        if (state < best) {
          best = state;
        }
        return;
      }

      // choose randomly a failed clause
      randInt.param(std::uniform_int_distribution<int>::param_type(0, state.cost-1));
      int failedNumber = randInt(randGen);
      vector<int>::iterator failedClause = nth_where(state.S.begin(), state.S.end(), failedNumber, [] (int const n) {return n==0;});
      int failedClauseIndex = std::distance(state.S.begin(), failedClause);
      const Clause& chosenClause = state.input->formula[failedClauseIndex];
      int flipLiteral = -1;
      if (randReal(randGen) < p) {
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
    if (state < best) {
      best = state;
    }
    if (stop) {
      return;
    }
    state.randomize(randGen);
  }
}
