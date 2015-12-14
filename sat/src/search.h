#ifndef _H_SEARCH
#define _H_SEARCH

#include <atomic>
#include "state.h"


// TODO: parameter constructs to experiment with
State anneal(State initial, std::minstd_rand& randGen, std::atomic<bool>& stop, int goal);
State minConflict(State initial, std::minstd_rand& randGen, std::atomic<bool>& stop, double p, int goal);

#endif
