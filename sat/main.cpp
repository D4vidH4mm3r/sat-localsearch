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
  cout << "Random instance satisfied " << state.numSatisfied << " and failed " << state.numFailed << endl;;
  cout << "(" << static_cast<float>(state.numSatisfied) /\
    static_cast<float>(input->numClauses) * 100 << "%)" << endl;;

  // do some search
  std::random_device randDev;
  std::minstd_rand randGen(randDev());
  std::uniform_int_distribution<int> randDist(0, 0);
  for (int j=0; j<100; j++) {
    randDist.param(std::uniform_int_distribution<int>::param_type(0, 0));
    int randomNonimproving = -1;
    int numNonimproving = 0;
    int gotStrictImprovement = false;
    for (int i=0; i<input->numLiterals; i++) {
      int delta = state.flipDelta(i+1);
      if (delta < 0) {
        state.flip(i+1);
        gotStrictImprovement = true;
        break;
      } else if (delta == 0) {
        numNonimproving++;
        if (randDist(randGen) == 0) {
          randomNonimproving = i;
        }
        randDist.param(std::uniform_int_distribution<int>::param_type(0, numNonimproving));
      }
    }
    if (!gotStrictImprovement) {
      if (randomNonimproving == -1) {
        cout << "There were no non-worsening flips :(" << endl;
        break;
      } else {
        //cout << "Choosing literal " << randomNonimproving+1 << " out of " << numNonimproving << " different nonimproving" << endl;
        state.flip(randomNonimproving+1);
      }
    }
    if (state.numFailed == 0) {
      cout << "Good times, nothing is failed" << endl;
      break;
    }
  }
  cout << state.numSatisfied << endl;;
}
