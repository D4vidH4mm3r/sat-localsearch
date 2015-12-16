#ifndef _H_SEARCH
#define _H_SEARCH

#include <atomic>
#include "state.h"


// TODO: parameter constructs to experiment with
void anneal(State&, std::minstd_rand& randGen, std::atomic<bool>& stop, int goal);
void minConflict(State&, std::minstd_rand& randGen, std::atomic<bool>& stop, double p, int goal);

#endif
