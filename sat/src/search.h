#ifndef _H_SEARCH
#define _H_SEARCH

#include <future>
#include <mutex>
#include <random>
#include "state.h"


State anneal(State initial, std::minstd_rand& randGen, bool verbose);

#endif
