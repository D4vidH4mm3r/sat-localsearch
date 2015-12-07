#ifndef _H_SEARCH
#define _H_SEARCH

#include <mutex>
#include <random>
#include "state.h"


void anneal(SATState state, SATState& best, std::minstd_rand& randGen, bool verbose, std::mutex& mtx);

#endif
