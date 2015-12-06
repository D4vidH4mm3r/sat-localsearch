#include <algorithm>
#include <cassert>
#include <chrono>
#include <ctime>
#include <fstream>
#include <random>
#include <math.h>
#include <iostream>
#include "input.h"
#include "state.h"


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
  int numSatisfied = input->numClauses - state.cost;
  if (verbose) {
    cout << "Random instance satisfied " << numSatisfied << " and failed " << state.cost << endl;
    cout << "(" << static_cast<float>(numSatisfied) /\
      static_cast<float>(input->numClauses) * 100 << "%)" << endl;;
  }

  // do some search
  std::random_device randDev;
  std::minstd_rand randGen(randDev());
  std::uniform_int_distribution<int> randLit(1, input->numLiterals);
  std::uniform_real_distribution<double> randReal(0.0, 1.0);
  unsigned long iterMax = input->numClauses*input->numClauses;
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
  auto timeAfter = std::chrono::system_clock::now();
  std::chrono::nanoseconds timeSpent = timeAfter-timeBefore;
  std::ofstream fileStream;
  cout << "OUTPUT NAME: " << outputName << endl;
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
