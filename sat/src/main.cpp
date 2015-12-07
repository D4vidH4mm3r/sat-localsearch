#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include "input.h"
#include "state.h"
#include "search.h"


using std::cout;
using std::endl;
using std::ref;

int main(int argc, const char* argv[]) {
  bool verbose = false;
  // read parameters for program
  string inputName = "../data/uf20-010.cnf";
  string outputName = "";
  int randSeed = 1;
  double timeout = 20.0;
  {
    std::istringstream iss;
    for (int i=1; i<argc; i++) {
      string arg = argv[i];
      if (arg == "--main::instance") {
        inputName = argv[i+1];
        i++;
      } else if (arg == "--main::output_file") {
        outputName = argv[i+1];
        i++;
      } else if (arg == "--main::seed") {
        iss.str(argv[i+1]);
        iss >> randSeed;
        i++;
      } else if (arg == "--main::timeout" || arg == "-t") {
        iss.str(argv[i+1]);
        iss >> timeout;
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
  SATState bestState = state;
  int numSatisfied = input->numClauses - state.cost;
  if (verbose) {
    cout << "Random instance satisfied " << numSatisfied << " and failed " << state.cost << endl;
    cout << "(" << static_cast<float>(numSatisfied) /\
      static_cast<float>(input->numClauses) * 100 << "%)" << endl;;
  }

  // do some search
  std::minstd_rand randGen;
  randGen.seed(1);
  int numThreads = std::thread::hardware_concurrency();
  vector<std::future<SATState> > futures(numThreads);
  int attempts = 0;
  // TODO: better thread pooling?
  while (true) {
    for (int i=0; i<numThreads; i++) {
      state.randomize();
      futures[i] = std::async(std::launch::async, anneal, state, ref(randGen), verbose);
    }
    for (auto& future : futures) {
      SATState res = future.get();
      if (res.cost < bestState.cost) {
        bestState = res;
      }
    }
    if (bestState.cost == 0) {
      break;
    }
    // estimate whether we have time to try again
    attempts++;
    auto timeAfter = std::chrono::system_clock::now();
    double timeSpent = static_cast<double>((timeAfter-timeBefore).count())/1e9;
    double timePerAttempt = timeSpent/attempts;
    if (timeSpent + 1.5*timePerAttempt > timeout) {
      break;
    }

  }

  auto timeAfter = std::chrono::system_clock::now();
  double timeSpent = static_cast<double>((timeAfter-timeBefore).count())/1e9;
  std::ofstream fileStream;
  if (outputName != "") {
    fileStream.open(outputName.c_str(), std::ios::out);
    if (!fileStream) {
      std::cerr << "Cannot open file " << outputName << endl;
      exit(1);
    }
  }
  std::ostream& outputStream = (outputName == "" ? cout : fileStream);
  outputStream << bestState;
  outputStream << "Time: " << timeSpent << std::endl;
  if (outputName != "") {
    fileStream.close();
  }
}
