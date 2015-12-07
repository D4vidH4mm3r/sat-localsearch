#ifndef _H_SEARCH
#define _H_SEARCH

#include <future>
#include <mutex>
#include <random>
#include "state.h"


SATState anneal(SATState initial, std::minstd_rand& randGen, bool verbose);

#endif
