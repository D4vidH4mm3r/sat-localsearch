#include <algorithm>
#include <cassert>
#include <ctime>
#include <random>
#include <iostream>
#include "input.h"
#include "state.h"


using std::cout;
using std::endl;

int main(int argc, const char* argv[]) {
  // read parameters for program
  string inputName;
  if (argc < 2) {
    cout << "No args given, running default instance thing" << endl;;
    inputName = "data/uf20-010.cnf";
  } else {
    inputName = argv[1];
  }

  // read instance
  SATInput* input = new SATInput(inputName);
  cout << "Read instance with " << input->numLiterals << " literals and ";
  cout << input->numClauses << " clauses" << endl;;

  // initialize state
  SATState state(input, 0);
  int numSatisfied = input->numClauses - state.cost;
  cout << "Random instance satisfied " << numSatisfied << " and failed " << state.cost << endl;
  cout << "(" << static_cast<float>(numSatisfied) /\
    static_cast<float>(input->numClauses) * 100 << "%)" << endl;;

  // do some search
  std::random_device randDev;
  std::minstd_rand randGen(randDev());
  std::uniform_int_distribution<int> randDist(0, 0);
  for (int j=0; j<10000; j++) {

    if (state.cost == 0) {
      cout << "Nothing is failed :D" << endl;
      break;
    }

    // choose randomly a failed clause
    randDist.param(std::uniform_int_distribution<int>::param_type(0, state.cost-1));
    int chooseNumber = randDist(randGen);
    int randomFailing;
    vector<int>::iterator failingClause;
    cout << "Looking for the " << chooseNumber << "th failed" << endl;
    {
      int count = 0;
      failingClause = std::find_if(state.numSatisfying.begin(), state.numSatisfying.end(), [&] (int const n) {
          if (n == 0) {
            if (count == chooseNumber) {
              return true;
            }
            count++;
          }
          return false;
        });
      randomFailing = std::distance(state.numSatisfying.begin(), failingClause);
    }

    // find the best literal to flip in this clause
    int bestDelta = input->numClauses+1;
    int bestFlip = -1;
    for (int lit : input->formula[randomFailing]) {
      int absLit = lit>0 ? lit : -lit;
      int delta = state.flipDelta(absLit);
      if (delta < bestDelta) {
        bestDelta = delta;
        bestFlip = absLit;
      }
    }
    cout << "Best literal to flip here is " << bestFlip << " (gives Δ=" << bestDelta << ")" << endl;
    state.flip(bestFlip);
  }
  cout << state;
}
