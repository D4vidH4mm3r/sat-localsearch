#include <algorithm>
#include <cassert>
#include <chrono>
#include <ctime>
#include <random>
#include <iostream>
#include "input.h"
#include "state.h"


using std::cout;
using std::endl;

int main(int argc, const char* argv[]) {
  bool verbose = false;
  // read parameters for program
  string inputName;
  if (argc < 2) {
    cout << "No args given, running default instance thing" << endl;;
    inputName = "data/uf20-010.cnf";
  } else {
    for (int i=1; i<argc; i++) {
      string arg = argv[i];
      if (arg == "--main::instance") {
        inputName = argv[i+1];
        i++;
      } else if (arg == "--main::seed") {
        // doesn't use seed right now
        i++;
      } else if (arg == "--verbose" || arg == "-v") {
        verbose = true;
      } else {
        inputName = arg;
      }
    }
  }

  // read instance
  SATInput* input = new SATInput(inputName);
  if (verbose) {
    cout << "Read instance with " << input->numLiterals << " literals and ";
    cout << input->numClauses << " clauses" << endl;;
  }

  auto timeBefore = std::chrono::system_clock::now();
  // initialize state
  SATState state(input, 0);
  int numSatisfied = input->numClauses - state.cost;
  if (verbose) {
    cout << "Random instance satisfied " << numSatisfied << " and failed " << state.cost << endl;
    cout << "(" << static_cast<float>(numSatisfied) /\
      static_cast<float>(input->numClauses) * 100 << "%)" << endl;;
  }

  // do some search
  std::random_device randDev;
  std::minstd_rand randGen(randDev());
  std::uniform_int_distribution<int> randDist(0, 0);
  for (int j=0; j<10000; j++) {

    if (state.cost == 0) {
      if (verbose) {
        cout << "Nothing is failed :D" << endl;
      }
      break;
    }

    // choose randomly a failed clause
    randDist.param(std::uniform_int_distribution<int>::param_type(0, state.cost-1));
    int chooseNumber = randDist(randGen);
    int randomFailing;
    vector<int>::iterator failingClause;
    if (verbose) {
      cout << "Looking for the " << chooseNumber << "th failed" << endl;
    }
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
    if (verbose) {
      cout << "Best literal to flip here is " << bestFlip << " (gives Δ=" << bestDelta << ")" << endl;
    }
    state.flip(bestFlip);
  }
  auto timeAfter = std::chrono::system_clock::now();
  std::chrono::nanoseconds timeSpent = timeAfter-timeBefore;
  cout << state;
  cout << "Time: " << static_cast<double>(timeSpent.count())/1e9 << std::endl;
}
