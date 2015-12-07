#ifndef _H_SEARCH
#define _H_SEARCH

#include "state.h"


// TODO: parameter constructs to experiment with
State anneal(State initial, std::minstd_rand& randGen, bool verbose);
State minConflict(State initial, std::minstd_rand& randGen);

#endif
