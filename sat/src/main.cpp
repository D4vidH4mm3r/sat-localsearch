#include <algorithm>
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
  int initType = 0; // 0 for random, 1 for greedy construction heuristic
  int timeout = 20;
  int goal = 0; // good enough cost to stop immediately
  double p = 0.5;
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
      } else if (arg == "--goal" || arg == "-g") {
        iss.str(argv[i+1]);
        iss >> goal;
        i++;
      } else if (arg == "--search::strategy" || arg == "-ss") {
        iss.str(argv[i+1]);
        iss >> searchStrategy;
        i++;
      } else if (arg == "--init_type" || arg == "-it") {
        iss.str(argv[i+1]);
        iss >> initType;
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

  // do some search
  int numThreads = std::thread::hardware_concurrency();
  vector<State> states;
  vector<std::thread> threads;
  threads.reserve(numThreads);
  states.reserve(numThreads);
  for (int i=0; i<numThreads; i++) {
    states.push_back(State(input, randGen, initType));
    if (verbose) {
      int numSatisfied = input->numClauses - states[i].cost;
      cout << "Initial instance satisfied " << numSatisfied << " and failed " << states[i].cost << endl;
      cout << "(" << static_cast<float>(numSatisfied) /\
        static_cast<float>(input->numClauses) * 100 << "%)" << endl;;
    }
    if (searchStrategy == 0) {
      threads.push_back(std::thread(minConflict, ref(states[i]), ref(randGen), ref(stop), p, goal));
    } else if (searchStrategy == 1) {
      threads.push_back(std::thread(anneal, ref(states[i]), ref(randGen), ref(stop), goal));
    } else {
      throw "Unknown search type (only have 0 for min-conflict and 1 for annealing)";
    }
  }
  for (auto& thread : threads) {
    thread.join();
  }
  auto bestit = std::min_element(states.begin(), states.end());
  State best = *bestit;

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
  outputStream << best;
  outputStream << "c Time: " << timeSpent << std::endl;
  if (outputName != "") {
    fileStream.close();
  }
}
