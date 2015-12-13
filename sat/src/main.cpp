#include <cassert>
#include <chrono>
#include <csignal>
#include <fstream>
#include <future>
#include <iostream>
#include <sstream>
#include <thread>
#include "input.h"
#include "state.h"
#include "search.h"


using std::cout;
using std::endl;
using std::ref;

std::atomic<bool> stop;

int main(int argc, const char* argv[]) {
  bool verbose = false;
  // read parameters for program
  string inputName = "../data/uf20-010.cnf";
  string outputName = "";
  int randSeed = 1;
  int searchStrategy = 0; // 0 for min-conflict, 1 for simulated annealing
  int timeout = 20;
  double p = 0.2;
  {
    std::istringstream iss;
    for (int i=1; i<argc; i++) {
      string arg = argv[i];
      if (arg == "--main::instance") {
        inputName = argv[i+1];
        i++;
      } else if (arg == "--main::output_file" || arg == "-o") {
        outputName = argv[i+1];
        i++;
      } else if (arg == "-p") {
        iss.str(argv[i+1]);
        iss >> p;
        i++;
      } else if (arg == "--main::seed" || arg == "-s") {
        iss.str(argv[i+1]);
        iss >> randSeed;
        i++;
      } else if (arg == "--search::strategy" || arg == "-ss") {
        iss.str(argv[i+1]);
        iss >> searchStrategy;
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
      iss.clear();
    }
  }
  auto timeBefore = std::chrono::system_clock::now();
  // spawn thread which will later stop search if need be
  stop = false;
  std::thread([&](){
      std::this_thread::sleep_for(std::chrono::seconds(timeout));
      if (verbose) {
        cout << "Search stopped by timer process!" << endl;
      }
      stop = true;
    }).detach();
  auto stopper = [](int s) {
    stop = true;
  };
  // signal 6 used by tests
  std::signal(SIGIOT, stopper);
  // signal used by Ctrl-C
  std::signal(SIGINT, stopper);

  // read instance
  Input* input = new Input(inputName);
  if (verbose) {
    cout << "Read instance with " << input->numLiterals << " literals and ";
    cout << input->numClauses << " clauses" << endl;;
  }

  // initialize random generator
  std::minstd_rand randGen(randSeed);

  // initialize state
  State state(input, randGen);
  State bestState = state;

  if (verbose) {
    int numSatisfied = input->numClauses - state.cost;
    cout << "Random instance satisfied " << numSatisfied << " and failed " << state.cost << endl;
    cout << "(" << static_cast<float>(numSatisfied) /\
      static_cast<float>(input->numClauses) * 100 << "%)" << endl;;
  }

  // do some search
  int numThreads = std::thread::hardware_concurrency();
  vector<std::future<State> > futures(numThreads);
  cout << "p: " << p << endl;
  for (int i=0; i<numThreads; i++) {
    state.randomize(randGen);
    if (searchStrategy == 0) {
      futures[i] = std::async(std::launch::async, minConflict, state, ref(randGen), ref(stop), p);
    } else if (searchStrategy == 1) {
      futures[i] = std::async(std::launch::async, anneal, state, ref(randGen), ref(stop));
    } else {
      throw "Unknown search type (only have 0 for min-conflict and 1 for annealing)";
    }
  }
  for (auto& future : futures) {
    State res = future.get();
    if (res.cost < bestState.cost) {
      bestState = res;
    }
    if (bestState.cost == 0) {
      goto done_searching;
    }
  }

 done_searching:
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
  outputStream << "c Time: " << timeSpent << std::endl;
  if (outputName != "") {
    fileStream.close();
  }
}
