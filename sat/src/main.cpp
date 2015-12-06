#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include "input.h"
#include "state.h"
#include "search.h"


using std::cout;
using std::endl;

int main(int argc, const char* argv[]) {
  bool verbose = false;
  // read parameters for program
  string inputName = "../data/uf20-010.cnf";
  string outputName = "";
  for (int i=1; i<argc; i++) {
    string arg = argv[i];
    if (arg == "--main::instance") {
      inputName = argv[i+1];
      i++;
    } else if (arg == "--main::output_file") {
      outputName = argv[i+1];
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

  // read instance
  SATInput* input = new SATInput(inputName);
  if (verbose) {
    cout << "Read instance with " << input->numLiterals << " literals and ";
    cout << input->numClauses << " clauses" << endl;;
  }

  auto timeBefore = std::chrono::system_clock::now();
  // initialize state
  SATState state(input, 0);
  SATState bestState = state;
  int numSatisfied = input->numClauses - state.cost;
  if (verbose) {
    cout << "Random instance satisfied " << numSatisfied << " and failed " << state.cost << endl;
    cout << "(" << static_cast<float>(numSatisfied) /\
      static_cast<float>(input->numClauses) * 100 << "%)" << endl;;
  }

  // do some search
  std::random_device randDev;
  std::minstd_rand randGen(randDev());
  for (int i=0; i<5; i++) {
    anneal(state, randGen, verbose);
    if (state.cost < bestState.cost) {
      if (verbose) {
        cout << "Better state (" << state.cost << " vs. " << bestState.cost << ") found" << endl;
      }
      bestState = state;
    }
    if (bestState.cost == 0) {
      break;
    }
  }

  auto timeAfter = std::chrono::system_clock::now();
  std::chrono::nanoseconds timeSpent = timeAfter-timeBefore;
  std::ofstream fileStream;
  if (outputName != "") {
    fileStream.open(outputName.c_str(), std::ios::out);
    if (!fileStream) {
      std::cerr << "Cannot open file " << outputName << endl;
      exit(1);
    }
  }
  std::ostream& outputStream = (outputName == "" ? cout : fileStream);
  outputStream << state;
  outputStream << "Time: " << static_cast<double>(timeSpent.count())/1e9 << std::endl;
  if (outputName != "") {
    fileStream.close();
  }

}
